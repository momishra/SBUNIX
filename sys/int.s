.text
.global test
.global timr
.global keyint
.extern irq_handler
.global idt_load
.global switch2User
.global switch_process
idt_load:
	lidt (%rdi)
	retq

	
test:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax 
	sti
	
	
timr:
	cli
	pushq %rdi
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %r8
    pushq %r9
    movq %rsp,%rdi
    call irq_function
    popq %r9
    popq %r8
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    popq %rdi
    //addq %rsp, 8
    sti 
    iretq


keyint:
	cli
	pushq %rdi
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %r8
    pushq %r9
    movq %rsp,%rdi
    call int_key
    popq %r9
    popq %r8
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    popq %rdi
    //addq $16, %rsp
    sti 
    iretq
	
	
switch2User:
  //cli  
  movq $0x23, %rax 
  movq %rax, %ds
  movq %rax, %es 
  movq %rax, %fs 
  movq %rax, %gs 
  movq %rsp, %rax 
  pushq $0x23
  pushq %rax
  pushfq
  //popq %rax
  //orq $0x200, %rax
  //pushq %rax
  pushq $0x2B
  pushq %rdi
  iretq

switch_process:
  //cli
  pushq %rax
pushq %rbx
pushq %rcx
pushq %rdx
pushq %rsi
pushq %rdi
pushq %rbp
pushq %r8
pushq %r9
pushq %r10
pushq %r11
pushq %r12
pushq %r13
pushq %r14
pushq %r15

#            movq %rsp,%rdi
#            addq $72,%rdi
//call timer_callback
call contextSwitch
/*
movq $0x23, %rax
movq %rax, %ds
movq %rax, %es
movq %rax, %fs
movq %rax, %gs
movq %rsp, %rax
*/
popq %r15
popq %r14
popq %r13
popq %r12
popq %r11
popq %r10
popq %r9
popq %r8
popq %rbp
popq %rdi
popq %rsi
popq %rdx
popq %rcx
popq %rbx
popq %rax
sti
iretq
//.endm



