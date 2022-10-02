ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)

MEMORY {
    ROM (rx) : ORIGIN = 0, LENGTH = 256k
    RAM (wx) : org = 0x00100000, len = 1M
}

INPUT(
    kernel/kernel.o
    kernel/low_level.o
    drivers/keyboard.o
    drivers/screen.o
)

OUTPUT(kernel.bin)

SECTIONS
{
    . = 0x8000;
    __start = .;
    __text_start = .;
    .text : {
       KEEP(*(.text.boot))
        *(.text_start)
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
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096);
    __bss_end = .;
    __end = .;
}