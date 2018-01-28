.text
.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31
.extern isr_handler

isr_common:
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
    call isr_handler
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
    iretq


isr0:
     cli
    pushq $0       # error code
    pushq $0       # INT Number  
    jmp isr_common

isr1:
     cli
    pushq $0       # error code
    pushq $1       # INT Number
    jmp isr_common

isr2:
     cli
    pushq $0       # error code
    pushq $2       # INT Number
    jmp isr_common

isr3:
     cli
    pushq $0       # error code
    pushq $3       # INT Number
    jmp isr_common

isr4:
     cli
    pushq $0       # error code
    pushq $4       # INT Number
    jmp isr_common

isr5:
     cli
    pushq $0       # error code
    pushq $5       # INT Number
    jmp isr_common

isr6:
     cli
    pushq $0      # error code
    pushq $6       # INT Number
    jmp isr_common

isr7:
     cli
    pushq $0       # error code
    pushq $7       # INT Number
    jmp isr_common

isr8:
     cli
    pushq $8       # INT Number
    jmp isr_common

isr9:
     cli
    pushq $0       # error code
    pushq $9       # INT Number
    jmp isr_common

isr10:
     cli
    pushq $0
    pushq $10      # INT Number
    jmp isr_common

isr11:
     cli
     pushq $0
    pushq $11      # INT Number
    jmp isr_common

isr12:
     cli
     pushq $0
    pushq $12      # INT Number
    jmp isr_common

isr13:
     cli
     pushq $0
    pushq $13      # INT Number
    jmp isr_common

isr14:
     cli
     pushq $0
    pushq $14      # INT Number
    jmp isr_common

isr15:
     cli
    pushq $0       # error code
    pushq $15      # INT Number
    jmp isr_common

isr16:
     cli
    pushq $0       # error code
    pushq $16      # INT Number
    jmp isr_common

isr17:
     cli
    pushq $0       # error code
    pushq $17      # INT Number
    jmp isr_common

isr18:
     cli
    pushq $0       # error code
    pushq $18      # INT Number
    jmp isr_common

isr19:
     cli
    pushq $0       # error code
    pushq $19      # INT Number
    jmp isr_common

isr20:
     cli
    pushq $0       # error code
    pushq $20      # INT Number
    jmp isr_common

isr21:
     cli
    pushq $0       # error code
    pushq $21      # INT Number
    jmp isr_common

isr22:
     cli
    pushq $0       # error code
    pushq $22      # INT Number
    jmp isr_common

isr23:
     cli
    pushq $0       # error code
    pushq $23      # INT Number
    jmp isr_common

isr24:
     cli
    pushq $0       # error code
    pushq $24      # INT Number
    jmp isr_common

isr25:
     cli
    pushq $0       # error code
    pushq $25      # INT Number
    jmp isr_common

isr26:
     cli
    pushq $0       # error code
    pushq $26      # INT Number
    jmp isr_common

isr27:
     cli
    pushq $0       # error code
    pushq $27      # INT Number
    jmp isr_common

isr28:
     cli
    pushq $0       # error code
    pushq $28      # INT Number
    jmp isr_common

isr29:
     cli
    pushq $0       # error code
    pushq $29      # INT Number
    jmp isr_common

isr30:
     cli
    pushq $0       # error code
    pushq $30      # INT Number
    jmp isr_common

isr31:
     cli
    pushq $0       # error code
    pushq $31      # INT Number
    jmp isr_common
