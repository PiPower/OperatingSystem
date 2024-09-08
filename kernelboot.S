.code16
.text
.global _kernelboot_start

_kernelboot_start:
    movb $0x0e, %ah
    movb $104, %al
    int $0x10
	jmp	.

.org 0x8000, 255