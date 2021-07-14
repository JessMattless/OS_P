[bits 32]
; Define constants to be used by the code
VIDEO_MEMORY: equ 0xb8000 ; This is the location of video memory defined by the 
                          ; default VGA display device for the PC

WHITE_ON_BLACK equ 0x0f   ; The way the display driver works at this low level is using 2 bytes.
                          ; As an example the word 0x4b0f
                          ; the The first byte is used for the character, E.g. 0x4b or the letter 'K'
                          ; The second byte defines thing like text/background colour and whether
                          ; the text blinks.

print_string_pm:
    pusha                 ; Used to push all registers to the stack
    mov edx, VIDEO_MEMORY ; set the edx register to the start of the video memory

print_string_pm_loop:
    mov al, [ebx]          ; Store the character at the location specified in the ebx register in al
    mov ah, WHITE_ON_BLACK ; Store the text attributes in ah
                           ; After this, the ax register will be al+ah
                           ; E.g. al: 0x4b, ah: 0x0f, ax = 0x4b0f
    
    cmp al, 0     ; if (al == 0), meaning the end of the string
    je print_string_pm_done       ; jump to done

    mov [edx], ax ; Store the character/attributes at the current character cell
                  ; in video memory
    
    add ebx, 1    ; Increment ebx to the next character in the string
    add edx, 2    ; Move to the next character in video memory

    jmp print_string_pm_loop ; loop to next character

print_string_pm_done:
    popa
    ret           ; return from the function