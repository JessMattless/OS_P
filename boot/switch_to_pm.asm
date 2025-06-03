[bits 16]
; Used to switch to 32-bit protected mode
switch_to_pm:
    cli     ; Interripts must be switched off during the transition to
            ; protected mode, until we have set up the interrupt vector
            ; correctly, otherwise interrupts would crash the CPU
    
    lgdt [gdt_descriptor]   ; Load the Global Descriptor Table.
                            ; this defines the protected mode segments
                            ; for code and data
    
    mov eax, cr0            ; To switch to protected mode, we must set the
    or eax, 0x1             ; first bit of the cr0 register, we do this by
    mov cr0, eax            ; moving the cr0 register into the eax register
                            ; then oring it with 0x1, then moving it back

    jmp CODE_SEG:init_pm    ; To correctly switch, we must make a "far jump"
                            ; to a new segment of code, this forces all
                            ; current tasks to complete on the CPU before the
                            ; switch is executed and ensures no 16-bit
                            ; instructions are run on the now 32-bit CPU

    [bits 32]
    ; Initialize the registers and stack, now in 32-bit protected mode
    init_pm:
        mov ax, DATA_SEG    ; Now in 32-bit mode, any old segments are useless
        mov ds, ax          ; so we now point any segment registers to the
        mov ss, ax          ; data selector defined in the GDT
        mov es, ax
        mov fs, ax
        mov gs, ax

        mov ebp, 0x90000    ; Update the stack position, to put it
        mov esp, ebp        ; at the very top of free space

        call BEGIN_PM