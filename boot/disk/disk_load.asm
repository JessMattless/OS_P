disk_load:
    push dx     ; Push DX on the stack so we can read the number of
                ; requested sectors later for comparison.

    mov ah, 0x02 ; BIOS read sector function

    ;mov dl, 0      ; Read drive 0, in this case the first floppy drive
    mov al, dh     ; Read n sectors from the starting point
    mov ch, 0x00   ; Select cylinder 3 from the drive
    mov dh, 0x00   ; Select the track on the second side of the disk, since it is base 0
    mov cl, 0x02   ; Select the 4th sector of the track, this has a base of 1

    ; Set the address that we want the BIOS to read the sectors to
    ; The BIOS expects to find this address in ES:BX.
    ; The way it works is by setting es to an address,
    ; multiply it by 16 (shift left once), and offsetting it 
    ; based on the value within BX
    mov bx, 0x0008  ; Used to set the es register, because you cannot
    mov es, bx      ; directly give it a hex value
    mov bx, 0x0100  ; Set the offset to 0x1234
    ; In this case, the address would end up as 0xa000:0x1234
    ; This will then be taken as address 0xa1234 by the CPU

    int 0x13 ; Call the BIOS interrupt that reads the sector(s)

    jc disk_error   ; jc is a jumping instruction,
                    ; that only jumps if the carry flag is set.
                    ; We use this to check if the number of sections requested
                    ; and the number of sections read is the same
    pop dx
    cmp dh, al       ; Compare the number of sectors read to the number requested
    jne disk_error
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $ ; stay here forever

DISK_ERROR_MSG: db "Disk read error", 0
