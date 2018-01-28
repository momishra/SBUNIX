#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/pci.h>
#include<sys/ahci.h>
void checkBus(uint8_t bus);
uint16_t getDeviceID(uint8_t bus, uint8_t device,uint8_t func);


unsigned long readBARs(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset){
 	uint32_t address;
        uint32_t lbus = (uint32_t)bus;
        uint32_t ldevice = (uint32_t)device;
        uint32_t lfunc = (uint32_t)func;
        //uint32_t tmp =0;
        uint32_t tmp1;
        address = (uint32_t)((lbus << 16) |(ldevice << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
        __asm__ volatile ("outl %0,%w1" : : "a"(address),"d"(0xCF8));
        __asm__ volatile("inl %w1, %0" : "=a"(tmp1) : "d"(0xCFC));
        return tmp1;
}

uint16_t pciConfigRead(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset){
	uint32_t address;
	uint32_t lbus = (uint32_t)bus;
	uint32_t ldevice = (uint32_t)device;
	uint32_t lfunc = (uint32_t)func;
	uint16_t tmp =0;
	uint32_t tmp1;
	address = (uint32_t)((lbus << 16) |(ldevice << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
	__asm__ volatile ("outl %0,%w1" : : "a"(address),"d"(0xCF8));
	__asm__ volatile("inl %w1, %0" : "=a"(tmp1) : "d"(0xCFC));
	tmp = (uint16_t)((tmp1 >> ((offset & 2)* 8)) & 0xFFFF);
	return tmp;
	}

uint16_t getVendorID(uint8_t bus, uint8_t device, uint8_t func){
        uint16_t vid = (uint16_t)pciConfigRead(bus,device,func,0);
        return vid;
}

uint8_t getHeaderType(uint8_t bus, uint8_t device, uint8_t func){
        uint8_t tmp = (uint8_t)(pciConfigRead(bus,device,func,0xE));
        //uint8_t headerType = (uint8_t)tmp;
        return tmp;
}

uint32_t getBARs(uint8_t bus,uint8_t device, uint8_t func,uint8_t offset){
        uint32_t bar1 = (uint32_t)pciConfigRead(bus,device,func,offset);
        return bar1;
}

void checkFunction(uint8_t bus,uint8_t device, uint8_t func){
        uint8_t secBus;
        uint8_t baseClass = (uint8_t)((pciConfigRead(bus,device,func,0x0A) >> 8) & 0xFF);
        uint8_t subClass = (uint8_t)(pciConfigRead(bus,device,func,0x0A) & 0xFF);
	uint8_t progif = (uint8_t)((pciConfigRead(bus,device,func,0x09)>>8) & 0xFF);
	uint16_t device_id = getDeviceID(bus,device,func);
	if((baseClass == 0x01) && (subClass == 0x06) && (progif==0x01)){
	kprintf("***AHCI FOUND*** Device ID= %x :: Vendor ID= %x ",device_id,getVendorID(bus,device,func));
	//uint32_t bar5_0 = (uint32_t)getBARs(bus,device,func,0x24) ;
        //uint32_t bar5_1 = (uint32_t)getBARs(bus,device,func,0x26)<<16;
        uint32_t bar5 = readBARs(bus,device,func,0x24);
        __asm__ volatile ("outl %0,%w1" : : "a"(0xA6000),"d"(0xCFC));
	int i= 100000000;
	while(i>0){i--;	}
	//bar5_0 = (uint32_t)getBARs(bus,device,func,0x24) ;
        //bar5_1 = (uint32_t)getBARs(bus,device,func,0x26)<<16;
	//bar5 = bar5_0 + bar5_1;
	bar5  = readBARs(bus,device,func,0x24);	
	//kprintf("New bar5 is %x",bar5);
        //abar= (hba_mem_t *)(uint64_t)bar5;
        probe_port((hba_mem_t *)(uint64_t)bar5);
	}
        if((baseClass == 0x06) && (subClass == 0x04)){//0x06 base class is bridge device and 0x04 sub class is PCI-to-PCI bridge
		//kprintf("Bridge device");
		secBus = (uint8_t)((pciConfigRead(bus,device,func,0x19)>>8) & 0xFF);// mask 8-bits to get correct sec Bus
                checkBus(secBus);
        }
}

uint16_t getDeviceID(uint8_t bus, uint8_t device,uint8_t func){
	uint16_t deviceID = (uint16_t)pciConfigRead(bus,device,func,2);
	return deviceID;
}


void checkDevice(uint8_t bus, uint8_t device){
	//kprintf("in checkdevice");
        uint8_t func =0;
        uint16_t vendorID = getVendorID(bus,device,func);
        if(vendorID == 0xFFFF) return;
	//uint16_t device_id = getDeviceID(bus,device,func);
        checkFunction(bus,device,func);
        uint8_t headType = getHeaderType(bus,device,func);
	if((headType & 0x80) != 0){
		for(func = 1; func <8; func++){
			if(getVendorID(bus,device,func) != 0xFFFF){
				checkFunction(bus,device,func);
				}
		}
	}
}

void checkBus(uint8_t bus){
        uint8_t device;
        for(device =0; device <32 ; device ++){
                //if(getVendorID(bus,device,0) == 0xFFFF){
                //        continue;
                //}else{
		      checkDevice(bus,device);
                 //       }
                }
}

void checkAllBuses(void){
	uint8_t bus;
	uint8_t func;
	uint8_t head = getHeaderType(0,0,0);
	//kprintf("head : %p",head);
	if((head & 0x80) == 0){//end-point device
	       checkBus(0);
	}else{//multi-function device
		for(func=0; func < 8;func++){
			if(getVendorID(0,0,func) != 0xFFFF)break;
			bus = func;
			checkBus(bus);
		}
	}
	}


// Start command engine
void start_cmd(hba_port_t *port)
{
	int i= 1000;
	while(i>0){i--;}
        // Wait until CR (bit15) is cleared
        while (port->cmd & HBA_PxCMD_CR);

        // Set FRE (bit4) and ST (bit0)
        port->cmd |= HBA_PxCMD_FRE;
        port->cmd |= HBA_PxCMD_ST;
}

// Stop command engine
void stop_cmd(hba_port_t *port)
{
        // Clear ST (bit0)
        port->cmd &= ~HBA_PxCMD_ST;

        int i=1000;
	while(i>0){i--;}
	// Wait until FR (bit14), CR (bit15) are cleared
        while(1)
        {
                if (port->cmd & HBA_PxCMD_FR)
                        continue;
                if (port->cmd & HBA_PxCMD_CR)
                        continue;
                break;
        }

        // Clear FRE (bit4)
        port->cmd &= ~HBA_PxCMD_FRE;
}


void port_rebase(hba_port_t *port, int portno)
{
	//kprintf("Initialising Port Memory ");
	abar->ghc=(uint32_t)(1<<31);
    	abar->ghc=(DWORD)(1<<0);
    	abar->ghc=(DWORD)(1<<31);
    	abar->ghc=(DWORD)(1<<1);
	//kprintf("ghc value : %p::",abar->ghc);
	stop_cmd(port); 
	port->cmd=port->cmd & 0xffff7fff; //Bit 15 CR
    	port->cmd=port->cmd & 0xffffbfff; //Bit 14 FR
    	port->cmd=port->cmd & 0xfffffffe; //Bit 0 ST
    	port->cmd=port->cmd & 0xfffffff7; //Bit 4	FRE
	port->clb = (AHCI_BASE ) & 0xFFFFFFFF;
	//port->clbu = 0;
	//memset((void*)(port->clb), 0, 1024);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	//port->fb = (AHCI_BASE + (32<<10) + (portno<<8)) & 0xFFFFFFFF;
	port->fb = ((((uint64_t)AHCI_BASE+ (uint64_t)((32<<10)))>>32)&0xffffffff) | (((uint64_t)AHCI_BASE + (uint64_t)((32<<10)))&0xffffffff);
	//port->fbu = 0;
	//memset((void*)(port->fb), 0, 256);
 	

	port->serr_rwc =1;
	port->is_rwc = 0;
	port->ie= 1 ;	
	
	uint64_t clb_addr=0,ctb_addr;
	clb_addr = (((clb_addr)<<32) | port->clb);
	clb_addr = clb_addr + 0xFFFFFFFF00000000;
	memset((void*)clb_addr,0,1024);


	uint64_t fbu_addr =0;
	fbu_addr = (((fbu_addr)<<32) | port->fb);
        fbu_addr = fbu_addr + 0xFFFFFFFF00000000;
        memset((void*)fbu_addr,0,256);

	clb_addr = 0;
	clb_addr = (((clb_addr)<<32) | port->clb);
        clb_addr = clb_addr + 0xFFFFFFFF00000000;

	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)(clb_addr);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		//cmdheader[i].ctba = (AHCI_BASE + (40<<10) + (portno<<13) + (i<<8)) & 0xFFFF;
		cmdheader[i].ctba = ((((uint64_t) AHCI_BASE + (uint64_t)((40 << 10)) + (uint64_t)((i <<8))) >> 32) & 0xffffffff) |
					(((uint64_t)AHCI_BASE + (uint64_t)((40<<10)) + (uint64_t)((i <<8))) & 0xffffffff);
		//cmdheader[i].ctbau = 0;
		ctb_addr = 0;
		ctb_addr = cmdheader[i].ctba;
		memset((void*)ctb_addr, 0, 256);
	}
 
	start_cmd(port);	// Start command engine
	port->is_rwc = 0;
	port->ie = 0xffffffff;
	kprintf("!! Done Port Memory Initialisation !!");
}

static int check_type(hba_port_t *port){
	//DWORD ssts = port->ssts;
	//BYTE ipm = (ssts >> 8) & 0x0F;
	//BYTE det = ssts & 0x0F;
	//if (det != HBA_PORT_DET_PRESENT)	// Check drive status
	//	return AHCI_DEV_NULL;
	//if (ipm != HBA_PORT_IPM_ACTIVE)
	//	return AHCI_DEV_NULL;
	int i =100000;
	while(i>0){i--;}
	//kprintf("port->sig %p::",port->sig);
	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}

void probe_port(hba_mem_t *abar_temp)
{
	uint32_t pi = abar_temp->pi;
	//uint16_t buf[8*32*4096];
	int i = 0;
	kprintf("pi: %d ::",pi);
	//uint16_t *test = (uint16_t *)((0x800000) + (1*0x1000));
	while (i<32)
	{
		//kprintf("::inside while::");
		if (pi & 1)
		{	
			//kprintf("inside if");
			//kprintf("sig %p::",(&abar_temp->ports[1])->sig);
			int dt = check_type(&abar_temp->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				kprintf("SATA drive found at port %d", i);
				//kprintf("port list %x",abar->ports[i]);
				abar = abar_temp;
				port_rebase(abar_temp->ports, i);
				//buf[4096] = 5;	
				//write(&abar_temp->ports[0],0,0,16,buf);
				//read(&abar_temp->ports[0],0,1,16,buf);
				//kprintf("%d",buf);
				//break;
			}
			
			else if (dt == AHCI_DEV_SATAPI)
			{
				//kprintf("SATAPI drive found at port %d", i);
				//abar = abar_temp;
				//break;
			}
			/*
			else if (dt == AHCI_DEV_SEMB)
			{
			//	kprintf("SEMB drive found at port %d", i);
			}
			else if (dt == AHCI_DEV_PM)
			{
			//	kprintf("PM drive found at port %d", i);
			}
			else
			{
			//	kprintf("No drive found at port %d", i);
			}*/
		}
 
		pi >>= 1;
		//int j =10000000;
		//while(j>0){j--;}
		i ++;
	}
}
/*
// Find a free command list slot
int find_cmdslot(hba_port_t *port)
{
        // If not set in SACT and CI, the slot is free
        DWORD slots = (port->sact | port->ci);
	int cmdslots = (abar->cap & 0xF00)>>8;
        for (int i=0; i<cmdslots; i++)
        {
                if ((slots&1) == 0)
                        return i;
                slots >>= 1;
        }
        kprintf("Cannot find free command list entry");
        return -1;
}

int write(hba_port_t *port, DWORD startl, DWORD starth, DWORD count, WORD *buf)
{	

	kprintf("Start writing");
	kprintf("tfd val is %x",port->tfd);
        port->is_rwc = (DWORD)-1;               // Clear pending interrupt bits
	//port->is_rwc = 0xffff;
        int spin = 0; // Spin lock timeout counter
        int slot = find_cmdslot(port);
        if (slot == -1)
                return 0;

        hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
        cmdheader += slot;
        cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(DWORD);   // Command FIS size
        cmdheader->w = 1;               // Read from device
        cmdheader->prdtl = (WORD)((count-1)>>4) + 1;    // PRDT entries count

        hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
        //memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t) + (cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));
        int i=0;
        // 8K bytes (16 sectors) per PRDT
        for (i=0; i<cmdheader->prdtl-1; i++)
        {
                cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
                cmdtbl->prdt_entry[i].dbc = 8*1024;     // 8K bytes
                cmdtbl->prdt_entry[i].i = 1;
                buf += 4*1024;  // 4K words
                count -= 16;    // 16 sectors
        }
        // Last entry
        cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
        cmdtbl->prdt_entry[i].dbc = count<<9;   // 512 bytes per sector
        cmdtbl->prdt_entry[i].i = 1;
        // Setup command
        fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);

        cmdfis->fis_type = FIS_TYPE_REG_H2D;
        cmdfis->c = 1;  // Command

	 cmdfis->command = ATA_CMD_READ_DMA_EX;

        cmdfis->lba0 = (BYTE)startl;
        cmdfis->lba1 = (BYTE)(startl>>8);
        cmdfis->lba2 = (BYTE)(startl>>16);
        cmdfis->device = 1<<6;  // LBA mode

        cmdfis->lba3 = (BYTE)(startl>>24);
        cmdfis->lba4 = (BYTE)starth;
        cmdfis->lba5 = (BYTE)(starth>>8);

        cmdfis->countl = count & 0xFF;
        cmdfis->counth = count >>8;

        // The below loop waits until the port is no longer busy before issuing a new command
        while ((port->tfd & (ATA_STATUS_BSY | ATA_STATUS_DRQ)) && spin < 1000000)
        {
                spin++;
        }
        if (spin == 1000000)
        {
                kprintf("Port is hung");
                return 0;
        }

        port->ci = 1<<slot;     // Issue command

        // Wait for completion
        while (1)
        {
                // In some longer duration reads, it may be helpful to spin on the DPS bit
                // in the PxIS port field as well (1 << 5)
                if ((port->ci & (1<<slot)) == 0)
                        break;
                if (port->is_rwc & HBA_PxIS_TFES)       // Task file error
                {
                        kprintf("Read disk error");
			 return 0;
                }
        }

        // Check again
        if (port->is_rwc & HBA_PxIS_TFES)
        {
                kprintf("Read disk error");
                return 0;
        }

        return 1;
}



int read(hba_port_t *port, DWORD startl, DWORD starth, DWORD count, WORD *buf)
{	
	kprintf("Start reading!");
	port->is_rwc = (DWORD)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
		return 0;
 
	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(DWORD);	// Command FIS size
	cmdheader->w = 0;		// Read from device
	cmdheader->prdtl = (WORD)((count-1)>>4) + 1;	// PRDT entries count
 
	hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t) + (cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));
	int i=0; 
	// 8K bytes (16 sectors) per PRDT
	for (i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
	// Setup command
	fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
 
	cmdfis->lba0 = (BYTE)startl;
	cmdfis->lba1 = (BYTE)(startl>>8);
	cmdfis->lba2 = (BYTE)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (BYTE)(startl>>24);
	cmdfis->lba4 = (BYTE)starth;
	cmdfis->lba5 = (BYTE)(starth>>8);
 
	cmdfis->countl = count & 0xFF;
	cmdfis->counth = count >>8;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_STATUS_BSY | ATA_STATUS_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		kprintf("Port is hung");
		return 0;
	}
 
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is_rwc & HBA_PxIS_TFES)	// Task file error
		{
			kprintf("Read disk error");
			return 0;
		}
	}
 
	// Check again
	if (port->is_rwc & HBA_PxIS_TFES)
	{
		kprintf("Read disk error");
		return 0;
	}
 
	return 1;
}*/
