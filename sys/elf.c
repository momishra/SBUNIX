#include <sys/defs.h>
#include<sys/elf64.h>
#include<sys/tarfs.h>
#include<sys/kprintf.h>
#include<sys/str.h>
#include<sys/sched.h>
#include<sys/mmu.h>
#include<sys/pmap.h>
#include<sys/memlayout.h>
#include<sys/types.h>

//#define USER_STACK_ADDR 0xFFFFF0F080700000
#define USER_STACK_ADDR 0x200000
#define USER_HEAP_ADDR 0x100000
#define USER_STACK_SIZE 0x1000

void* find_file(char *filename){
    int i,j;
    uint64_t fsize;
    struct posix_header_ustar *t = (struct posix_header_ustar *)&_binary_tarfs_start;

    if(filename == NULL) return NULL;
    
     while(strlength(t->name)!= 0){
    // kprintf("File type %s", tfs->
       if(strcomp(t->name,filename)!=0){
            j=0;
            fsize=0;
            for(i=10;i>=0;--i)
                fsize = fsize + (t->size[i]-48) * pow(8,j++); //file size in bytes
            if(fsize%512!=0){
                fsize=(fsize/512)*512;
                fsize+=512;   
                }
            t = (struct posix_header_ustar *)((uint64_t)t + fsize + sizeof(struct posix_header_ustar));   
        }
        else{
         return (void *)t;
     }    
    }
    return NULL;
}


/*uint64_t find_file(char *filename){
    struct posix_header_ustar *tfs = (struct posix_header_ustar *)&_binary_tarfs_start;
    int i=1, temp = 512;
    uint64_t size;
    
    while(strlength(tfs->name)!= 0){
      tfs = (struct posix_header_ustar *)(&_binary_tarfs_start+temp);
      size = oct_dec(atoi(tfs->size));
      if(strlength(tfs->name) == 0){
        return 999;
      }
      if(strcomp(tfs->name, filename) >= 0){
        return temp+512;
      }
      if(size == 0){
        temp = temp + 512;
      }else{
        temp += (size%512==0) ? size + 512 : size + (512 - size%512) + 512;
      }
    i+=1;
    }
  return 0;
}
*/

struct task_struct* elf_file_load(struct task_struct *process, char *filename){
  //first check the validity of elf file
  //__asm__ volatile("movq %0, %%cr3;"::"r"(process->cr3));
	Elf64_Ehdr *elfhdr;
	Elf64_Phdr *phdr;
	int i;
 
   //uint64_t offset = find_file(filename);
	struct posix_header_ustar *file = (struct posix_header_ustar*)find_file(filename);
 
  //elfhdr = (Elf64_Ehdr*)(&_binary_tarfs_start + offset);
	elfhdr = (Elf64_Ehdr*)(file + 1); //contains entry point info
  Elf64_Half numSegments = elfhdr->e_phnum;   
 
  phdr = (Elf64_Phdr*)((uint64_t)elfhdr + elfhdr->e_phoff);
  //uint64_t Pheader_table = (uint64_t)elfhdr + elfhdr->e_phoff;
  //Elf64_Phdr* phdr = (Elf64_Phdr*)((uint8_t*) elfhdr+ elfhdr->e_phoff);
  //Elf64_Phdr* ephdr = phdr+elfhdr->e_phnum;
  process->rip = elfhdr->e_entry;  
  //uint64_t size = elfhdr->e_phentsize;
  uint64_t prev_cr3;
  __asm__ volatile("movq %%cr3, %0;"
                    :"=r"(prev_cr3)
                    :
                    :"memory"
                    );
  
for(i=0;i<numSegments;++i){
    if(phdr->p_type == PT_LOAD){
      
      __asm__ volatile("movq %0, %%cr3;"::"r"(process->cr3));
       mapPage(process,(void*)phdr->p_vaddr,phdr->p_memsz);
       memcpy((char*)phdr->p_vaddr, (void*) elfhdr + phdr->p_offset, phdr->p_memsz);  
       if(phdr->p_filesz < phdr->p_memsz){
        memset((char*)phdr->p_vaddr+phdr->p_filesz,0,phdr->p_memsz - phdr->p_filesz);
      }      
      struct vm_area_struct *vma = (struct vm_area_struct*)alloc_vma(process->mm);
      vma->vm_mm = process->mm;
      vma->vm_flags = phdr->p_flags;
      vma->vm_start = phdr->p_vaddr; //virtual address at which first segment resides
      vma->vm_end = phdr->p_vaddr + phdr->p_memsz;
      vma->vm_next = NULL;
      vma->vm_memsz = phdr->p_memsz;
      //vma->vm_file = (uint64_t)elfhdr;
      vma->vm_offset = phdr->p_offset;
      //__asm__ volatile("movq %0, %%cr3;"::"r"(boot_cr3));      
    }
  phdr++;
  }
  
