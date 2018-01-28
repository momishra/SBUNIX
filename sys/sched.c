#include<sys/sched.h>
#include<sys/pmap.h>
#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/gdt.h>
#include<sys/elf64.h>
#include<sys/types.h>
#include<sys/isr.h>
#include<sys/tarfs.h>
#include<sys/str.h>
uint64_t *old_rsp;
uint64_t *old_rip;
uint64_t ret_pid;
int isFirst=1;
char cwd[1024];
/*
void switch_to_user_mode(struct task_struct* ts){
  kprintf("Inside switch to user mode");
  set_tss_rsp(&(ts->kstack[511]));
  
  __asm__ volatile(
    "cli;"
    "movq %0,%%cr3;"
    "movq $0x23, %%rax;"
    "movq %%rax, %%ds;"
    "movq %%rax, %%es;"
    "movq %%rax, %%fs;"
    "movq %%rax, %%gs;"
    "movq %1, %%rax;"
    "pushq $0x23;"
    "pushq %%rax;"
    "pushfq;"
    "pushq $0x2b;"
    "pushq %2;"
    "movq $0x0, %%rdi;"
    "movq $0x0, %%rsi;"
    "iretq;"
    ::"r"(ts->cr3),"r"(ts->rsp),"r"(ts->rip)    
  );
}
*/

void switch_to_user_mode(struct task_struct* ts){
  kprintf("Inside switch to user mode");
  //set_tss_rsp(&(ts->kstack[510]));
  /*
  __asm__ volatile(
    "movq %rsp, %rcx;"
    "pushq $0x23;"
    "pushq %rcx;"
    "pushfq;"
    "pushq $0x2b;"
    "pushq %rdi;"
    "iretq;"    
  );
  */
  set_tss_rsp(&(ts->kstack[511]));
}


void dummy()
{
  /*curr_task = (struct task_struct *)kmalloc(sizeof(struct task_struct *));
  struct task_struct *head;
  head = curr_task;
  //uint64_t *pml4 = (uint64_t*)page_alloc(0);
  //curr_task->pml4e = pml4;
  curr_task->rip = (uint64_t*)&fun1;
  curr_task->rsp = (uint64_t*)page_alloc(0);
  //curr_task->stack = curr_task->rsp;
  curr_task->ntask = (struct task_struct *)kmalloc(sizeof(struct task_struct *));
  curr_task = curr_task->ntask;
  curr_task->rip = (uint64_t*)&fun2;
  curr_task->rsp = (uint64_t*)page_alloc(0);
  //curr_task->stack = curr_task->rsp;
  curr_task->ntask=head;
  curr_task = curr_task->ntask;
  
  readyQ[1] =(uint64_t )head;
  readyQ[2] =(uint64_t )head->ntask;*/
  //curr_task = (struct task_struct*)readyQ[1];
}

void contextSwitch(){
  //curr_task = ready;
  //struct task_struct *prev = (struct task_struct*)ready;
  //curr_task = prev;
  //curr_task = (struct task_struct*)curr_task->ntask;
  
  struct task_struct *prev = (struct task_struct*)readyQ[process_count-1];
  curr_task = (struct task_struct*)readyQ[process_count];
   //set_tss_rsp(&(curr_task->kstack[511]));
   /***saving previous rsp and rip***/
   __asm__ volatile(
   "movq %%rsp, %0;"
   "movq $1f, %1;"
   :"=r"(prev->rsp),"=r"(prev->rip)
   );

  /***Loading next processes address space***/
  __asm__ volatile("movq %0, %%cr3;"
    :
    :"a"(curr_task->cr3)
    );

  /***setting new process kernel stack***/
   
  
  /***loading next processes rsp***/
  __asm__ __volatile__ (
    //"movq $1f,%0;"
    "movq %0, %%rsp;"
    //"movq %1, %%cr3;"
    //"pushq %1;"
    //"iretq;"
    :
   // :"r"(curr_task->rsp), "r"(curr_task->rip), "r"(curr_task->cr3)
     :"r"(curr_task->rsp)
    :"memory"
   );
   set_tss_rsp(&(curr_task->kstack[511]));
}


