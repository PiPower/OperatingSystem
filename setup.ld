/*
 * setup.ld
 *
 * Linker script for the i386 setup code
 */
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
SECTIONS
{
	. = 0;
    __program_start = .;

        .kernel_boot : { 
        *(.kernel_boot) 
    }
    
    . += 0x10000;
      .text		: { 
        *(.text) 
    }
    __start_data_section = .;
	.rodata		: { 
        *(.rodata*)
        }

	.data		: { 
        *(.data*) 
        }
    
	.bss : {
		__bss_start = .;
		*(.bss)
		__bss_end = .;
	}
    __end_data_section = .;

	.pci_driver : {
		__pci_driver_start = .;
		*(.pci_driver)
		__pci_driver_end = .;
	}
    __program_end = .;
	. = ASSERT(__program_end - __program_start - 0x10000 <= 0x8000, "Kernel is too big !");

}