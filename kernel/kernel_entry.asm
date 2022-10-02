; We use this to ensure that we jump straight
; to the entry function of the kernel
;[bits 32]       ; By now we're in protected mode, so 32-bit
[extern _start] ; instructions can be used
                ; Declare that we want to reference an external
                ; symbol named 'main'
                ; so it can be substituted by the linker to use the
                ; final address

call _start ; Invoking main() from our C kernel
jmp $