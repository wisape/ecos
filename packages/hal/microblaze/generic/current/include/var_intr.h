//=============================================================================
//
//     var_intr.h
//
//     Microblaze variant interrupt and clock abstractions
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   ccoutand
// Date:        2011-02-01
// Purpose:     Microblaze variant specific interrupt and clock abstractions
// Description:
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/var_io.h>             // IO macros

#include <cyg/hal/plf_intr.h>           // Platform Interrupt


//==========================================================================
// Clock control
//
// RTC based on XPS timer block

#ifdef CYGPKG_HAL_MICROBLAZE_RTC_BLOCK_XPS_TIMER

#define CYGHWR_HAL_CLOCK_DEFINED
#define CYGHWR_HAL_MB_RTC_TIMER_BASE   (CYGPKG_HAL_MICROBLAZE_RTC_BLOCK_BASE + (0x10 * CYGNUM_HAL_MICROBLAZE_RTC_BLOCK_ID))
#define CYGNUM_HAL_INTERRUPT_RTC        CYGNUM_HAL_MICROBLAZE_RTC_IRQNUM

externC cyg_uint32 _period;

#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_INIT               \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_UP   |       \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_EN   |       \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_ENI  |       \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_ARHT


#define HAL_CLOCK_INITIALIZE( _period_ )                                                 \
{                                                                                        \
    cyg_uint32 __p = 0;                                                                  \
    _period = _period_;                                                                  \
    __p = CYGPKG_HAL_MICROBLAZE_RTC_BLOCK_FREQ / ( 1000000 / _period ) - 1;              \
    HAL_WRITE_UINT32(CYGHWR_HAL_MB_RTC_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TLR,           \
                               __p );                                                    \
    HAL_WRITE_UINT32(CYGHWR_HAL_MB_RTC_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TCSR,          \
                            CYGHWR_HAL_MB_XPS_TIMER_TCSR_LOAD );                         \
    HAL_WRITE_UINT32(CYGHWR_HAL_MB_RTC_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TCSR,          \
                            CYGHWR_HAL_MB_XPS_TIMER_TCSR_INIT );                         \
}

#define HAL_CLOCK_RESET( __vector, __period )                                            \
{                                                                                        \
    HAL_WRITE_UINT32(CYGHWR_HAL_MB_RTC_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TCSR,          \
                          CYGHWR_HAL_MB_XPS_TIMER_TCSR_INIT |                            \
                          CYGHWR_HAL_MB_XPS_TIMER_TCSR_TOIN );                           \
    if (_period != __period) {                                                           \
        HAL_CLOCK_INITIALIZE(__period);                                                  \
    }                                                                                    \
    _period = __period;                                                                  \
}

#define HAL_CLOCK_READ( _pvalue_ )                                                       \
{                                                                                        \
    cyg_uint32 __per, __value;                                                           \
    HAL_READ_UINT32(CYGHWR_HAL_MB_RTC_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TLR, __per );   \
    HAL_READ_UINT32(CYGHWR_HAL_MB_RTC_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TCR, __value ); \
    __value = ( __per + 1 ) - __value;                                                   \
    __value /= (CYGPKG_HAL_MICROBLAZE_RTC_BLOCK_FREQ / 1000000 );                        \
   *(_pvalue_) = __value;                                                                \
}

#define HAL_CLOCK_LATENCY( __pvalue ) HAL_CLOCK_READ( __pvalue )



#endif


//==========================================================================
// Interrupt mask and config for variant-specific devices
//

#ifdef CYGPKG_HAL_MICROBLAZE_IPC

// Interrupt controller based on Xilinx IPC
# ifdef CYGPKG_HAL_MICROBLAZE_IPC_BLOCK_XPS_INTC

#define CYGHWR_HAL_MB_XPS_INTC_BASE CYGPKG_HAL_MICROBLAZE_IPC_BLOCK_BASE

// CIE is a location used to clear IER bits in a single atomic operation,
// rather than using a read/modify/write sequence

#define HAL_VAR_INTERRUPT_MASK( __vector )                                              \
{                                                                                       \
    cyg_uint32 __v = BIT_(__vector);                                                    \
                                                                                        \
    HAL_WRITE_UINT32( CYGHWR_HAL_MB_XPS_INTC_BASE + CYGHWR_HAL_MB_XPS_INTC_CIE, __v );  \
}

// SIE is a location used to set IER bits in a single atomic operation,
// rather than using a read/modify/write sequence

#define HAL_VAR_INTERRUPT_UNMASK( __vector )                                            \
{                                                                                       \
    cyg_uint32 __v = BIT_(__vector);                                                    \
                                                                                        \
    HAL_WRITE_UINT32( CYGHWR_HAL_MB_XPS_INTC_BASE + CYGHWR_HAL_MB_XPS_INTC_SIE, __v );  \
}

// This is an HW configuration, no SW required

#define HAL_VAR_INTERRUPT_SET_LEVEL( __vector, __level )                                \
{                                                                                       \
}

#define HAL_VAR_INTERRUPT_ACKNOWLEDGE( __vector )                                       \
{                                                                                       \
    cyg_uint32 __v = BIT_(__vector);                                                    \
                                                                                        \
    HAL_WRITE_UINT32( CYGHWR_HAL_MB_XPS_INTC_BASE + CYGHWR_HAL_MB_XPS_INTC_IAR, __v );  \
}

#define HAL_VAR_INTERRUPT_CONFIGURE( __vector, __level, __up )                          \
{                                                                                       \
}

# endif

#endif // CYGPKG_HAL_MICROBLAZE_IP

// Can be overwritten by the platform HAL
#ifndef CYGNUM_HAL_ISR_MAX
# define CYGNUM_HAL_ISR_MAX                  CYGNUM_HAL_MICROBLAZE_IPC_MAXIRQ
#endif

// End of var_intr.h
