print_string: ; Prints a string using the string provided in the bx register
    mov al, [bx]    ; Go to the location in memory where the string in *bx* is stored
                    ; e.g. if *bx* is "Hello", it goes to the start of that string. This sets *al* to 'H' or 0x48
    cmp al, 0       ; Compare al to 0
    jne print_char  ; If al != 0, jump to the "print_char" label
    ret             ; Else return

print_char: ; Prints a character on screen making use of the scrolling teletype BIOS routine
    mov ah, 0x0e        ; Used for printing to the screen
    int 0x10            ; Print the value stored in al
    add bx, 0x0001      ; Increment the memory location stored in *bx* by 1 byte
    jmp print_string    ; Jump back to the "print_function" label

print_new_line:  ; Prints a new line, making use of a carriage return and a line feed
    mov ah, 0x0e
    mov al, 0x0d ; This is the equivalent of a carriage return in hex
    int 0x10
    mov al, 0x0a ; This is the equivalent of a line feed in hex
    int 0x10
    ret
    