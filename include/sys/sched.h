#ifndef __SCHED_H
#define __SCHED_H

#include<sys/defs.h>
#include<sys/isr.h>
#include<sys/memlayout.h>
#define stdout 1
#define stderr 2
#define STACK_SIZE 4096-64

enum state{
RUNNING=0, SLEEPING, ZOMBIE 
};

struct file{
  uint64_t file_start;
  uint64_t vm_pgoff;
  uint64_t vm_sz;
};

struct vm_area_struct{
  unsigned long vm_start; //start address within vm_mm
  unsigned long vm_end; //first byte after end address within vm_mm
  struct vm_area_struct *vm_next,*vm_prev;
  struct mm_struct *vm_mm;
  unsigned long vm_flags; //protection bits
  uint64_t vm_memsz;
  uint64_t vm_offset; //offset within vm_file in PGSIZE unit
  struct file *vm_file;
};


struct mm_struct{ //program's memory
  struct vm_area_struct *mmap; //list of vmas
  int mm_count; //number of references to mm_struct
  uint64_t* pte;
  uint64_t* pgd;
  unsigned long start_code, end_code, start_data, end_data;
  unsigned long start_brk, brk, start_stack;
  unsigned long arg_start, arg_end, env_start, env_end;
  unsigned long def_flags;
  unsigned long total_vm; //total paes mapped
  unsigned long locked_vm;
};

struct task_struct{ //process descriptor
	char pname[50];
 //char *arg;
 char trash[50];
	volatile long state;
	uint64_t *stack; //stack of the process
	uint64_t kstack[512]; //kernel stack 
	int on_cpu; //timestamp
	uint64_t *bptr; //address from which vm of the process starts
	struct mm_struct *mm; //memory descriptor
	pid_t pid;
	pid_t ppid;
  //char *pname;
	int exit_status; //what we need to do with this
  uint64_t sleeptime;
	uint64_t *pml4e;
	uint64_t cr3;
	uint64_t *rsp;
	uint64_t rip;
	struct task_struct *ntask; //next task	
  struct task_struct *child_task; //next task
  struct task_struct *sib_task; //next task
};

uint64_t prev_cr3;
struct task_struct *prev_task;
struct task_struct *ready;
struct task_struct *curr_task;
uint64_t nextpid;
uint64_t num_ready_list;
uint64_t process_count;
uint64_t readyQ[100];
uint64_t sleepQ[100];
void mo_switch(); //crap for test
//char cwd[1024] = "/";
/****FOr system calls -START****/
uint64_t getpid();
void execve(char *filename, char *argv[], char *envp[]);
void exit_process();
uint64_t myfork();
void dup_task_struct(struct task_struct *parent);
void list_process();
void list_file();
void chdir(char *dir_name);
void PWD();
void SLEEP(uint64_t time);
/****FOr system calls -END****/
extern void pusha();
extern void popa();
extern void switch_process();
void switch_to_user_mode(struct task_struct* pcb);
struct vm_area_struct *find_vma(struct mm_struct *mm, uint64_t pata);
void contextSwitch();
struct task_struct *loadpcb(char *filename);
void schedule();
struct vm_area_struct *alloc_vma(struct mm_struct *mm);
struct vm_area_struct *find_vma(struct mm_struct *mm, uint64_t pata);
void initProcess();
extern void switch2User(uint64_t );
void switch2(struct task_struct *curr,struct task_struct *next);
void fun1();
void fun2();
void fun3();
void dummy();
void init_context_switch();
void *get_kva(struct Page *p);
struct task_struct *load(char *filename);
void contextSwitch2();
char *stdin_read(char *str);
void list_files();
void read_file(char *filename);
int sys_write(uint64_t fd, uint64_t addr, int len);
//char pwd[100];
#endif
