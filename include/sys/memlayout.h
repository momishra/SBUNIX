#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H

#include <sys/defs.h>
#include <sys/mmu.h>

#define KERNBASE 0xffffffff80000000
#define IOPHYSMEM 0x09fc00  //starting 640KB thing
#define EXTPHYSMEM 0x100000 //1MB end


//#define USER_STACK_ADDR 0xffffffff80000000 - 0x1000

typedef uint64_t pml4e_t; //entry in pml4
typedef uint64_t pdpe_t; //entry in pdp
typedef uint64_t pte_t; //entry in page table
typedef uint64_t pde_t; //entry in page directory

struct Page {
struct Page *pp_link; //next page on free list
uint64_t pp_ref;
};

#endif

