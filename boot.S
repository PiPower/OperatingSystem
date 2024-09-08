.code16
.text
.global _bootloader_start
.set BOOTLOAD_SECTOR, 0x07c0
.set STACK_TOP, 0x7DFF
_bootloader_start:
    # set cs = BOOTLOAD_SECTOR and ip to be in refrence to this file
    ljmp $BOOTLOAD_SECTOR, $_run
_run:
    # set segment register 
    movw $BOOTLOAD_SECTOR, %ax
    movw %ax, %ds
    movw $0, %ax
    movw %ax, %ss
    # set stack
    movw $STACK_TOP, %sp 
    movw $STACK_TOP, %bp
_xd:

    movb $0x0e, %ah
    movb $104, %al
    int $0x10
	jmp	.

.org 510, 0
.word 0xaa55 