#ifndef PAGING 
#define PAGING


/*
    kernel stores page dir at PAGE_DIR_ADDR and first 
    page table at addr PAGE_TABLE_ADDR
*/
#define PAGE_SIZE 0x1000
#define PAGE_DIR_ADDR 0x00100000
#define PAGE_TABLE_ADDR 0x00101000


#ifndef ASSEMBLER
void setup_paging();
#endif 

#endif