  //allocate stack
  struct vm_area_struct *stackvma = (struct vm_area_struct*)alloc_vma(process->mm);
  stackvma->vm_mm = process->mm;
  stackvma->vm_start = USER_STACK_ADDR - PGSIZE;
  stackvma->vm_end = USER_STACK_ADDR;
  stackvma->vm_flags = (PF_U+PF_W); 
  stackvma->vm_next = NULL;
  //__asm__ volatile("movq %0, %%cr3;"::"r"(process->cr3));
  mapPage(process,(void*)stackvma->vm_start,PGSIZE);
  __asm__ volatile("movq %0, %%cr3;"::"r"(prev_cr3));
  //process->kstack[510] = stackvma->vm_end; //user stack address
  
  
  //allocate heap
   struct vm_area_struct *heapvma = (struct vm_area_struct*)alloc_vma(process->mm);
  heapvma->vm_mm = process->mm;
  heapvma->vm_start = USER_HEAP_ADDR;
  stackvma->vm_end = USER_HEAP_ADDR;
  //stackvma->vm_flags = (PF_R+PF_W); 
  stackvma->vm_next = NULL;
  return process;
}


/*
struct task_struct* elf_file_load(struct task_struct *process, char *filename){
  //first check the validity of elf file
	Elf64_Ehdr *elfhdr;
	Elf64_Phdr *phdr;
	int i;
 
   //uint64_t offset = find_file(filename);
	struct posix_header_ustar *file = (struct posix_header_ustar*)find_file(filename);
 
  //elfhdr = (Elf64_Ehdr*)(&_binary_tarfs_start + offset);
	elfhdr = (Elf64_Ehdr*)(file + 1); //contains entry point info
  Elf64_Half numSegments = elfhdr->e_phnum;   
 
 // process->rip = (uint64_t*)elfhdr->e_entry;
  phdr = (Elf64_Phdr*)((uint64_t)elfhdr + elfhdr->e_phoff);
  
  //iterate to all the segments and check if they are to be loaded
  
for(i=0;i<numSegments;++i){
    if(phdr->p_type == PT_LOAD){
    // Note that p_filesz must always be
		// <= p_memsz so it is only necessary to check p_memsz.
      //load the content in here
      __asm__ volatile("movq %0, %%cr3;"::"r"(process->cr3));
       mapPage(process,(void*)phdr->p_vaddr,phdr->p_memsz);
       memcpy((char*)phdr->p_vaddr, (void*) elfhdr + phdr->p_offset, phdr->p_filesz);
       //memcpy((char*)phdr->p_vaddr, (void*) elfhdr + phdr->p_offset, phdr->p_memsz);      
      if(phdr->p_filesz < phdr->p_memsz){
        memset((char*)phdr->p_vaddr+phdr->p_filesz,0,phdr->p_memsz - phdr->p_filesz);
      }
      
      struct vm_area_struct *vma = (struct vm_area_struct*)alloc_vma(process->mm);
      vma->vm_mm = process->mm;
      vma->vm_flags = phdr->p_flags;
      vma->vm_start = phdr->p_vaddr; //virtual address at which first segment resides
      vma->vm_end = phdr->p_vaddr + phdr->p_memsz;
      vma->vm_next = NULL;
      //vma->vm_file = (uint64_t)elfhdr;
      vma->vm_offset = phdr->p_offset;
      }
  phdr++;
  }
  
  process->rip = elfhdr->e_entry;  
    
  //after loading all segments in memory
  struct vm_area_struct *stackvma = (struct vm_area_struct*)alloc_vma(process->mm);
  stackvma->vm_mm = process->mm;
  stackvma->vm_start = USER_STACK_ADDR - PGSIZE;
  stackvma->vm_end = USER_STACK_ADDR;
  stackvma->vm_flags = (PF_R+PF_W); 
  process->rsp = (uint64_t*)USER_STACK_ADDR;
  
  return process;
}
*/