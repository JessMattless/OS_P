; Interrupt Service Routine Wrapper

; Stub function for use with unregistered ISRs
; Calls interrupt_stub in interrupts.h
global _isr_stub
_isr_stub:
    pushad
    cld
    extern interrupt_stub
    call interrupt_stub
    popad
    iret

; Create a macro for creating handler functions
; Each handler function just calls interrupt_handler in interrupts.h with the interrupt number
%macro ISR_NOERR 1
global isr%1
isr%1:
    pusha
    push dword %1
    extern interrupt_handler
    call interrupt_handler
    add esp, 4
    popa
    iret
%endmacro

; run the macro x times
%assign i 0
%rep 256
    ISR_NOERR i
%assign i i+1
%endrep
