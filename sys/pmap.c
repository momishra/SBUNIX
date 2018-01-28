#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/memlayout.h>
#include<sys/pmap.h>
#include<sys/types.h>

#define MEM_EXCEED 4; //error code to be put in error.h

static char *nextfree; //virtual address of next byte of free memory
uint64_t kern_off;
size_t npages;

uint64_t boot_cr3;
size_t npages_basemem;
size_t npages_extmem;

struct Page *page; //physical page state array
static struct Page *page_free_list;

uint64_t *pte;

void mem_initialise(uint32_t* modulep, void* physbase, void* physfree)
{
  kern_off = KERNBASE+(uint64_t)physfree;
  //kprintf("KERNBASE : %p and physfree %p and physbase : %p\n",KERNBASE, physfree, physbase);
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
        if (smap->base==0)
        {
                npages_basemem=smap->length/4096;
                //kprintf("Lower Memory Pages = %d\n",npages_basemem);
        }
        else
        {
                npages_extmem=((smap->base+smap->length) - (uint64_t)physfree)/4096;
                //kprintf("Higher Memory Pages = %d\n",npages_extmem);
        }
      //kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
    }
  }
  npages=npages_basemem+npages_extmem;
  //kprintf("Available Physical Pages [%d]\n", npages);

  pml4e_t *pml4e;

  pml4e = boot_alloc(PGSIZE);
  //kprintf("pml4e: %p\n",pml4e);
  memset(pml4e,0,PGSIZE);

  boot_pml4e = pml4e;
  boot_cr3 = PADDR(pml4e);
  //kprintf("Boot_cr3 %p\n",boot_cr3);

  page = (struct Page*)boot_alloc(npages*sizeof(struct Page));

  page_initialise();

  //uint64_t size = CEIL((uint64_t)(physfree) + npages, 4096);
  //kprintf("size: %p\n",size);
  boot_map_region(pml4e,KERNBASE+(uint64_t)physbase, 0x7ffe000, (uint64_t)physbase,PTE_W|PTE_P|PTE_U);
  boot_map_region( pml4e,KERNBASE+(uint64_t)0xb8000, 4096*3, (uint64_t)0xb8000,PTE_W|PTE_P|PTE_U);
   __asm__ volatile("mov %0, %%cr3":: "b"(boot_cr3)); 
uint64_t cr0;
	__asm__ volatile("mov %%cr0, %0":"=r"(cr0));
	cr0 |= 0x80000000;
  __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

void *boot_alloc(uint64_t n){
  char *result=0;
  if(!nextfree){
    nextfree = CEIL((char*)kern_off, PGSIZE);
    result = nextfree;
    nextfree = nextfree+n;
  }
  else{
    if(n==0){
      nextfree = CEIL((char*)nextfree, PGSIZE);
      //PADDR(nextfree);
      result = nextfree;
    }else{
        if(n>0){
          nextfree = CEIL((char*)nextfree, PGSIZE);
          result = nextfree;
          nextfree+=n;
          //PADDR(nextfree);
        }
      }
    }
  return result;
}


void mapPage(struct task_struct *pcb, void *virtaddr,uint32_t size){
  
    void *startaddr = FLOOR(virtaddr, PGSIZE);
    void *endaddr = CEIL((char*)virtaddr+size, PGSIZE);
    struct Page *p;
    char *i;
    
    for(i = (char*)startaddr; i < (char*)endaddr; i +=PGSIZE){
      if(!(p = page_alloc(ALLOC_ZERO))){
        kprintf("No memory\n");
        //MEM_EXCEED;
      }else{
        if(insertPage((uint64_t*)pcb->pml4e, p, i, 0x7)!=0){
          kprintf("No memory\n");
        }
      }
    } 
}

/*
uint64_t *alloc_free_page(){
  uint64_t *result = 0;
  if(!nextfree){
    nextfree = CEIL((char*)kern_off,PGSIZE);
  }
  
  result = nextfree;
  uint64_t *paddr = get_page(NULL);
  // call insertPage function -- which is inserting the entry in page table?
  nextfree = CEIL((void*)nextfree+PGSIZE,PGSIZE);
  return result;
}

uint64_t *get_page(uint64_t *paddr){
  uint64_t *page;
  if(page_free_list){
        page = page_free_list;
        page_free_list = page_free_list->pp_link;
        return PADDR(page);
  }else{
  kprintf("page free list empty\n")
    return NULL;
  }
}
*/

