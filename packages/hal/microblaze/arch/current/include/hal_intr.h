#ifndef CYGONCE_HAL_INTR_H
#define CYGONCE_HAL_INTR_H
/*==========================================================================
//
//      hal_intr.h
//
//      Microblaze interrupt and clock abstractions
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2011 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    ccoutand
// Date:         2011-02-03
// Description:  Define interrupt and clock abstractions
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/system.h>
#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>

#include <cyg/hal/hal_io.h>

//==========================================================================
// Exception vectors
//
// These are the common vectors defined by all Microblaze CPUs.  The
// exact number of vectors is variant specific, so the limits will be
// defined in var_intr.h.

#define CYGNUM_HAL_VECTOR_RESET                    0      // Reset entry point
#define CYGNUM_HAL_VECTOR_USER_EXCEPTION           1      // User Exception
#define CYGNUM_HAL_VECTOR_EXTERNAL                 2      // Interrupt
#define CYGNUM_HAL_VECTOR_SOFT_HARD_BREAK          3      // Hardware / Software breakpoint
#define CYGNUM_HAL_VECTOR_HARD_FAULT               4      // Hard fault
#define CYGNUM_HAL_VECTOR_RESERVED_05              5
#define CYGNUM_HAL_VECTOR_RESERVED_06              6
#define CYGNUM_HAL_VECTOR_RESERVED_07              7
#define CYGNUM_HAL_VECTOR_RESERVED_08              8
#define CYGNUM_HAL_VECTOR_RESERVED_09              9


//==========================================================================
// Include variant definitions here.

#include <cyg/hal/var_intr.h>

#ifndef CYGNUM_HAL_INTERRUPT_RTC
# error "Variant / platform must defined the RTC vector number"
#endif

#define CYGNUM_HAL_VSR_MIN                   CYGNUM_HAL_VECTOR_RESET
#ifndef CYGNUM_HAL_VSR_MAX
# define CYGNUM_HAL_VSR_MAX                  CYGNUM_HAL_VECTOR_RESERVED_09
#endif
#define CYGNUM_HAL_VSR_COUNT                 ( CYGNUM_HAL_VSR_MAX + 1 )

// When no interrupt controller is used, ISR min = ISR max = 0
#define CYGNUM_HAL_ISR_MIN                   0
#ifndef CYGNUM_HAL_ISR_MAX
# define CYGNUM_HAL_ISR_MAX                  0
#endif
#define CYGNUM_HAL_ISR_COUNT                 ( CYGNUM_HAL_ISR_MAX + 1 )


//==========================================================================
// Exception vectors.
//
// These are the values used when passed out to an external exception
// handler using cyg_hal_deliver_exception()

// From ESR special register
#define CYGNUM_HAL_EXCEPTION_STREAM                    0
#define CYGNUM_HAL_EXCEPTION_DATA_UNALIGNED_ACCESS     1
#define CYGNUM_HAL_EXCEPTION_OPCODE                    2
#define CYGNUM_HAL_EXCEPTION_INSTRUCTION_BUS           3
#define CYGNUM_HAL_EXCEPTION_DATA_BUS                  4
#define CYGNUM_HAL_EXCEPTION_DIVIDE                    5
#define CYGNUM_HAL_EXCEPTION_FPU                       6
#define CYGNUM_HAL_EXCEPTION_PRIVILEGE                 7
// 8 to 15 not defined
#define CYGNUM_HAL_EXCEPTION_DATA_STORAGE             16
#define CYGNUM_HAL_EXCEPTION_INST_STORAGE             17
#define CYGNUM_HAL_EXCEPTION_DATA_TLBMISS_ACCESS      18
#define CYGNUM_HAL_EXCEPTION_CODE_TLBMISS_ACCESS      19

#define CYGNUM_HAL_EXCEPTION_DATA_ACCESS                CYGNUM_HAL_EXCEPTION_DATA_STORAGE
#define CYGNUM_HAL_EXCEPTION_CODE_ACCESS                CYGNUM_HAL_EXCEPTION_INST_STORAGE
#define CYGNUM_HAL_EXCEPTION_ILLEGAL_INSTRUCTION        CYGNUM_HAL_EXCEPTION_OPCODE

#define CYGNUM_HAL_EXCEPTION_MIN     CYGNUM_HAL_EXCEPTION_STREAM
#define CYGNUM_HAL_EXCEPTION_MAX     CYGNUM_HAL_EXCEPTION_CODE_TLBMISS_ACCESS
#define CYGNUM_HAL_EXCEPTION_COUNT   (CYGNUM_HAL_EXCEPTION_MAX - \
                                      CYGNUM_HAL_EXCEPTION_MIN + 1)


//==========================================================================
// VSR handling

__externC volatile CYG_ADDRESS   hal_vsr_table[CYGNUM_HAL_VSR_COUNT];

#ifndef HAL_VSR_GET
#define HAL_VSR_GET( __vector, __pvsr )                         \
    *(CYG_ADDRESS *)(__pvsr) = hal_vsr_table[__vector];
#endif

#ifndef HAL_VSR_SET
#define HAL_VSR_SET( __vector, __vsr, __poldvsr )               \
CYG_MACRO_START                                                 \
    if( __poldvsr != NULL )                                     \
        *(CYG_ADDRESS *)__poldvsr = hal_vsr_table[__vector];    \
    hal_vsr_table[__vector] = (CYG_ADDRESS)__vsr;               \
CYG_MACRO_END
#endif

#ifndef HAL_VSR_SET_TO_ECOS_HANDLER
__externC void hal_default_interrupt_vsr( void );
__externC void hal_default_exception_vsr( void );
# define HAL_VSR_SET_TO_ECOS_HANDLER( __vector, __poldvsr )     \
CYG_MACRO_START                                                 \
CYG_MACRO_END
#endif

// Default definition of HAL_TRANSLATE_VECTOR(), a no-op
#ifndef HAL_TRANSLATE_VECTOR
# define HAL_TRANSLATE_VECTOR(__vector, __index) ((__index) = (__vector))
#endif

//==========================================================================
// ISR handling
//
// Interrupt handler/data/object tables plus functions and macros to
// manipulate them.
 
__externC volatile CYG_ADDRESS   hal_interrupt_handlers[CYGNUM_HAL_ISR_COUNT];
__externC volatile CYG_ADDRWORD  hal_interrupt_data    [CYGNUM_HAL_ISR_COUNT];
__externC volatile CYG_ADDRESS   hal_interrupt_objects [CYGNUM_HAL_ISR_COUNT];

//--------------------------------------------------------------------------
// Interrupt delivery
//
// This function is used by the HAL to deliver an interrupt, and post
// a DSR if required. It may also be used to deliver secondary
// interrupts from springboard ISRs.

__externC void hal_deliver_interrupt( cyg_uint32 vector );

//--------------------------------------------------------------------------
// Default ISR The #define is used to test whether this routine
// exists, and to allow code outside the HAL to call it.

externC cyg_uint32 hal_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data);
#define HAL_DEFAULT_ISR hal_default_isr

//--------------------------------------------------------------------------

#define HAL_INTERRUPT_IN_USE( _vector_, _state_)                           \
{                                                                          \
    cyg_uint32 _index_;                                                    \
    HAL_TRANSLATE_VECTOR ((_vector_), _index_);                            \
                                                                           \
    if( hal_interrupt_handlers[_index_] == (CYG_ADDRESS)hal_default_isr )  \
        (_state_) = 0;                                                     \
    else                                                                   \
        (_state_) = 1;                                                     \
}

#define HAL_INTERRUPT_ATTACH( _vector_, _isr_, _data_, _object_ )          \
{                                                                          \
    if( hal_interrupt_handlers[_vector_] == (CYG_ADDRESS)hal_default_isr ) \
    {                                                                      \
        hal_interrupt_handlers[_vector_] = (CYG_ADDRESS)_isr_;             \
        hal_interrupt_data[_vector_] = (CYG_ADDRWORD) _data_;              \
        hal_interrupt_objects[_vector_] = (CYG_ADDRESS)_object_;           \
    }                                                                      \
}

#define HAL_INTERRUPT_DETACH( _vector_, _isr_ )                            \
{                                                                          \
    if( hal_interrupt_handlers[_vector_] == (CYG_ADDRESS)_isr_ )           \
    {                                                                      \
        hal_interrupt_handlers[_vector_] = (CYG_ADDRESS)hal_default_isr;   \
        hal_interrupt_data[_vector_] = 0;                                  \
        hal_interrupt_objects[_vector_] = 0;                               \
    }                                                                      \
}

//--------------------------------------------------------------------------
// CPU interrupt control.
//

#ifndef __ASSEMBLER__
typedef cyg_uint32  CYG_INTERRUPT_STATE;
#endif

#ifndef HAL_DISABLE_INTERRUPTS
# define HAL_DISABLE_INTERRUPTS(__old)          \
    __asm__ volatile (                          \
        "msrclr %0, %1                  \n"     \
        "andi   %0, %0, %1              \n"     \
        : "=r" (__old)                          \
        :  "I" (CYGARC_REG_MSR_IE)              \
        );
#endif

#ifndef HAL_RESTORE_INTERRUPTS
# define HAL_RESTORE_INTERRUPTS(__old)          \
    CYG_MACRO_START                             \
    cyg_uint32 tmp1;                            \
    cyg_uint32 tmp2 = ~CYGARC_REG_MSR_IE;       \
    __asm__ volatile (                          \
        "mfs   %0, rmsr                 \n"     \
        "and   %0,%0,%1                 \n"     \
        "andn  %1,%2,%1                 \n"     \
        "or    %0,%0,%1                 \n"     \
        "mts   rmsr, %0                 \n"     \
        : "=&r" (tmp1), "+r" (tmp2)             \
        : "r" (__old)                           \
        );                                      \
    CYG_MACRO_END
#endif

#ifndef HAL_ENABLE_INTERRUPTS
# define HAL_ENABLE_INTERRUPTS()                \
    cyg_uint32 tmp;                             \
    __asm__ volatile (                          \
        "msrset %0, %1                  \n"     \
        : "=r" (tmp)                            \
        :  "I" (CYGARC_REG_MSR_IE)              \
        );
#endif

#ifndef HAL_QUERY_INTERRUPTS
# define HAL_QUERY_INTERRUPTS(__state)          \
    __asm__ volatile (                          \
        "mfs  %0, rmsr                  \n"     \
        "andi %0,%0,%1                  \n"     \
        : "=&r" (__state)                       \
        :  "I"  (CYGARC_REG_MSR_IE)             \
        );
#endif

//--------------------------------------------------------------------------
// Interrupt masking and unmasking
//
// This is mostly done via the architecture defined NVIC. The
// HAL_VAR_*() macros allow the variant HAL to provide extended
// support for additional interrupt sources supported by supplementary
// interrupt controllers.

__externC void hal_interrupt_mask( cyg_uint32 vector );
__externC void hal_interrupt_unmask( cyg_uint32 vector );
__externC void hal_interrupt_set_level( cyg_uint32 vector, cyg_uint32 level );
__externC void hal_interrupt_acknowledge( cyg_uint32 vector );
__externC void hal_interrupt_configure( cyg_uint32 vector, cyg_uint32 level, cyg_uint32 up );


#define HAL_INTERRUPT_MASK( __vector ) hal_interrupt_mask( __vector )
#define HAL_INTERRUPT_UNMASK( __vector ) hal_interrupt_unmask( __vector )
#define HAL_INTERRUPT_SET_LEVEL( __vector, __level ) hal_interrupt_set_level( __vector, __level )
#define HAL_INTERRUPT_ACKNOWLEDGE( __vector ) hal_interrupt_acknowledge( __vector )
#define HAL_INTERRUPT_CONFIGURE( __vector, __level, __up ) hal_interrupt_configure( __vector, __level, __up )

//--------------------------------------------------------------------------
// Routine to execute DSRs using separate interrupt stack

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK

externC void hal_interrupt_stack_call_pending_DSRs(void);
#define HAL_INTERRUPT_STACK_CALL_PENDING_DSRS() \
              hal_interrupt_stack_call_pending_DSRs()

#endif

//==========================================================================
// Clock control - Not at core level. The core does not feature any
// RTC capability yet.
//

#ifndef CYGHWR_HAL_CLOCK_DEFINED
# error "HAL does not implement the required clock macros"
#endif

//==========================================================================
// HAL_DELAY_US().
//

__externC void hal_delay_us( cyg_int32 us );
#define HAL_DELAY_US( __us ) hal_delay_us( __us )

//==========================================================================
// Reset- Not at core level, this must be platform dependent
//

#ifndef CYGHWR_HAL_PLATFORM_RESET_DEFINED

#define HAL_PLATFORM_RESET()                                            \
{                                                                       \
    for(;;);                                                            \
}

#endif

__externC void hal_reset_vsr( void );
#define HAL_PLATFORM_RESET_ENTRY &hal_reset_vsr

//==========================================================================
#endif //CYGONCE_HAL_INTR_H

