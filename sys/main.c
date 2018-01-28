#include<sys/pmap.h>
#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/sched.h>
#include<sys/memlayout.h>
//#include <sys/ahci.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include<sys/elf64.h>
#include<sys/syscall.h>
#include<sys/str.h>
//#include <sys/pci.h>

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;
extern uint64_t boot_cr3;
extern pml4e_t *boot_pml4e;
//extern static unsigned short *ptr = (unsigned short*)0xb8000;

void idleProcess(){
  kprintf("in idle process\n");
  schedule();
  kprintf("\nidle\n");
}

void start(uint32_t *modulep, void *physbase, void *physfree)
{  
 	 mem_initialise(modulep,physbase,physfree);
    kprintf("Paging Initialised.....\n");
   //init_gdt();
   set_idt();
   isr_install();
   idt_install();
   init_syscall();
   kprintf("Syscalls Enabled.....\n");
    int_handler(); 
   //key_install();
  	//__asm__ volatile("INT $32");
  	__asm__ volatile("sti");
   
  	//checkAllBuses();
  	//checkbuses();
   tarfs_init();
   kprintf("Tarfs initialiased.....\n");
   //uint64_t start = tarfs_open_dir("etc/");
   //tarfs_read_dir(start);
   //tarfs_obj *entry = tarfs_file_open("etc/test.txt");
   //char buf[1024];
   //tarfs_file_read(entry,sizeof(buf),(uint64_t)buf);
   /******Kernel Task Creation - START********/
   //ready = (struct task_struct *)kmalloc(sizeof(struct task_struct));
   struct task_struct *pcb0 = (struct task_struct *)kmalloc(sizeof(struct task_struct));
   pcb0->pml4e = boot_pml4e;    
   pcb0->cr3 = boot_cr3;    
   pcb0->pid = nextpid;    
   pcb0->stack = (uint64_t*)initial_stack;
   pcb0->rsp = (uint64_t *)&pcb0->stack[STACK_SIZE];
   pcb0->rip = (uint64_t)&idleProcess;
   char f[40]= "init";
   strcopy(pcb0->pname, f);
   //pcb0->pname[40] = "initial_process";
   process_count = 0;
   //ready = pcb0;
   num_ready_list++;
   //ready->ntask = NULL;
   /******Kernel Task Creation - END********/
   
   readyQ[process_count]=(uint64_t)pcb0;
   
   /********USER PROCESSES - START********/
    struct task_struct *pcb1 = loadpcb("bin/sbush");
    //ready->ntask = pcb1;
   //kprintf("process 1 %p\n",pcb1);
   //readyQ[1]=(uint64_t)pcb1;
   //__asm__ volatile("movq %0,%%cr3"::"r"(pcb1->cr3));
   
   //dummy();
   
   //__asm__ volatile("movq %0, %%cr3;"::"r"(pcb1->cr3));
   
   
   switch_process(pcb1);
   //switch_to_user_mode(pcb1);
   //switch2User((uint64_t)&fun2);
   //__asm__ volatile("mov $0x2b,%ax");
    //__asm__ volatile("ltr %ax");
   //contextSwitch(); 
   //mo_switch((uint64_t)pcb1->rip);
   kprintf("\nBack to the main function\n");
   while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp2;
  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
  load_screen();
  init_gdt();
  //set_idt();
  //isr_install();
  //idt_install(); 
  //int_handler(); 
	 
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
/*
 for(
 nitialise(modulep,physbase,physfree);   temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;
*/
  while(1) __asm__ volatile ("hlt");
}