void page_initialise(void){
  size_t i;
  uint64_t p;
  for (i=0;i<npages;i++)
  {
    if (i==0){
      page[i].pp_ref = 1;
      page[i].pp_link = NULL;
    }
    else if (i<npages_basemem){
      page[i].pp_ref = 0;
      page[i].pp_link = page_free_list;
      page_free_list = &page[i];
  }
    else if((i<=(EXTPHYSMEM/PGSIZE) || (i<(((uint64_t)boot_alloc(0)- KERNBASE)>>PGSHIFT)))){
      page[i].pp_ref++;
      page[i].pp_link = NULL;
  }
    else{
      page[i].pp_ref = 0;
      page[i].pp_link = page_free_list;
      page_free_list = &page[i];
    }
    p = page2pa(&page[i]);
  if ((p ==0 || p == IOPHYSMEM) && (page[i].pp_ref == 0))
  kprintf("page error: i %d\n",i);
}
}

struct Page* page_alloc(int alloc_flag)
{
struct Page *p = NULL;
if(page_free_list)
{
p = page_free_list;
page_free_list = page_free_list->pp_link;
if(alloc_flag && ALLOC_ZERO)
memset(page2kva(p),0,PGSIZE);
}
else
return NULL;
return p;
}

void page_free(struct Page *p)
{
if(p->pp_ref!=0)
kprintf("page->pp_ref is not zero");
else if(p->pp_link)
kprintf("page_>pp_link is not null");
else
{
p->pp_link= page_free_list;
page_free_list = p;
}
}

void page_decref(struct Page *p)
{
p->pp_ref--;
if(p->pp_ref==0)
page_free(p);
}

/*
pte_t *pml4e_walk(pml4e_t *pml4e, const	void *va, int create){
  uint64_t pml4_index = PML4(va); 
  pdpe_t *pdpe;
  pte_t *pte;
  
  struct Page *p = NULL;
  
  if(!pml4e[pml4_index] & PTE_P){
    if(!create){
      return NULL; //no entry created
    }else{
      p = page_alloc(ALLOC_ZERO);
      if(p != NULL){
        *(pml4e+pml4_index) = (pml4e_t)(page2pa(p)) | PTE_P | PTE_W | PTE_U;
        p->pp_ref++;
      }else{
        return NULL;
      }
    } 
  }
  
  pdpe = KADDR(ADDR_FROM_LA(pml4e[pml4_index]));
  pte = pdpe_walk(pdpe,va,create);
  if(pte!=NULL){
    return pte;
  }else{
    if(p!=NULL){
      pml4e[pml4_index] = 0;
      page_decref(p);
    }
    return NULL;
  }
}

pte_t *pdpe_walk(pdpe_t *pdpe, const void *va, int create){
  pde_t *pde;
  pte_t *pte;
  struct Page *p = NULL;
  uint64_t pdpe_index = PDPE(va);
  if((!pdpe[pdpe_index]) & PTE_P){
    if(!create){
      return NULL;
    }else{
      p = page_alloc(ALLOC_ZERO);
      if(p!=NULL){
        *(pdpe+pdpe_index) = (pdpe_t)(page2pa(p)) | PTE_P | PTE_W | PTE_U;
        p->pp_ref++;
      }else{
        return NULL;
      }
    }
  }
   pde = KADDR(ADDR_FROM_LA(pdpe[pdpe_index]));
   pte = pde_walk(pde,va,create);
   if(pte!=NULL){
     return pte;
    }else{
      if(p!=NULL){
        pdpe[pdpe_index] = 0;
        page_decref(p);
      }
      return NULL;
  }
}

pte_t *pde_walk(pde_t *pde, const void *va, int create){
  pte_t *pte;
  struct Page *p = NULL;
  uint64_t pde_index = PDE(va);
  if((!pde[pde_index]) & PTE_P){
    if(!create){
      return NULL;
    }else{
      p = page_alloc(ALLOC_ZERO);
      if(p!=NULL){
        pde[pde_index] = (pde_t)(page2pa(p)) | PTE_P | PTE_W | PTE_U;
        if(pde_index >= PDE(KERNBASE)){
          pde[pde_index] = (pde_t)(page2pa(p)) | PTE_P | PTE_W | PTE_U;
        }
        p->pp_ref++;
      }else{
        return NULL;
      }
    }
  }
  uint64_t temp = *(pde+pde_index);
  temp = (uint64_t)ADDR_FROM_LA(temp);
  pte = KADDR(temp);
  // pte = KADDR(ADDR_FROM_LA(*(pde+pde_index));
   pte = &pte[PTX(va)];
   return pte;
}
*/


