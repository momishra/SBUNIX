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
.extern fault_handler

isr_common_stub:
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
    call fault_handler
    popq %r9
    popq %r8
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    popq %rdi
    addq $16 ,%rsp 
    iretq

#  0: Divide By Zero Exception
isr0:
     cli
    pushq $0       # error code
    pushq $0       # INT Number  
    jmp isr_common_stub

isr1:
     cli
    pushq $0       # error code
    pushq $1       # INT Number
    jmp isr_common_stub

isr2:
     cli
    pushq $0       # error code
    pushq $2       # INT Number
    jmp isr_common_stub

isr3:
     cli
    pushq $0       # error code
    pushq $3       # INT Number
    jmp isr_common_stub

isr4:
     cli
    pushq $0       # error code
    pushq $4       # INT Number
    jmp isr_common_stub

isr5:
     cli
    pushq $0       # error code
    pushq $5       # INT Number
    jmp isr_common_stub

isr6:
     cli
    pushq $0      # error code
    pushq $6       # INT Number
    jmp isr_common_stub

isr7:
     cli
    pushq $0       # error code
    pushq $7       # INT Number
    jmp isr_common_stub

isr8:
     cli
    pushq $8       # INT Number
    jmp isr_common_stub

isr9:
     cli
    pushq $0       # error code
    pushq $9       # INT Number
    jmp isr_common_stub

isr10:
     cli
    pushq $10      # INT Number
    jmp isr_common_stub

isr11:
     cli
    pushq $11      # INT Number
    jmp isr_common_stub

isr12:
     cli
    pushq $12      # INT Number
    jmp isr_common_stub

isr13:
     cli
    pushq $13      # INT Number
    jmp isr_common_stub

isr14:
     cli
    pushq $14      # INT Number
    jmp isr_common_stub

isr15:
     cli
    pushq $0       # error code
    pushq $15      # INT Number
    jmp isr_common_stub

isr16:
     cli
    pushq $0       # error code
    pushq $16      # INT Number
    jmp isr_common_stub

isr17:
     cli
    pushq $0       # error code
    pushq $17      # INT Number
    jmp isr_common_stub

isr18:
     cli
    pushq $0       # error code
    pushq $18      # INT Number
    jmp isr_common_stub

isr19:
     cli
    pushq $0       # error code
    pushq $19      # INT Number
    jmp isr_common_stub

isr20:
     cli
    pushq $0       # error code
    pushq $20      # INT Number
    jmp isr_common_stub

isr21:
     cli
    pushq $0       # error code
    pushq $21      # INT Number
    jmp isr_common_stub

isr22:
     cli
    pushq $0       # error code
    pushq $22      # INT Number
    jmp isr_common_stub

isr23:
     cli
    pushq $0       # error code
    pushq $23      # INT Number
    jmp isr_common_stub

isr24:
     cli
    pushq $0       # error code
    pushq $24      # INT Number
    jmp isr_common_stub

isr25:
     cli
    pushq $0       # error code
    pushq $25      # INT Number
    jmp isr_common_stub

isr26:
     cli
    pushq $0       # error code
    pushq $26      # INT Number
    jmp isr_common_stub

isr27:
     cli
    pushq $0       # error code
    pushq $27      # INT Number
    jmp isr_common_stub

isr28:
     cli
    pushq $0       # error code
    pushq $28      # INT Number
    jmp isr_common_stub

isr29:
     cli
    pushq $0       # error code
    pushq $29      # INT Number
    jmp isr_common_stub

isr30:
     cli
    pushq $0       # error code
    pushq $30      # INT Number
    jmp isr_common_stub

isr31:
     cli
    pushq $0       # error code
    pushq $31      # INT Number
    jmp isr_common_stub