void init_context_switch(){
  //struct task_struct *prev = (struct task_struct*)readyQ[1];
  //struct task_struct *next = (struct task_struct*)readyQ[1];
  
  /*__asm__ __volatile__ (
    "movq %%rsp, %0;"
    "movq $1f,%1;"
    :"=g"(prev->rsp),"=g"(prev->rip)
    :
    :"memory"
  );
  
  set_tss_rsp(curr_task->rsp);
  */
  
  __asm__ __volatile__ (
    //"movq $1f,%0;"
    "movq %0, %%rsp;"
    "pushq %1;"
    "retq;"
    "1:\t"
    :
    :"r"(curr_task->rsp), "r"(curr_task->rip)
    :"memory"
);
  kprintf("end switch\n");
}

/*
void mo_switch(){
  //set_tss_rsp(&(pcb->kstack[511]));
 __asm__ volatile(
 "movq %2, %%cr3;"
  "movq $0x23, %%rax;"
  "movq %%rax, %%ds;"
  "movq %%rax, %%es;"
  "movq %%rax, %%fs;"
  "movq %%rax, %%gs;"
  "movq %%rsp, %%rax;"
  "pushq $0x23;"
  "pushq %%rax;"
  "pushfq;"
  //popq %rax
  //orq $0x200, %rax
  //pushq %rax
  "pushq $0x2B;"
  //"movq %1, %%rsp;"
  //"pushq %0;"
  "push %%rdi"
  "iretq;"
  //:
  //:"r"(pcb->rip),"r"(pcb->rsp),"r"(pcb->cr3)
  //:"memory"
);
}
*/

void switch2(struct task_struct *prev, struct task_struct *next){
  
    //set_tss_rsp(next->rsp);
    
  __asm__ volatile(
    "movq %%rsp, %0;"
    "movq $1f,%1;"
    :"=g"(prev->rsp),"=g"(prev->rip)
    :
    :"memory"
  );
  
  __asm__ __volatile__ (
    "movq %0, %%rsp;"
    "pushq %1;"
    "retq;"
    "1:\t"
    :
    :"r"(next->rsp), "r"(next->rip)

);  
  
  /*__asm__ __volatile__ (
    "movq $1f,%0;"
    "movq %1, %%rsp;"
    "pushq %2;"
    "retq;"
    :"=g"(prev->rip)
    :"r"(next->rsp), "r"(next->rip)
);*/

  /*
    __asm__ volatile(
      //"movq %%rsp,%1;"
      //"movq $1f,%0;"
      "movq %0, %%rsp;"
      "pushq %1;"
      "retq;"
      "1:\t"
    //  :"=g"(prev->rip)
      :
      :"r"(next->rsp),"r"(next->rip)
    );
    */
  kprintf("Switched to new process\n");
}

/*
void schedule()
{
kprintf("\nSchedule start\n");
struct task_struct *prev = curr_task;
struct task_struct *next = curr_task->ntask;
curr_task = curr_task->ntask;
switch2(prev, next);
kprintf("\nSchedule ended\n");
}
*/

void schedule()
{
	//struct task_struct *prev = (struct task_struct*)readyQ[0];
  curr_task = (struct task_struct*)readyQ[1];
  // __asm__ volatile("movq %%rsp, %0;""movq $1f, %1;":"=r"(prev->rsp),"=r"(prev->rip));
  //curr_task = (struct task_struct*)readyQ[1];

  __asm__ __volatile__ (
    //"movq $1f,%0;"
    "movq %0, %%rsp;"
    "movq %1, %%cr3;"
    //"pushq %1;"
    //"retq;"
      :
   // :"r"(curr_task->rsp), "r"(curr_task->rip), "r"(curr_task->cr3)
     :"r"(curr_task->rsp), "r"(curr_task->cr3)
    :"memory"
);
 set_tss_rsp(&(curr_task->kstack[511])); 
 kprintf("end switch\n");
}

/*
void fun1()
{
kprintf("hello1");
kprintf("hello2");
kprintf("hello3");
kprintf("hello4");
kprintf("hello5");
kprintf("hello6");
kprintf("hello7");
//schedule();
kprintf("\nEnd of fun1()\n");
switch2User();
//while(1);
}*/

void fun2()
{
//kprintf("--world--");
//schedule();
//kprintf("\nEnd of fun2()\n");
/*struct task_struct *next = (struct task_struct *)readyQ[0];

  __asm__ volatile(
  "movq %0, %%rsp;"
  "movq %2, %%cr3;"
  "pushq %1;"
  "retq"
  "1:\t"
  :"=r"(next->rsp),"=r"(next->rip),"=r"(next->cr3)
  :
  :"memory"
  );
  */
}

