##=============================================================================
##
##    variant.S
##
##    Freescale Quicc III variant code
##
##=============================================================================


#include <cyg/hal/variant.inc>

#---------------------------------------------------------------------------
# Interrupt vector tables.
# These tables contain the isr, data and object pointers used to deliver
# interrupts to user code.

        .data

        .extern hal_default_decrementer_isr
        .extern hal_default_isr

        .globl  hal_interrupt_handlers
hal_interrupt_handlers:
        .rept   CYGNUM_HAL_ISR_COUNT-2
        .long   hal_default_isr
        .endr
        .long   hal_default_decrementer_isr               # decrementer 
        .long   hal_default_isr                           # spurious interrupt
        
        .globl  hal_interrupt_data
hal_interrupt_data:
        .rept   CYGNUM_HAL_ISR_COUNT
        .long   0
        .endr

        .globl  hal_interrupt_objects
hal_interrupt_objects:
        .rept   CYGNUM_HAL_ISR_COUNT
        .long   0
        .endr

##-----------------------------------------------------------------------------
## end of variant.S

