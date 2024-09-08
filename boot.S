.code16
.text
.global _start
#define BOOTLOAD_SECTOR 0x07c0
_start:
    movw $BOOTLOAD_SECTOR, %ax
    movw %ax, %cs
    movw $_xd, %ax
    jmp *%ax
    .byte 255
    .byte 255
    .byte 255
    .byte 255
    .byte 255
_xd:

    movb $0x0e, %ah
    movb $104, %al
    int $0x10
	jmp	.

.org 510, 0
.word 0xaa55 