; Simple assembly boot sector, That prints a message to the screen using a routine
; The way the a- registers work here (ax, ah, al) are as follows
; mov ax, 0    -> 0x0000
; mov ah, 0x56 -> 0x5600
; mov al, 0x23 -> 0x5623
; mov ah, 0x16 -> 0x1623
; mov ax, 0    -> 0.0000
; These 3 registers are all linked, ax referring to the whole word of data
; ah reffers to only the first byte, and al reffers to only the second

[org 0x7c00]

    mov [BOOT_DRIVE], dl ; BIOS stores the boot drive in DL

    mov bp, 0x8000 ; Set the stack out of the way for when we load our sectors
    mov sp, bp

    mov bx, 0x9000 ; Load 5 sectors to 0x09000 (es = 0x0000:bx = 0x9000)
    mov dh, 5
    mov dl, [BOOT_DRIVE]
    call disk_load

    mov dx, [0x9000] ; Print the first word loaded at the address 0x9000
    call print_hex   ; this should be 0xdada

    call print_new_line

    mov dx, [0x9000 + 512] ; Also print the first word loaded in the
    call print_hex         ; second loaded sector, should be 0xface


    ;mov bx, BOOT_MSG  ; Move the contents in the location specified in HELLO_MSG
    ;call print_string ; to the bx register, then call the function that uses it.

    ;call print_new_line

    ;mov dx, 0x1fb6    ; Same as with the print_string function, but taking the value given in
    ;call print_hex    ; dx, instead of bx.

    jmp $ ; This is used to jump to the current address in memory (Which in this case loops forever)

; Include the file "print_string.asm", the line gets replaced with the contents of the file.
%include "print_string.asm" 
%include "print_hex.asm"
%include "disk_load.asm"

; Data
BOOT_MSG:               ; Define a label in the file, so you can easily grab the value within it.
    db 'Booting OS', 0  ; Add a 0 to the end of the string, so the program knows when it has hit the end.
BOOT_DRIVE: db 0

times 510-($-$$) db 0   ; When the code is compiled, the boot sector must be a multiple of 512 butes, the final 2 byutes needing to be 0xaa55
                        ; This line tells the program to pad the file up to 510 bytes, we add the final 2 manually.

dw 0xaa55               ; Final 2 bytes used to tell the BIOS this is a boot sector.

; BIOS usually only loads the first 512-byte sector from the disk.
; so if we add more sectors we can test that the disk loading code works

times 256 dw 0xdada
times 256 dw 0xface