/*
void switch2User(struct task_struct *curr_task){
  set_tss_rsp(curr_task->kstack);
  //pml4e_t pml4e = curr_task->pml4e & ~0xFFF;
  
  __asm__ volatile(
   "cli;"
   //"movq %0, %%cr3;"
   "mov $0x23, %%ax;"
   "mov %%ax, %%ds;"
   "mov %%ax, %%es;"
   "mov %%ax, %%fs;"
   "mov %%ax, %%gs;"
   //"movq %0, %%rax;"
   "pushq $0x23;"
   "pushq %%rax;"
   "pushfq;"
   "popq %%rax;"
   "orq $0x200, %%rax;"
   "pushq %%rax;"
   "pushq $0x1B;"
   "pushq $1f;"
   "movq $0x0, %%rdi;"
   "movq $0x0, %%rsi;"
   //"iretq;"
   "1:"
   ::
  // ::"r"(curr_task->rsp), "r"(curr_task->rip)
  );
  
  kprintf("User mode on\n");
}*/

struct task_struct *loadpcb(char *filename){
  struct task_struct *process = (struct task_struct *)kmalloc(sizeof(struct task_struct));
  readyQ[++process_count]=(uint64_t)process;
  struct Page *p = page_alloc(0);
  strcopy(process->pname, filename);
  
  //strcopy(process->trsh,t);
  __asm__ volatile("movq %%cr3, %0;":"=r"(prev_cr3)::"memory");
  
  //int len = strlength(filename);
  //char buf[len];
  //strcopy(buf,filename);
  //memcpy(buf,filename,strlength(filename));
  uint64_t *pml4 = (uint64_t*)get_kva(p);
  memset(pml4,0,PGSIZE);
  pml4[511] = boot_pml4e[511];
  process->pml4e = (uint64_t*)pml4;
  //memcpy(process->pml4e,boot_pml4e, PGSIZE);
  process->cr3 = PADDR(pml4);
  process->pid = ++nextpid;
  process->state = RUNNING;
  //process->pml4e[510] = process->cr3 | PTE_P | PTE_R;
  
  process->mm = (struct mm_struct*)((char*)(process+1));
  process->mm->mm_count = 0;
  process->mm->mmap = NULL;
  char str[20];
  memcpy(str, filename, strlength(filename));
  process->stack = userMalloc((uint64_t *)process->pml4e,512);
  //process->stack = (uint64_t*)kmalloc(1);
  
  __asm__ volatile("movq %0, %%cr3;"::"r"(process->cr3));
  //kprintf("process->cr3\n=%p",process->cr3);
  
  int pos=506;
  int f=1;
  for(;pos>490;pos--) process->kstack[pos] = f++;
  
  process->kstack[491] = (uint64_t)(&switch_process+28);
  process->rsp = &(process->kstack[490]); //something with this
  //process->rsp = &(process->kstack[506]); //something with this
  process->kstack[511] = 0x23;
  //process->kstack[511] = 0x10;
  process->kstack[510] = (uint64_t)(&process->stack[511]);
  //process->kstack[510] = (uint64_t)process->rsp;
  //process->kstack[509] = 0x246;
  process->kstack[509] = 0x20202;
  process->kstack[508] = 0x2b;
  //process->kstack[508] = 0x8;
  elf_file_load(process,str);
  //elf_file_load(process,buf);
  //kprintf("process rip is %p", process->rip);
  
  process->kstack[507] = (uint64_t)process->rip; 
  //process->kstack[507] = (uint64_t)&fun2;
  //process->rsp = (&process->kstack[506]);
  __asm__ volatile("movq %0, %%cr3;"::"r"(prev_cr3));
  //__asm__ volatile("movq %0, %%cr3;"::"r"(boot_cr3));
  //__asm__ volatile("movq %0, %%cr3;"::"r"(boot_cr3));
   __asm__ volatile("movq %0, %%cr3;"::"r"(prev_cr3));
  
  return process;
} 

struct vm_area_struct *alloc_vma(struct mm_struct *mm){
	struct vm_area_struct *last;
  struct vm_area_struct *temp;
	//char *temp;	

