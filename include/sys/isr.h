#ifndef ISR_H
#define ISR_H

struct register_t {
	//uint64_t r9,r8,rsi,rbp,rdx,rcx,rbx,rax,rdi; 
 uint64_t r9,r8,rsi,rbp,rdx,rcx,rbx,rdi;
	uint64_t int_no, err_code; 
  uint64_t rip,cs,rflags,userrsp,ss; 
}__attribute__((packed));

extern void test();
extern void timr();
extern void testISR();
void idt_install();
extern void irq_function();
void int_handler();
extern int tck;
//typedef void (*isr_t)(registers_t);

void reg_int_handler(int n, void (*handler));

extern void keyint();
extern void int_key();

void scancodes();
void checkAllBuses();

void key_install();

void isr_install();

//void irq_handler(struct register_t *r);

#endif
