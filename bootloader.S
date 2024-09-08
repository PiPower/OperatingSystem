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
    # loading kernel boot
    movb %dl, _disk_sector
    movw $0, %ax
    movb _disk_sector, %al
    jmp .

_bootloader_msg:
    .asciz "botloader loader kernel boot \n"
_disk_sector:
    .byte 0

.org 510, 0
.word 0xaa55 