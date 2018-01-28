#ifndef IDT_H
#define IDT_H

#include"defs.h"

/*
typedef struct {
	uint32_t low_off;
	uint32_t selector;
	uint16_t always;
	uint16_t flag;
	uint32_t high_off;
}__attribute__((packed))idt_handler;
*/

typedef struct{
   uint16_t offset_1; 
   uint16_t selector; 
   uint8_t ist;       
   uint8_t type_attr; 
   uint16_t offset_2; 
   uint32_t offset_3; 
   uint32_t zero;     
}__attribute__((packed))idt_desc;

typedef struct {
	uint16_t limit;
	uint64_t base;
}__attribute__((packed))idt_address;

idt_desc idt[256];
idt_address idt_add;
//extern void idt_load(idt_address *idt_add);

void set_idt_handler(int n, uint64_t handler);
void set_idt_user(int n, uint64_t handler);
void set_idt();
#endif
