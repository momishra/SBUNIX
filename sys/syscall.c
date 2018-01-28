#include<sys/syscall.h>
#include<sys/sched.h>
#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/gdt.h>
#include<sys/elf64.h>
#include<sys/types.h>
#include<sys/idt.h>
#include<sys/pmap.h>

char comm_buffer[4096];
extern void syscall();

void test1(){
  kprintf("\nJAI MATA DI!!\n");
}

void *syscalls[20] = 
{
&test1,
&putc,
&getpid,
&execve,
&exit_process,
&myfork,
&list_process,
&stdin_read,
&contextSwitch,
&list_files,
&exit_process,
&read_file,
&chdir,
&PWD,
&SLEEP,
&sys_write
};

void init_syscall()
{
  //void *fun = syscalls[0];
  //set_idt_handler(0x80, (uint64_t)syscall);
  set_idt_user(0x80,(uint64_t)syscall);
  //set_idt_user(0x81,(uint64_t)contextSwitch);
}



void syscall_wrap(struct register_t *r) {
   uint64_t fun=0;
   __asm__ volatile("movq %%rax, %0;":"=r"(fun));
   
   void *function = syscalls[fun];
   //void *function = syscalls[r->rax];
   uint64_t ret;/*
   __asm__ volatile (  
     "pushq %1;" 
     "pushq %2;" 
     "pushq %3;" 
     "pushq %4;" 
     "pushq %5;" 
     "callq *%6;" 
     "popq %%rbx;" 
     "popq %%rbx;" 
     "popq %%rbx;" 
     "popq %%rbx;"
     "popq %%rbx;" 
    : "=a" (ret) : "r" (r->rdi), "r" (r->rsi), "r" (r->rdx), "r" (r->rcx), "r" (r->rbx), "r" (location));*/
    
    __asm__ volatile (  
     "pushq %%rdi;"
     "pushq %%rsi;"
     "pushq %%rbx;"
     "pushq %%rcx;"
     "pushq %%rdx;"
     "movq %1, %%rdi;"
     "movq %2, %%rsi;"
     "movq %3, %%rdx;"
     "movq %4, %%rcx;"
     "movq %5, %%r8;"
     "callq *%6;" 
     "popq %%rdx;"
     "popq %%rcx;"
     "popq %%rbx;"
     "popq %%rsi;"
     "popq %%rdi;" 
     : "=a" (ret) 
     : "r" (r->rdi), "r" (r->rsi),  "r" (r->rbx),"r" (r->rcx), "r" (r->rdx), "r" (function)
     : "rsi","rdi","rbx","rcx","rdx"
     );
     //*((uint64_t*)curr_task->stack[490])=ret;
     __asm__ volatile("movq %0, %%rax"::"r"(ret));
     
   //r->rax = ret;
   /*
   __asm__ volatile(
     "pushq %%rdi;"
    
     "movq %1, %%rdi;" 
     "callq *%6;" 
     
     "popq %%rdi;"
    : "=a" (ret) : "r" (r->rdi), "r" (r->rsi), "r" (r->rdx), "r" (r->rcx), "r" (r->rbx), "r" (function));
   r->rax = ret;*/
}
/*
uint64_t sched_sys(){
  int a;  
 	__asm__ volatile("int $0x80" : "=a" (a) : "r" (1));  
	return a;

}*/


void handle_page_fault(struct register_t *r){
    uint64_t faulting_addr = 0;
    __asm__ __volatile__ ("movq %%cr2, %0" : "=a" (faulting_addr));
    
	  int present = !(r->err_code & PF_P);
	  int rw = r->err_code & PF_W;
	  int us = r->err_code & PF_U;

    kprintf("Faulting address is %p\n", faulting_addr);
	  if(present) kprintf("Page fault error : present\n");
	  if(rw) kprintf("Page fault error : read-only\n");
	  if(us) kprintf("Page fault error : user-mode\n");

    //uint64_t err = r->err_code & 0xF; // only last 3 bits are imp
   // struct task_struct *curr = (struct task_struct *)readyQ[1]; 
    //struct task_struct *curr = curr_task;
   // struct vm_area_struct *vma = find_vma(curr->mm, faulting_addr);
    //kprintf("vma is %p\n",vma);
    //uint64_t start = FLOOR(faulting_addr, PGSIZE);
    /*uint64_t* phys_addr;
    uint32_t write_perm = 0;*/

	/*
    if(vma == NULL) {
        if(faulting_addr >= (r->userrsp-0x20) && faulting_addr <= (r->userrsp)){
			vma = grow_stack();
            if(vma==NULL){
                handle_fatal_error("Stack OverFlow!!");
            }
        }else{
            handle_fatal_error("segment violation");
        }
    }*/
	/*
    if(vma->vm_flags & PF_W ){
        write_perm =1;
    }
        
	if( (err & PF_P) && (err & PF_W)){
		if(write_perm){
            uint64_t phys = pa((uint64_t*)start) & ~0XFFF; //pa??
            if(get_reference_count((uint64_t*) phys)==1){ //refernce count of the page?
				        set_write((uint64_t*)start);
                flush_TLB();
			}
			else{
         uint64_t* new = get_zeroed_page();
         phys_addr = (uint64_t*)(pa(new) & ~0XFFF) ;
         memcpy((void*)new,(void*)start,PAGE_SIZE);
         map(phys_addr, (uint64_t*)(start), write_perm, 1);
         flush_TLB();
         free_phys_page((uint64_t*)phys); //decrement ref count
     }
            return;
        }
        else{
                while(1); //seg fault
            }       
        }
        
        //LOG("-------PAGE FAULT ROUTINE CALLED & TRYING TO READ or present bit not set------\n");
        if( (err & PAGEFAULT_P) && !(err & PAGEFAULT_W)){
            //uint64_t pt = pa((uint64_t*)start);
            //printf("pt entry in child is %lx\n",pt);
            //printf("User or Supervisor process tried to read or write to a page its not allowed to\n");
            // Raise segv fault. Exit the process to-do 
            while(1);
        }
        	
        phys_addr = alloc_phys_page(NULL);
        map(phys_addr, (uint64_t*)(start), write_perm, 1);
        memset((void *)start, 0, PAGE_SIZE);
		if( vma->vm_file == NULL){
			//printf("anonymous vma \n ");
			//return;
		}
		else{
			uint64_t diff, copy_sz, end,dest, src;
            end = start+PGSIZE;
            diff =0;

            if(start <= vma->vm_start){
                dest = vma->vm_start;
                src = vma->vm_file->file_start + vma->vm_file->vm_pgoff ;
            }
            else{
                dest = start;
                diff = start - vma->vm_start;
                src = vma->vm_file->file_start + vma->vm_file->vm_pgoff + diff; 
            }
            
            if((vma->vm_file->vm_sz) <= PAGE_SIZE){
                copy_sz = vma->vm_file->vm_sz;
            }
            else if(end <= (vma->vm_end - vma->vm_file->bss_size)){
                copy_sz = PAGE_SIZE;
            }
            else{
                copy_sz = end - (vma->vm_end - vma->vm_file->bss_size);
            }

            memcpy((void*)dest,(void*) src,copy_sz);  
                    
        }*/
}