pte_t *pml4e_walk(pml4e_t *pml4e, const	void *va, int create)
{
pte_t *pml4indexptr = NULL;
int pml4eindex;
//int pdpeindex;
uint64_t temp;
struct Page *page=NULL;
if(pml4e==NULL)
return NULL;
pml4eindex= PML4(va);

if((uint64_t*)pml4e[pml4eindex]==NULL && create== false)
return NULL;

else if((uint64_t*)pml4e[pml4eindex] == NULL && create== true)
{
//int perm = PTE_USER;
int perm = 0x7;
page = page_alloc(ALLOC_ZERO);
if(!page)
return NULL;
page->pp_ref++;
pml4e[pml4eindex] = page2pa(page) | perm;
//pml4e[pml4eindex] = page2pa(page) | 0x7;
}

temp = pml4e[pml4eindex];
temp = PTE_ADDR(temp);
temp= (uint64_t)KADDR(temp);
//kprintf("PDPE Addr %p\n",(pte_t*)temp);
pml4indexptr = (pte_t*)pdpe_walk((pte_t*)temp, va, create);
if (pml4indexptr == NULL && page!=NULL)
{
page_decref(page);
pml4e[pml4eindex] = 0;
}
return pml4indexptr;
}

pte_t *pdpe_walk(pdpe_t *pdpe, const void *va, int create)
{
pte_t* pdpeindexptr = NULL;
int pdpeindex = 0;
//int pgdirindx =0;
uint64_t temp;
struct Page *page = NULL;
if(pdpe == NULL)
{
return NULL;
}
pdpeindex = PDPE(va);
if(!(uint64_t*)pdpe[pdpeindex] && create == false)
return NULL;
else if((uint64_t*)pdpe[pdpeindex] == NULL && create==true)
{
//int perm = PTE_W | PTE_U | PTE_P;
int perm = 0x7;
struct Page *page = page_alloc(ALLOC_ZERO);
if(!page)
return NULL;
page->pp_ref++;
pdpe[pdpeindex] = page2pa(page) | perm;
}
temp = pdpe[pdpeindex];
temp = PTE_ADDR(temp);
temp = (uint64_t)KADDR(temp);
//kprintf("PDPE Addr %p\n",(pte_t*)temp);
pdpeindexptr = (pte_t*)pgdir_walk((pte_t*)temp,va,create);
if(pdpeindexptr == NULL && page!=NULL)
{
page_decref(page);
pdpe[pdpeindex]=0;
}
return pdpeindexptr;
}

pte_t *pgdir_walk(pde_t *pgdir, const void *va, int create)
{
pte_t *pgtblindexptr=NULL;
int dirindex;
int pgtblindex;
uint64_t temp;
if(pgdir == NULL)
return NULL;
dirindex = PDX(va);
pgtblindex = PTX(va);
if((uint64_t*)pgdir[dirindex]==NULL && create == false)
return NULL;
else if((uint64_t*)pgdir[dirindex]==NULL && create == true)
{
//int perm = PTE_W | PTE_U | PTE_P;
int perm = 0x7;
struct Page *page = page_alloc(ALLOC_ZERO);
if(!page)
return NULL;
page->pp_ref++;
pgdir[dirindex] = page2pa(page) | perm;
}
temp = pgdir[dirindex];
temp= PTE_ADDR(temp);
temp = (uint64_t)KADDR(temp);
//kprintf("PDPE Addr %p\n",(pte_t*)temp);
pgtblindexptr = (pte_t*)temp + pgtblindex;
return pgtblindexptr;
}

void boot_map_region(pml4e_t *pml4e, uint64_t la, size_t size, uint64_t pa, int perm)
{
  /*pte_t *pgtblindexptr=NULL;
  int i=0;
  if(pml4e == NULL){
    return;
  }

  for(i=0;i<size/PGSIZE;i++){
    pgtblindexptr = pml4e_walk(pml4e, (void*)la, 1);
    if(pgtblindexptr == NULL){
      kprintf("Could not Boot Up!!");
      continue;
    }
    *pgtblindexptr = pa | perm;
    pa += PGSIZE;
    la+=PGSIZE;
  }*/
  
  
  uint32_t i = 0;
  pte_t *pte_ptr;
  uint64_t va = CEIL(la, PGSIZE);
  for(i=0 ;i< CEIL(size, PGSIZE); i+=PGSIZE){
  pte_ptr = pml4e_walk(pml4e, (char*)(va+i), 1);
  if(pte_ptr!=NULL){
  *pte_ptr = pa+i;
  *pte_ptr = *pte_ptr | (perm|PTE_P|PTE_U|PTE_W);
  }
  }
}

