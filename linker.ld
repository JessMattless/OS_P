ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SEARCH_DIR(drivers)
SEARCH_DIR(kernel)

OUTPUT(kernel.bin)

SECTIONS
{
    . = 0x4000;
    .text BLOCK(4K) : ALIGN(4K) {
        *(.text);
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }
    
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
}