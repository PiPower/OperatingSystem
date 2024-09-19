#include "memory_layout.h"
#define MAX_ENTRIES 16

.code16
.section ".kernel_boot", "ax"
.extern main
.global _start
_start:
    #set segment registers
    movw $KERNELLOADER_SEGMENT, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss 
    # set stack
    movw $KERNELLOADER_STACK_TOP, %sp
    movw $KERNELLOADER_STACK_TOP, %bp
_check_VGA:
    movw $0x1200, %ax
    movb $0x10, %bl
    int $0x10

    cmp $0x10, %bl
    je _failure # Adapter is CGA

    movw $0x1a00, %ax
    movb $0x10, %bl
    int $0x10
    cmp $0x08, %bl # 0x08 is code for VGA with analog color display
    jne _failure # Unsupported adapter
_memtest:
    # adress of entries array is KERNELLOADER_SEGMENT + entries_offset
    movw $KERNELLOADER_SEGMENT, %ax
    movw %ax, %es
    movw $0x0, %bx
    movl $0x534D4150, %edx
    movw $map_entries, %di # entries have to fit within first 65 kb range
    movw $0, %si # init counter
_call_mmap_init:
    movl $0x40, %ecx
    movl $0xe820, %eax
    clc
    int $0x15
    addw $1, %si
    jc _a20_line  
    cmpl $0, %ebx # cmpl may change carry flag so it is tested second
    je _a20_line 
    cmpw $MAX_ENTRIES, %si # jump if MAX_ENTRIES entries in table
    je _a20_line
    add $0x14, %di # offset entries buffer pointer
    jmp _call_mmap_init
_a20_line:
    movw %si, map_entries_size # store number of entries 
    mov $0x2403, %ax # A20-Gate Support
    int $0x15
    jb  _failure # INT 15h is not supported
    cmp $0x00, %ah 
    jnz _failure # INT 15h is not supported

    mov $0x2402, %ax # A20-Gate Status 
    int $0x15
    jb  _failure # couldn't get status
    cmp  $0x00, %ah 
    jnz _failure # couldn't get status

    cmp $0x01, %al
    jz _go_to_pm # A20 is already activated

    mov $0x2401, %ax # A20-Gate Activate
    int $0x15
    jb _failure # couldn't activate the gate
    cmp $0x00, %ah
    jnz _failure # couldn't activate the gate

_go_to_pm:
    movb $0x0e, %ah
    movb $0x65, %al
    int $0x10 # print E on successful entry to pm
    
    cli
    movb $0x80, %al
    outb  %al, $0x70 # Disable NMI 
    movb $0x00, %al
    outb %al, $0x80 # delay so that NMI can be in effect

    movb $0xff, %al
    outb %al, $0xa1 # Mask all interrupts on the secondary PIC
	movb $0x00, %al
    outb %al, $0x80 # delay so that NMI can be in effect

    movb $0xfb, %al
    outb %al, $0x21 # Mask all but cascade on the primary PIC 
	movb $0x00, %al
    outb %al, $0x80 # delay so that NMI can be in effect
    movb $0x65, %ah

    lgdtl gdt_init
    jmp	1f # Short jump to serialize on 386/486
1:
    movl %cr0, %edx
	orb	$0x1 , %dl	# Protected mode
	movl %edx, %cr0

    .byte 0x66, 0xea		# ljmp ptr16:32
    .long (_32bit_mode + KERNELLOADER_MEM_BASE)
    .word 8
 
_failure:
    movb $0x0e, %ah
    movb $0x46, %al
    int $0x10 # print S on successful load
    jmp . 

.align 16
.global gdt
gdt:
	.word	0,0,0,0	# seg 0 - null
	.word	0xffff, 0x0000, 0x9a00, 0x00cf	# seg 1 - kernel flat 4GB code
	.word	0xffff, 0x0000, 0x9200, 0x00cf	# seg 2 - kernel flat 4GB data
	.word	0xffff, 0x0000, 0xfa00, 0x00cf	# seg 3 - user flat 4GB code
	.word	0xffff, 0x0000, 0xf200, 0x00cf	# seg 4 - user flat 4GB data
	.word	0x0068, (tss- _start),0x8901, 0x00cf  # seg 5 - TSS

gdt_init:
	.word	gdt_init-gdt - 1
	.long	(gdt + KERNELLOADER_MEM_BASE) 

tss:
	.long	0

.global map_entries_size
map_entries_size:
    .word 0

.global map_entries
map_entries:
   .space (20 * MAX_ENTRIES), 0

.code32
.extern msg

_32bit_mode:
    # set segments to data segment
    movw $16, %ax
	movw %ax, %ds	
	movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
	movw %ax, %ss
    
    movl $0x90000, %esp
    movl $0x90000, %ebp
    
    movl $main, %eax
    call %eax

