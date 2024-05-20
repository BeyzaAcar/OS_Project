.set IRQ_BASE, 0x20 # set the IRQ_BASE to 0x20

.section .text 

.extern _ZN16InterruptManager15handleInterruptEhj # extern declaration of the function handleInterrupt

.macro HandleException num
.global _ZN16InterruptManager16handleException\num\()Ev # declare the function handleInterruptRequest\num\()Ev (where \num\() is the number and \() is the empty string and v is the void return type)
    movb $num, interruptnumber # set the interrupt number to the value of the macro
    jmp int_bottom # jump to the bottom of the interrupt handler
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager15handleInterruptRequest\num\()Ev # declare the function handleInterruptRequest\num\()Ev (where \num\() is the number and \() is the empty string and v is the void return type)
    movb $num + IRQ_BASE, interruptnumber # set the interrupt number to the value of the macro
    jmp int_bottom # jump to the bottom of the interrupt handler
.endm

HandleInterruptRequest 00x0 # call the macro with the number 0x0
HandleInterruptRequest 00x1 # call the macro with the number 0x1

int_bottom:
    # First, we need to save the registers that are going to be used by the function handleInterrupt
    pusha # push all the registers on the stack
    pushl %ds # push the data segment register on the stack
    pushl %es # push the extra segment register on the stack
    pushl %fs # push the fs segment register on the stack
    pushl %gs # push the gs segment register on the stack

    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj # call the function handleInterrupt
    #addl $5, %esp # remove the arguments from the stack
    movl %eax, %esp # set the stack pointer to the value returned by the function

    # Now, we need to restore the registers that were saved (reverse order because its a stack)
    popl %gs # pop the gs segment register from the stack
    popl %fs # pop the fs segment register from the stack
    popl %es # pop the extra segment register from the stack
    popl %ds # pop the data segment register from the stack
    popa # pop all the registers from the stack

    iret # return from the interrupt



.data # data section
    interruptnumber: .byte 0 # variable to store the interrupt number