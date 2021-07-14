; Global Descriptor Table, used to define different characteristics
; for addressing memory in 32-bit mode
; it's split into two sections, the code segment
; and data segment descriptors

; Not entirely sure how this all works, but it's necessary for 32-bit mode
; More Info: https://en.wikipedia.org/wiki/Global_Descriptor_Table

gdt_start:

gdt_null:  ; Null descriptor, mandatory
    dd 0x0 ; dd means double word (4 bytes)
    dd 0x0

gdt_code:  ; The code segment descriptor
    ; base = 0x0, limit = 0xfffff
    ; 1st flags: (present)1 (privelage)00 (descriptor type) 1 - 1001b
    ; type flags: (code)1 (conforming)0 (readable)0 (accessed)9 - 1010b
    ; 2nd flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 - 1100b
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10011010b ; 1st flags (1001b) + type flags (1010b)
    db 11001111b ; 2nd flags, Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

gdt_data:  ; The data segment descriptor
    ; Same as code segment except for different type flags
    ; type flags: (code)0 (expand down)0 (writable)1 (accessed)0 - 0010b
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; 1st flags (1001b) + type flags (0010b)
    db 11001111b ; 2nd flags, Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

gdt_end:         ; The reason this is here is so the assembler can
                 ; calculate the side of the GDT for the GDT descriptor

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of the GDT, always less one
                                ; of the true size
    dd gdt_start                ; Start address of the GDT

; Define some constants for the GDT segment descriptor offsets
; these are what segment registers have to contain when running 
; in protected mode. E.g. when setting DS = 0x10 in 32-bit mode,
; the CPU knows that we want it to use the segment described at
; the offset 0x10 (16 bytes) in the GDT, which in this case is
; the data segment (0x0 - NULL; 0x08 - CODE; 0x10 - DATA)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start