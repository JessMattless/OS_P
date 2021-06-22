; Simple assembly boot sector, That prints a message to the screen using a routine


; The way the a- registers work here (ax, ah, al) are as follows
; mov ax, 0    -> 0x0000
; mov ah, 0x56 -> 0x5600
; mov al, 0x23 -> 0x5623
; mov ah, 0x16 -> 0x1623
; mov ax, 0    -> 0.0000
; These 3 registers are all linked, ax referring to the whole word of data
; ah reffers to only the first byte, and al reffers to only the second

mov ah, 0x0e            ; int 10/ah = 0eh -> scrolling teletype BIOS routine used for writing to the screen

mov al, 'H'             ; Put the ascii character 'H' in the AL register (0x48)
int 0x10                ; int 10/ah = 0eh -> scrolling teletype BIOS routine used for writing to the screen
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

jmp $                   ; This is used to jump to the current address in memory (Which in this case loops forever)

times 510-($-$$) db 0   ; When the code is compiled, the boot sector must be a multiple of 512 butes, the final 2 byutes needing to be 0xaa55
                        ; This line tells the program to pad the file up to 510 bytes, we add the final 2 manually.

dw 0xaa55               ; Final 2 bytes used to tell the BIOS this is a boot sector.