	if(mm->mmap == NULL){ //nothing in vma list
		//temp = (char*)kmalloc(1);
   
    temp = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
	  //last = (struct vm_area_struct*)temp;
    last = (struct vm_area_struct*)temp;
		mm->mmap = last;
		mm->mm_count += 1;
		return (struct vm_area_struct *)temp; 
	}else{ 
	//iterate till the last element in vmalist
	last = mm->mmap;
	while(last->vm_next != NULL){
		last = last->vm_next;
	}
	//temp = (char*)last + sizeof(struct vm_area_struct);
	//last->vm_next = (struct vm_area_struct*)temp;
  temp = (struct vm_area_struct*)kmalloc(sizeof(struct vm_area_struct));
  last->vm_next = (struct vm_area_struct*)temp;
	mm->mm_count +=1;
	return (struct vm_area_struct*)temp;
	}
}

void *get_kva(struct Page *p){ //get_kva and page2kva is same
  uint64_t paddr = page2ppn(p) << PGSHIFT;
  uint64_t kaddr = paddr+KERNBASE;
  return (void*)kaddr;
}




uint64_t getpid(){
  //kprintf("pid is %d",curr_task->pid);
  return curr_task->pid;
}

/* ########## Fork Implementation ########### */

//struct task_struct* dup_task_struct(struct task_struct *parent){
  
void dup_task_struct(struct task_struct *parent){  
  //uint64_t p_rsp;
  struct task_struct *child = (struct task_struct *)kmalloc(sizeof(struct task_struct));
  //process_count++;
  char f[40] = "fork";
  child->pid = ++nextpid;
  strcopy(child->pname,f);
  ret_pid = child->pid;
  child->ppid = parent->pid;
  readyQ[++process_count]=(uint64_t)child;
  struct Page *p = page_alloc(0);
  uint64_t *pml4 = (uint64_t*)get_kva(p);
  //memset(pml4,0,PGSIZE);
  /*for(int i=0;i<512;i++){
    pml4[i] = parent->pml4e[i];
  }*/
  pml4[511] = boot_pml4e[511];
  child->pml4e = (uint64_t*)pml4;
  child->cr3 = PADDR(pml4);
  //memcpy(child->pml4e,parent->pml4e,512);
  
  child->mm = (struct mm_struct *)((char*)(child+1));
  child->mm->mm_count = 0;
  child->mm->mmap = NULL;
  
  /*struct Page *p1 = page_alloc(0);
  uint64_t *temp = (uint64_t*)get_kva(p1);
  memcpy(temp,parent->stack,PGSIZE);*/
  
  struct Page *p1 = page_alloc(0);
  uint64_t *tmpstack = (uint64_t*)get_kva(p1);
  for(int i=0;i<512;i++){
    tmpstack[i] = parent->stack[i];
  }
  //child->stack = (uint64_t*)userMalloc((uint64_t *)child->pml4e,512);
  //child->stack = parent->stack;
  //boot_map_region(child->pml4e,(uint64_t)child->stack,4096,PADDR(tmpstack),0x7);
  //parent->stack[511]=16;
  //child->stack = (uint64_t*)userMalloc((uint64_t *)child->pml4e,512);
    //boot_map_region(child->pml4e,(uint64_t)child->stack,4096,PADDR(tmpstack),0x7);
    //parent->stack[511]=16;
    
  __asm__ volatile("mov %0, %%cr3;"::"r"(child->cr3));
  for(int i=0;i<512;i++){
    child->pml4e[i] = parent->pml4e[i];
  }
  //child->stack = parent->stack;
  //child->stack = (uint64_t*)kmalloc(512);
    child->stack = (uint64_t*)userMalloc((uint64_t *)child->pml4e,512);
    for(int i=0;i<512;i++){
    child->stack[i] = parent->stack[i];
  }
    //boot_map_region(child->pml4e,(uint64_t)child->stack,4096,PADDR(tmpstack),0x7);
  //boot_map_region(child->pml4e,child->stack,4096,PADDR(temp),PTE_R | PTE_P |PTE_U);
  //mapPage(child,(void*)child->stack,4096);
  //memcpy((char*)child->stack, (void*)PADDR(tmpstack), 4096);
  
  //boot_map_region(child->pml4e,(uint64_t)child->stack,4096,PADDR(tmpstack),0x7);
  uint64_t diff = (uint64_t)((parent->kstack[509]) - (uint64_t)(parent->stack))/8;
  //start copying kstack of parent process
  child->kstack[491] = (uint64_t)(&switch_process+28);
  child->kstack[490]= 16;
  child->kstack[511] = 0x23;
  //child->stack[492] =(uint64_t) *(parent->kstack[509]);
  //void *sum = (child->stack + diff);
  child->kstack[510] = (uint64_t)(&(child->stack[diff]));
      //child->kstack[510] = (parent->kstack[509]);
  //child->kstack[509] = 0x246;
  child->kstack[509] = 0x200286;
  child->kstack[508] = 0x2B;
  child->kstack[506] = 0; //rax
  child->kstack[507] = parent->kstack[506];   
  for(int x = 505; x>491; x--)
  {
    child->kstack[x]=parent->kstack[x-2];
  }
  //child->kstack[491] = (uint64_t)(&switch_process+28);
  //child->kstack[490] = rsp
  child->rsp = &(child->kstack[490]);
   //copy parent's vma
   /*struct vm_area_struct *parent_vma = parent->mm->mmap;
   struct vm_area_struct *child_vma;
   while(parent_vma != NULL){
     child_vma = (struct vm_area_struct *)alloc_vma(child->mm);
     child_vma->vm_start = parent_vma->vm_start;
     child_vma->vm_end = parent_vma->vm_end;
     child_vma->vm_file = parent_vma->vm_file;
     child_vma->vm_offset = parent_vma->vm_offset;
     child_vma->vm_memsz = parent_vma->vm_memsz;
     child_vma->vm_flags = parent_vma->vm_flags;
     //map this vma
     mapPage(child,(void*)parent_vma->vm_start,parent_vma->vm_memsz);
     
        parent_vma = parent_vma->vm_next;
   }*/ 
   
  /* __asm__ volatile(
     "movq %%rsp, %1;"
     "movq $2f,%0;"
     "2:\t"
     :"=r"(child->rip),"=r"(p_rsp)
     :
     :"memory"
   ); */
   
   __asm__ volatile("mov %0, %%cr3;"::"r"(parent->cr3));
   //parent->stack[5]=16;
   //contextSwitch();
   //return child;
}



