#include "memory_layout.h"

.code16
.text
.global _kernelboot_start
.extern main

_kernelboot_start:
    #set segment registers
    movw $KERNELLOADER_SEGMENT, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss 
    # set stack
    movw $KERNELLOADER_STACK_TOP, %sp
    movw $KERNELLOADER_STACK_TOP, %bp
    
    jmp _memtest
_go_to_main:
    movb $0x0e, %ah
    movb $0x65, %al
    int $0x10 # print S on successful load
    jmp .
    call main

_memtest:
    # adress of entries array is KERNELLOADER_SEGMENT + entries_offset
    movw $KERNELLOADER_SEGMENT, %ax
    movw %ax, %es
    movw $0x0, %bx
    movl $0x534D4150, %edx
    movw $entries, %di # entries have to fit within first 65 kb range
_call_mmap_int:
    movl $0x40, %ecx
    movl $0xe820, %eax
    clc
    int $0x15
    cmpl $0, %ebx
    jc _go_to_main
    je _go_to_main
    cmpl $10, %ebx # max 10 entries in table
    je _go_to_main
    # ebx != so it is not the last entry
    movl %ebx, entries_size
    add $20, %di
    jmp _call_mmap_int

_gdt:
    

entries_size:
    .word 0
    .word 255
entries:
   .space 200, 0
