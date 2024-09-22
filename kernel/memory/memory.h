#ifndef MEMORY
#define MEMORY

/*
    this is main api for managing memory in the system
*/


void setup_paging();
void* kmalloc();
void init_memory_subsystem();

#endif