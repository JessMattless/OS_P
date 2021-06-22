; Simple assembly boot sector, makes the sector loop forever

loop:                   ; Define a label, used to jump back to
    jmp loop            ; Instruction used to jump to a label somewhere in the code

times 510-($-$$) db 0   ; When the code is compiled, the boot sector must be a multiple of 512 butes, the final 2 byutes needing to be 0xaa55
                        ; This line tells the program to pad the file up to 510 bytes, we add the final 2 manually.

dw 0xaa55               ; Final 2 bytes used to tell the BIOS this is a boot sector.