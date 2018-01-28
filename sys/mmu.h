#ifndef MMU_H
#define MMU_H

//address is pageno + offset
#define PAGENO(addr) (((uint64_t)(addr)) >> PTXSHIFT) //get pageno

//part of address 
#define PML4(addr) ((((uint64_t)(addr)) >> PML4SHIFT) & 0x1FF) //PML4 address part
#define PDPE(addr) ((((uint64_t)(addr)) >> PDPESHIFT ) & 0x1FF) //PDPE part
#define PDE(addr) ((((uint64_t)(addr)) >> PTSHIFT ) & 0x1FF) //PDE part
#define PTE(addr) ((((uint64_t)(addr)) >> PGSHIFT) & 0x1FF) //PTE part
#define POFF(addr) (((uint64_t)(addr)) & 0xFFF) //offset part
#define PTX(la) ((((uint64_t)(la))>>PTXSHIFT)&0x1FF) //Page table index
#define PDX(la) ((((uint64_t)(la)) >> PDXSHIFT) & 0x1FF)
//the above obtained addresses will be combined with prev table entry to find next table entry

#define PGADDR(pml,pdpe,pde,pte,off) ((void *) ((pml) << PML4SHIFT | (pdpe) << PDPESHIFT | (pde) << PTSHIFT | (pte) << PGSHIFT | off)) //get the address from each part again
#define PTE_AVAIL 0xE00 //available for software use
#define PTE_USER	(PTE_AVAIL | PTE_P | PTE_W | PTE_U)
//512 because of addition of 4th level - PML4
#define NUMPLM 512 //plm4e
#define NUMPDPE 512 //page directory pointer entry
#define NUMPDE 512 //page directory entry
#define NUMPTE 512 //page table entry

#define PGSIZE 4096 //bytes
#define PGSHIFT 12 //len in bits of offset part of virtual address
#define PTXSHIFT 12 //offset of PTX in a linear address
#define PTSIZE (PGSIZE*NUMPTE) //512*4096
#define PTSHIFT 21 //page table
#define PDXSHIFT 21 //offset of a PDX in a linear Address
#define PDPESHIFT 30 //page directory pointer
#define PML4SHIFT 39 //PML4
#define PTE_ADDR(pte)	((uint64_t) (pte) & ~0xFFF)
#define PTE_P 0x001 //Present bit
#define PTE_R 0x002 //read\write
#define PTE_U 0x004 //user\supervisor
#define PTE_W 0x008 //write-through
#define PTE_C 0x010 //cache-disabled
#define PTE_A 0x020 //accessed bit
#define PTE_D 0x040 //dirty bit
#define PTE_S 0x080 //page size
#define PTE_G 0x180 //ignored

//Control register flags
#define CR0_PE 0x00000001
#define CR0_MP 0x00000002
#define CR0_EM 0x00000004
#define CR0_TS 0x00000008
#define CR0_ET 0x00000010
#define CR0_NE 0x00000020
#define CR0_WP 0x00010000
#define CR0_AM 0x00040000
#define CR0_NW 0x20000000
#define CR0_CD 0x40000000
#define CR0_PG 0x80000000

//not added CR4 --for now

//flag register -- for status of processor
#define E_CF 0x00000001
#define E_PF 0x00000004
#define E_AF 0x00000010
#define E_ZF 0x00000040
#define E_SF 0x00000080
#define E_TF 0x00000100
#define E_IF 0x00000200
#define E_DF 0x00000400
#define E_OF 0x00000800
#define E_IOPL_MASK 0x00003000
#define E_IOPL_0 0x00000000
#define E_IOPL_1 0x00001000
#define E_IOPL_2 0x00002000
#define E_IOPL_3 0x00003000
#define E_NT 0x00004000
#define E_RF 0x00010000
#define E_VM 0x00020000
#define E_AC 0x00040000
#define E_VIF 0x00080000
#define E_VIP 0x00100000
#define E_ID 0x00200000

//page fault error codes
#define PFE_PR 0x1 //by protection -- not set the by non-present page
#define PFE_WR 0x2 //by page write -- not set then by page read
#define PFE_U 0x4 //CPL = 3

#endif
