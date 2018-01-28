#ifndef __PMAP__H
#define __PMAP__H
#include<sys/memlayout.h>
#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/mmu.h>
#include<sys/sched.h>

struct Env;
extern struct Page * page;
extern size_t npages;

enum{
ALLOC_ZERO = 1<<0,
};

uint64_t boot_cr3;
uint64_t *boot_pml4e;
//extern uint64_t *boot_pml4e;

extern uint64_t bar5;
#define PAGE_TABLE_ALIGNMENT 0x1000;

#define PADDR(kva) \
({\
uint64_t __m_kva = (uint64_t)(kva); \
if (__m_kva < KERNBASE) \
kprintf("PADDR called with invalid kva %x", __m_kva); \
__m_kva - KERNBASE; \
})

#define KADDR(pa) \
({ \
uint64_t __m_pa = (pa);\
uint32_t __m_ppn = PAGENO(__m_pa);\
if (__m_ppn >= npages)\
kprintf("KADDR called with invalid pa");\
(void*)((uint64_t)(__m_pa + KERNBASE));\
})

static inline uint64_t page2ppn(struct Page *pp)
{
return pp - page;
}

static inline uint64_t page2pa(struct Page* pp)
{
return page2ppn(pp) << PGSHIFT;
}

static inline struct Page* pa2page(uint64_t pa)
{
if (PAGENO(pa) >= npages)
kprintf("pa2page called with invalid pa");
return &page[PAGENO(pa)];
}

static inline void *page2kva(struct Page *pp)
{
return KADDR(page2pa(pp));
}

void mem_initialise(uint32_t* modulep, void* physbase, void* physfree);

void mapPage(struct task_struct *pcb, void *virtaddr,uint32_t size);
void *boot_alloc(uint64_t n);
void page_initialise(void);
struct Page* page_alloc(int alloc_flags);
void page_free(struct Page *page);
uint64_t* kmalloc(uint64_t size);
void page_decref(struct Page* page);
void invalidateTLB(pml4e_t *pml4e,void *vaddr);
void removePage(pml4e_t *pml4e,void *vaddr);
int insertPage(pml4e_t *pml4e, struct Page *page, void *vaddr, int pflag);
uint64_t* userMalloc(pml4e_t *pml4e, uint64_t size);
struct Page* searchPage(pml4e_t *pml4e, void *vaddr,pte_t **rpte);
pte_t *pml4e_walk(pml4e_t *pml4e, const void *va, int create);
pte_t *pgdir_walk(pde_t *pgdir, const void *va, int craete);
pte_t *pdpe_walk(pdpe_t *pdpe, const void *va, int create);
pte_t *pde_walk(pde_t *pgdir, const void *va, int create);
void boot_map_region(pml4e_t *pml4e, uint64_t la, size_t size, uint64_t pa, int perm);
#endif
