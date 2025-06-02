; Simple assembly boot sector, That prints a message to the screen using a routine
; The way the a- registers work here (ax, ah, al) are as follows
; mov ax, 0    -> 0x0000
; mov ah, 0x56 -> 0x5600
; mov al, 0x23 -> 0x5623
; mov ah, 0x16 -> 0x1623
; mov ax, 0    -> 0x0000
; These 3 registers are all linked, ax referring to the whole word of data
; ah refers to only the first byte, and al refers to only the second

[org 0x7c00]
KERNEL_OFFSET equ 0x2000 ; This is the memory offset used to load the kernel

    mov [BOOT_DRIVE], dl ; BIOS stores the boot drive in DL, so
                         ; it's stored there for later

    mov bp, 0x9000  ; Set the stack out of the way
    mov sp, bp      ; We cannot set sp directly so we use bp
                    ; to set it indirectly

    mov bx, BOOT_MSG_16
    call print_string

    call print_new_line

    call load_kernel

    call switch_to_pm ; We never return from here

    jmp $ ; This is used to jump to the current address in memory (Which in this case loops forever)

; Include the file "x", the line gets replaced with the contents of the file.
%include "boot/print/print_string.asm" 
%include "boot/disk/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/print/print_string_pm.asm" 
%include "boot/switch_to_pm.asm"
;%include "hex/print_hex.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL ; Print a message on screen to notify the loading
    call print_string       ; of the kernel

    call disk_load          ; specified in KERNEL_OFFSET

    ret

[bits 32]
; This is where we end up from switch_to_pm above, after initializing
; 32 bit protected mode
BEGIN_PM:
    mov ebx, BOOT_MSG_32
    call print_string_pm ; Use the new 32 bit print string routine,
                         ; as the old one doesn't work now

    call KERNEL_OFFSET

    jmp $

; Data
; Define a label in the file, so you can easily grab the value within it.
; Add a 0 to the end of the string, so the program knows when it has hit the end.
BOOT_DRIVE       db 0x80
BOOT_MSG_16      db 'Booting OS in 16-bit mode', 0  
BOOT_MSG_32      db 'Booting OS in 32-bit protected mode', 0
MSG_LOAD_KERNEL  db 'Loading kernel into memory', 0


times 510-($-$$) db 0   ; When the code is compiled, the boot sector must be a multiple of 512 bytes, the final 2 bytes needing to be 0xaa55
                        ; This line tells the program to pad the file up to 510 bytes, we add the final 2 manually.
dw 0xaa55               ; Final 2 bytes used to tell the BIOS this is a boot sector.