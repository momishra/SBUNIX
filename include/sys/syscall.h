#ifndef __SYSCALL_H
#define __SYSCALL_H
#include<sys/defs.h>
#include<sys/isr.h>
#include<sys/memlayout.h>

#define CALL(n)static __inline uint64_t __syscall##n

CALL(0) (uint64_t n){
	uint64_t a;
	__asm__ volatile("int $0x80":"=a"(a):"0"(n));
	return a;
}

CALL(1) (uint64_t n, uint64_t ar1){
	uint64_t a;
	__asm__ volatile("int $0x80":"=a"(a):"0"(n), "D"((uint64_t)ar1));
	return a;
}

CALL(2) (uint64_t n, uint64_t ar1, uint64_t ar2){
	uint64_t a;
	__asm__ volatile("int $0x80":"=a"(a):"0"(n), "D"((uint64_t)ar1), "S"((uint64_t)ar2));
	return a;
}


CALL(3)(uint64_t n, uint64_t ar1, uint64_t ar2, uint64_t ar3) {
    int64_t a; 
    __asm__ volatile("int $0x80" : "=a" (a) : "0" (n), "D" ((uint64_t)ar1), "S" ((uint64_t)ar2), "b"((uint64_t)ar3)); 
    return a;
}

void init_syscall();
void syscall_wrap(struct register_t *r);
uint64_t sched_sys();
void handle_page_fault(struct register_t *r);
//void myfork();

#endif