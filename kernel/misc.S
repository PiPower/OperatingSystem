#define ASSEMBLER
#include "paging.h"

.text
.code32
.global start_paging
start_paging:
    movl $0x00100000, %eax 
    movl %eax, %cr3

    movl %cr0, %eax
    orl $0x80000000, %eax 
    movl %eax, %cr0
    ljmp $0x08, $ret_por
ret_por:

    ret 