uint64_t myfork(){
  struct task_struct *parent = curr_task;
  dup_task_struct(parent);
  //kprintf("Child process pid is %d\n",child->pid);
  return ret_pid;
  //now here do something with child process..like loading binary?
}
/*
char *stdin_read(char *str){
  __asm__ volatile("sti;");
  //inputbuf[14] = "This is \nlife"; 
  //char temp[14] = "This is \nlife";
  //int i=0;
  //inputbuf[inputlen++] = 'a';
  //inputbuf[inputlen++] = '\n';
  //newline++;
  kprintf("%d\n",newline);
  int line = 0;
  while(line == 0){
  line = newline;
  //kprintf("in while");
  //if (newline==1){
  //kprintf("%d  ",line);
  if (line>0)
  break;
  //break;
  
  //}
  }
  
  kprintf("out of while loop\n");
  str = "hello\n";
  
  while(inputbuf[i] != '\n'){
    str[i] = temp[i];
    i++;
  }
  int count = strlength(str);
  if(count < 10) inputlen = count;
  //memcpy((void*)str,(void*)inputbuf,inputlen);
  while(inputlen >=0){
  //inputbuf[inputlen] = '\0';
  temp[i] = '\0';
  inputlen --;
  }
  inputlen =0;
  return str;
}
*/
void exit_process()
{
  /*//load kernel rsp
  struct task_struct *kprocess = ready;
  __asm__ volatile("movq %0, %%rsp;"::"r"(kprocess->rsp));
  
  //load kernel cr3
  __asm__ volatile("movq %0, %%cr3"::"r"(boot_cr3));
  
  int i=1;
  while(i<=process_count){
    if(readyQ[i] == (uint64_t)curr_task)
    break;
    i++;
  }
  if(i<process_count)
  {
  for(int a=i;a+1<=process_count;a++){
    //while((i+1)<=process_count){
    readyQ[a] = readyQ[a+1];
    }
  }
  process_count--;*/
  
  curr_task = (struct task_struct*)readyQ[process_count-1];
  process_count--;
  //struct task_struct *prev = (struct task_struct*)readyQ[process_count];
   //set_tss_rsp(&(curr_task->kstack[511]));
   /***saving previous rsp and rip***/
   /*__asm__ volatile(
   "movq %%rsp, %0;"
   "movq $1f, %1;"
   :"=r"(prev->rsp),"=r"(prev->rip)
   );*/
  /***Loading next processes address space***/
  __asm__ volatile("movq %0, %%cr3;"
    :
    :"a"(curr_task->cr3)
    );

  /***setting new process kernel stack***/
  set_tss_rsp(&(curr_task->kstack[511])); 
  
  /***loading next processes rsp***/
  __asm__ __volatile__ (
    //"movq $1f,%0;"
    "movq %0, %%rsp;"
    //"movq %1, %%cr3;"
    //"pushq %1;"
    //"iretq;"
    :
   // :"r"(curr_task->rsp), "r"(curr_task->rip), "r"(curr_task->cr3)
     :"r"(curr_task->rsp)
    :"memory"
   );
  
  
}


