ENTRY(main)
OUTPUT_FORMAT(elf64-x86-64)

/* MEMORY {
    ROM (rx) : ORIGIN = 0, LENGTH = 256k
    RAM (wx) : org = 0x00100000, len = 1M
} */

SEARCH_DIR(kernel)
SEARCH_DIR(drivers)

INPUT(
    kernel.o
    screen.o
)

OUTPUT(kernel.bin)

SECTIONS
{
    . = 0x00000000;
    __start = .;
    __text_start = .;
    .text : {
        *(.text);
    }
    . = ALIGN(4096);
    __text_end = .;

    __rodata_start = .;
    .rodata : {
        *(.rodata)
    }
    . = ALIGN(4096);
    __rodata_end = .;

    __data_start = .;
    .data : {
        *(.data)
    }
    . = ALIGN(4096);
    __data_end = .;

    __bss_start = .;
    .bss : {
        /* bss = .; */
        *(.bss)
    }
    . = ALIGN(4096);
    __bss_end = .;
    __end = .;
}