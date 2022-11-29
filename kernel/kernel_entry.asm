;We use this to ensure that we jump straight
; to the entry function of the kernel
; [bits 32]       ; By now we're in protected mode, so 32-bit
; [extern main] ; instructions can be used
                ; Declare that we want to reference an external
                ; symbol named 'main'
                ; so it can be substituted by the linker to use the
                ; final address

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top

    extern kernel_main
    call kernel_main ; Invoking main() from our C kernel

    cli
.hang: hlt
    jmp .hang

.end:
