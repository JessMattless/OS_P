TABLE: db '0123456789ABCDEF', 0 ;Define the TABLE variable, to be used to grab the characters for the hex later
HEX_OUT: db '0x0000', 0 ;Define the output variable, which will be changed later to reflect the string to print

print_hex: ;Label
    push bx ;Push the BX register to the bottom of the stack
    mov bx, TABLE ;Move the TABLE variable into the BX register
    mov ax, dx ;Move the contents of the DX register into the AX register
               ;The DX register is the input (E.G: mov dx, '0x38cf')
               ;Each register is split into 2 sections, with AX being the entire register
               ;E.G.
               ;mov ax, 0x38cf | AX = 0x38cf | AH = 0x38 | AL = 0xcf
               ;This is the same for BX and DX too
    mov ah, al ;Copy the AL register to the AH register, | AX = 0xcfcf
    shr ah, 4 ;Shift the contents of the AH register right by 4 bits
              ;Before: 11001111 | After: 11111100 | AX = 0xfccf
    and al, 0x0f ;AND the contents of the al register with 0x0f. | 11001111 AND 00001111 | AL = 00001111
                 ;AX = 0xfc0f
    xlat ;A Table lookup, uses AL on the table BX (I'm not too sure what this does)
    xchg ah, al ;I really don't know what this does, it's used for data exchange between the two registers
    xlat ;Table lookup again

    mov bx, HEX_OUT ;Put HEX_OUT in the BX register
    add bx, 0x0004 ;Move BX forward by 4 chars | before: [0x0000] | after: 0x00[00]
                   ;Used to write further on in the string
    mov [bx], ax ;Move the contents of AX to the location specified by BX

    ;The steps above are repeated once again until the end, with the exception of a couple lines below
    mov bx, TABLE
    mov ax, dx

    mov al, ah ;Instead of moving al to ah, move ah to al.
               ;This is used to write the two digits left out by the first pass
    shr ah, 4
    and al, 0x0f
    xlat
    xchg ah, al
    xlat

    mov bx, HEX_OUT
    add bx, 0x0002 ;Only push forward 2 characters this time | [0x0000] | 0x[0000]
    mov [bx], ax
    mov bx, HEX_OUT
    call print_string ;Call a print string function defined elsewhere.
                      ;Used to print the now complete HEX_OUT string
    pop bx ;Pop the BX register from the bottom of the stack
    ret ;Return to the place in memory that caled the function
    