/* ############### Execve Implementation ################ */

void execve(char *filename, char *argv[], char *envp[]){
  
 /* char *tmp_pathname = (char *)filename;
    char **tmp_argument = (char **)argv;
    //char **tmp_env = (char **)envp;
    int i=0;
    //int j=0;
    char pathname[1024];
    strcopy(pathname,tmp_pathname);
 
    char **temp1=tmp_argument;
    while(i<3){
        kprintf("arg%d is %s\n",i,temp1[i]);
        i++;
    }
 */
    
  
  struct task_struct *pcb = loadpcb(filename);
  //pcb->arg = temp1[0];
  kprintf("%s",pcb->trash);
  //kprintf("Running Process....%s\n",pcb->pname);
  contextSwitch();
  //exit_process(0);
}


/* ##### process list ##### */

void list_process(){
  struct task_struct *pcb;
  for(int i=0;i<=process_count;i++){
    pcb = (struct task_struct *)readyQ[i];
    kprintf("PID: %d   |   NAME: %s\n", pcb->pid, pcb->pname);
  }
}

struct vm_area_struct *find_vma(struct mm_struct *mm, uint64_t pata){
  struct vm_area_struct *vmlist = (struct vm_area_struct*)mm->mmap;
  struct vm_area_struct *vma=NULL;
  while(vmlist){
    if(vmlist && vmlist->vm_start<=pata && vmlist->vm_end>pata){
      vma = vmlist;
      break;
    }
    vmlist = vmlist->vm_next;
  }
  
  return vma;
}

void list_files(){
  if(strcomp(cwd, "")!=0){
    uint64_t start = tarfs_open_dir(cwd);
    tarfs_read_dir(start);
  }else{
    int i=0;
    tarfs_obj entry;
    while(strlength(tfs[i].name)!=0){
    memset(&entry,0,sizeof(entry));
    entry = tfs[i];
    if(entry.pindex == -1){
      kprintf("%s    ",entry.name);
    }
  i++;
  }
  kprintf("\n");
  }
}

void read_file(char *filename){
  //filename = curr_task->arg;
  if (filename!=NULL){
  char s[50];
  strcopy(s,cwd);
  str_cat(s,filename);
  
  tarfs_obj *entry = tarfs_file_open(s);
  if (entry!=NULL)
  {
  char buf[1024];
  tarfs_file_read(entry,sizeof(buf),(uint64_t)buf);}}
}
/*
void sleep_r(uint64_t t){
  
}
*/


void chdir(char *dir_name){
	if(dir_name == '\0'){
		//cwd = "/";
    strcopy(cwd,""); //for root directory
		kprintf("Root directory\n");
	}
  /*else if(strcomp(dir_name,"..")==0){
     char tmp[50];
     strcopy(tmp,cwd);
     char *last = str_token(tmp, "/");
     if (last != NULL) {
      kprintf("Last token: '%s'\n", last+1);
    }
    str_cat(last,"/");
    char *s = str_token(cwd,last);
    strcopy(cwd,s);
  }*/
  else{
		//search for dir_name in tfs entries
   char s[50];
   strcopy(s,cwd);
   str_cat(s,dir_name);
   //kprintf("%s",s);
		uint64_t addr = tarfs_open_dir(s);
		if(addr != 0){
			str_cat(cwd,dir_name);
			//str_cat(cwd,"/");
      //kprintf("%s", cwd);
		}
	}
}

void PWD(){

  if(strcomp(cwd, "")==0){
    kprintf("rootfs/");
  }
	kprintf("%s\n",cwd);
}

int sys_write(uint64_t fd, uint64_t addr, int len){
	if(fd == stdout || fd == stderr){
		kprintf((char*)addr);
   len = strlength((char*)addr);
		return len;
	}
return 0;
}



/*
void list_file(){
  if(pwd == "/"){ //root directory list all elements of tfs having pindex =-1
    list_tarfs_dir();
  }
 uint64_t start = tarfs_open_dir("lib/");
 tarfs_read_dir(start);
}*/