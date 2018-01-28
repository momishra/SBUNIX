.text
.global syscall

syscall:
    //cli
    pushq $0	# error code
    pushq $0x80 # INT number
    pushq %rdi
    //pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %r8
    pushq %r9
    movq %rsp,%rdi
    callq syscall_wrap
    popq %r9
    popq %r8
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    //popq %rax
    popq %rdi
    addq $16, %rsp
    iretq

