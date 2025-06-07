; Global Descriptor Table, used to define different characteristics
; for addressing memory in 32-bit mode
; it's split into two sections, the code segment
; and data segment descriptors

; Not entirely sure how this all works, but it's necessary for 32-bit mode
; More Info: https://en.wikipedia.org/wiki/Global_Descriptor_Table
; IMPORTANT LINKS: https://wiki.osdev.org/GDT_Tutorial
; https://wiki.osdev.org/Global_Descriptor_Table

gdt_start:

gdt_null:  ; Null descriptor, mandatory
    dd 0x00000000 ; dd means double word (4 bytes)
    dd 0x00000000

gdt_code:  ; The code segment descriptor
    ; base = 0x0, limit = 0xfffff
    ; Access Byte: (present)1 (privilege)00 (descriptor type) 1 - 1001b
    ; Cont: (code)1 (conforming)0 (readable)0 (accessed)1 - 1010b
    ; Flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 - 1100b
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0000    ; Base (bits 16-31)
    db 0x00      ; Base (bits 32-39)
    db 10011011b ; Access Byte (bits 40-47)
    db 11001111b ; Flags, Limit (bits 48-55)
    db 0x00      ; Base (bits 56-63)

gdt_data:  ; The data segment descriptor
    ; Same as code segment except for different access byte
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0000    ; Base (bits 16-31)
    db 0x00      ; Base (bits 32-39)
    db 10010011b ; Access Byte (40-47)
    db 11001111b ; Flags, Limit (bits 48-55)
    db 0x00      ; Base (bits 56-63)

gdt_task:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10001001b
    db 11001111b
    db 0x00

gdt_end:         ; The reason this is here is so the assembler can
                 ; calculate the size of the GDT for the GDT descriptor

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ;- gdt_task   ; Size of the GDT, always less one
                                        ; of the true size
    dd gdt_start                        ; Start address of the GDT

; Define some constants for the GDT segment descriptor offsets
; these are what segment registers have to contain when running 
; in protected mode. E.g. when setting DS = 0x10 in 32-bit mode,
; the CPU knows that we want it to use the segment described at
; the offset 0x10 (16 bytes) in the GDT, which in this case is
; the data segment (0x0 - NULL; 0x08 - CODE; 0x10 - DATA)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
TASK_STATE_SEG equ gdt_task - gdt_start