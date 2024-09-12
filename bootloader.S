#include "memory_layout.h"

.code16
.text
.global _bootloader_start
_bootloader_start:
    # set cs = BOOTLOAD_SECTOR and ip to be in refrence to this file
    ljmp $BOOTLOAD_SECTOR, $_run
_run:
    cli
    # set segment registers
    movw $BOOTLOAD_SECTOR, %ax
    movw %ax, %ds
    movw %ax, %es
    movw $0, %ax
    movw %ax, %ss
    # set stack
    movw $BOOTLOADER_STACK_TOP, %sp 
    movw $BOOTLOADER_STACK_TOP, %bp

    sti # disable interrupts
    cld 
_load_kernelloader:
    movw $KERNELLOADER_SEGMENT, %ax
    movw %ax, %es 
    movw $0x00, %bx # set bs to zero so es:bx  0x10000
    movb $0x02, %ah # BIOS read sector function
    movb $KERNELLOADER_SECTOR_COUNT, %al # Read sector count
    movb $0x00, %ch # Select cylinder 0
    movb $0x02, %cl # Start reading from second sector
    movb $0x00, %dh # Select head 0
    # dl is set on bootloader entry 
    clc # clear carry flag 
    int $0x13 # BIOS interrupt
    jc _load_error
    movb $0x0e, %ah
    movb $0x53, %al
    int $0x10 # print S on successful load
    ljmp $KERNELLOADER_SEGMENT, $KERNELLOADER_OFFSET
_load_error:
    movb $0x0e, %ah
    movb $0x46, %al
    int $0x10 # print F on failed load
	jmp	.
_bootloader_msg:
    .asciz "botloader loader kernel boot \n"
_disk_sector:
    .byte 0

.org 510, 0
.word 0xaa55 