uint64_t* kmalloc(uint64_t size){
  uint64_t count=0;
  struct Page *p;
  if (size<PGSIZE)
    count++;
  else{
    count = size/PGSIZE; //this was PGSIZE/size earlier -- i changed
    if((size - (count*PGSIZE)>0))
    count++;
  }

  //page = alloc_free_page();
  if(page_free_list){
    p = page_free_list;
  uint64_t *st = (uint64_t *)page2kva(p);

  for(int i=0;i<count; i++){
    p->pp_ref++;
    memset(page2kva((void*)p), 0, PGSIZE);
    p = p->pp_link;
  }
  page_free_list = p;
  return st;
  }
  return NULL;
}


void pfree(struct Page *p)
{
if (p->pp_ref == 0)
{
p->pp_link = page_free_list;
page_free_list = p;
}
else
panic("pp_ref for page is not zero");
}

int insertPage(pml4e_t *pml4e, struct Page *page, void *vaddr, int pflag){ //mapping new page at virtual entry
	pte_t *pte = pml4e_walk(pml4e,vaddr,1); //pte for vaddr
	if(pte != NULL){
		page->pp_ref++;
		if(*pte & PTE_P){
			removePage(pml4e,vaddr);
		  invalidateTLB(pml4e,vaddr);
		}	
		//*pte = ((uint64_t)page2pa(page)) | pflag | PTE_U | PTE_W |PTE_P;//add new mapping
   *pte = ((uint64_t)page2pa(page)) |0x7;
	}else{
		return MEM_EXCEED; //error -- no memory for allocation
	}
	return 0;
}

void removePage(pml4e_t *pml4e,void *vaddr){ //removes mapping for vaddr
	pte_t *pte;
	struct Page *page;
	if((page= searchPage(pml4e,vaddr,&pte))!= NULL){
		if(pte){
			*pte = 0;
			invalidateTLB(pml4e,vaddr);
		}
		page_decref(page);
	}
}

//searchPage returns page at given virtual address
struct Page* searchPage(pml4e_t *pml4e, void *vaddr,pte_t **rpte){
	pte_t *pte =  pml4e_walk(pml4e,vaddr,0);
	
	 if(pte == NULL){
                *rpte = NULL;
                return NULL; //no page mapped
        }

	if(*pte != 0){
		 if(rpte != NULL) *rpte = pte;
		 return pa2page(PTE(*pte));
	}
	return NULL;
}

static inline void __flush_tlb(uint64_t addr){
	__asm__ volatile("invlpg (%0)" : : "r" ((uint64_t)addr) : "memory");
}
void invalidateTLB(pml4e_t *pml4e,void *vaddr){
	//check if the page entry being changed is currently used by the processor
	__flush_tlb((uint64_t)vaddr);
}

uint64_t* userMalloc(pml4e_t *pml4e, uint64_t size){
	static uint64_t *bptr = (uint64_t*)0xFFFFF0F080700000; //need to see this?
  //static uint64_t *bptr = (uint64_t*)0xF000000; //need to see this?
	uint64_t *ret;
	ret =  bptr;
	uint64_t numBlocks;
	uint64_t *pte = NULL;
	uint64_t i;
	struct Page *p = NULL;
	numBlocks = size/PGSIZE;
	if(size - ((size/PGSIZE)*PGSIZE) !=0){
		numBlocks++;
	}
	while(numBlocks){
		numBlocks --;
		for(i =0;i< CEIL(size, PGSIZE);i +=PGSIZE){
			pte = pml4e_walk(pml4e, (char*)bptr,1);
			p = page_alloc(ALLOC_ZERO);
			p->pp_ref++;
			//*pte = ((uint64_t)page2pa(p)) | PTE_P |PTE_R | PTE_U;
    *pte = ((uint64_t)page2pa(p)) | 0x7;
 		}
		bptr +=512; //number of entries is 512
	}
	return ret;
}
