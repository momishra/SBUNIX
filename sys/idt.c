#include"../include/sys/defs.h"
#include"../include/sys/idt.h"
#include"../include/sys/kprintf.h"
void idt_load(idt_address *idt_add);

uint64_t get_bit(uint64_t num, int x)
{
	uint64_t temp = (1<<x) -1;
	return (num & temp);
}

void set_idt_handler(int n, uint64_t handler )
{
//uint64_t t = handler;
//kprintf("inside set_idt_handler %x", handler);
idt[n].offset_1 = get_bit(handler, 16);
idt[n].selector = 0x08;
idt[n].ist=0;
idt[n].type_attr = 0x8E;
idt[n].offset_2 = get_bit((handler>>16),(16));
idt[n].offset_3 = (handler>>32)&0xFFFFFFFF;
idt[n].zero= 0;
}

void set_idt_user(int n, uint64_t handler)
{
idt[n].offset_1 = get_bit(handler, 16);
idt[n].selector = 0x08;
idt[n].ist=0;
idt[n].type_attr = 0xEE;
idt[n].offset_2 = get_bit((handler>>16),(16));
idt[n].offset_3 = (handler>>32)&0xFFFFFFFF;
idt[n].zero= 0;

}

void set_idt()
{     
	//kprintf("inside set_idt"); 
	idt_add.base = (uint64_t)&idt;
	idt_add.limit = (uint32_t)((256*16)-1);
	idt_load(&idt_add);
	 //__asm__ volatile(
	//	"lidtq (%0)" :: "r"(&idt_add)
	//);
/*	__asm__ volatile(
		"lidtq (0x55555555)"
	);*/
        
}
