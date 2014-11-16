/*=============================================================================
//
//      hal_diag.c
//
//      HAL diagnostic output code
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
// Original(s): nickg for STM32 HAL
// Date:        2011-02-01
// Purpose:     HAL diagnostic output
// Description: Implementations of HAL diagnostic output support.
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

#include <pkgconf/hal.h>
#include CYGBLD_HAL_VARIANT_H

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing

#include <cyg/hal/hal_arch.h>           // SAVE/RESTORE GP macros
#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_if.h>             // interface API
#include <cyg/hal/hal_intr.h>           // HAL_ENABLE/MASK/UNMASK_INTERRUPTS
#include <cyg/hal/hal_misc.h>           // Helper functions
#include <cyg/hal/drv_api.h>            // CYG_ISR_HANDLED

#include <cyg/hal/var_io.h>             // USART registers

//-----------------------------------------------------------------------------

typedef struct {
    cyg_uint32          uart;
    CYG_ADDRESS         base;
    cyg_int32           msec_timeout;
    int                 isr_vector;
    cyg_uint32          baud_rate;
    int                 irq_state;
} channel_data_t;


// Include the HW specific code here
#if   defined(CYGPKG_HAL_MICROBLAZE_UART_BLOCK_XPS_UARTLITE)
# include <cyg/hal/hal_diag_uartlite.inl>
#elif defined(CYGPKG_HAL_MICROBLAZE_UART_BLOCK_XPS_UART16550)
# include <cyg/hal/hal_diag_uart16550.inl>
#endif

#ifndef CYGONCE_HAL_MICROBLAZE_DEBUG_CHANNEL_INL
# error "Diag Hardware interface not defined"
#endif

cyg_uint8
hal_mb_serial_getc(void* __ch_data)
{
    cyg_uint8 ch;
    CYGARC_HAL_SAVE_GP();

    while(!hal_mb_serial_getc_nonblock(__ch_data, &ch));

    CYGARC_HAL_RESTORE_GP();

    return ch;
}

//=============================================================================
// Virtual vector HAL diagnostics

#if defined(CYGSEM_HAL_VIRTUAL_VECTOR_DIAG)

static void
hal_mb_serial_write(void* __ch_data, const cyg_uint8* __buf,
                         cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    while(__len-- > 0)
        hal_mb_serial_putc(__ch_data, *__buf++);

    CYGARC_HAL_RESTORE_GP();
}

static void
hal_mb_serial_read(void* __ch_data, cyg_uint8* __buf, cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    while(__len-- > 0)
        *__buf++ = hal_mb_serial_getc(__ch_data);

    CYGARC_HAL_RESTORE_GP();
}

cyg_bool
hal_mb_serial_getc_timeout(void* __ch_data, cyg_uint8* ch)
{
    int delay_count;
    channel_data_t* chan = (channel_data_t*)__ch_data;
    cyg_bool res;
    CYGARC_HAL_SAVE_GP();

    delay_count = chan->msec_timeout * 100; // delay in 10 us steps

    for(;;) {
        res = hal_mb_serial_getc_nonblock(__ch_data, ch);
        if (res || 0 == delay_count--)
            break;
        
        CYGACC_CALL_IF_DELAY_US(10);
    }

    CYGARC_HAL_RESTORE_GP();
    return res;
}

static int
hal_mb_serial_isr(void *__ch_data, int* __ctrlc,
                       CYG_ADDRWORD __vector, CYG_ADDRWORD __data)
{
    channel_data_t* chan = (channel_data_t*)__ch_data;
    cyg_uint8 ch;

    CYGARC_HAL_SAVE_GP();

     *__ctrlc = 0;

     if( hal_mb_serial_getc_nonblock(__ch_data, &ch) )
     {
         if( cyg_hal_is_break( (char *)&ch , 1 ) )
             *__ctrlc = 1; 
     }

    HAL_INTERRUPT_ACKNOWLEDGE(chan->isr_vector);

    CYGARC_HAL_RESTORE_GP();
    return 1;
}

static void
hal_mb_serial_init(void)
{
    hal_virtual_comm_table_t* comm;
    int cur;
    int i;

    cur = CYGACC_CALL_IF_SET_CONSOLE_COMM(CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);

    for( i = 0; i < CYGNUM_HAL_VIRTUAL_VECTOR_COMM_CHANNELS ; i++ )
    {
        hal_mb_serial_init_channel(&mb_ser_channels[i]);

        CYGACC_CALL_IF_SET_CONSOLE_COMM(i);
        comm = CYGACC_CALL_IF_CONSOLE_PROCS();
        CYGACC_COMM_IF_CH_DATA_SET(*comm, &mb_ser_channels[i]);
        CYGACC_COMM_IF_WRITE_SET(*comm, hal_mb_serial_write);
        CYGACC_COMM_IF_READ_SET(*comm, hal_mb_serial_read);
        CYGACC_COMM_IF_PUTC_SET(*comm, hal_mb_serial_putc);
        CYGACC_COMM_IF_GETC_SET(*comm, hal_mb_serial_getc);
        CYGACC_COMM_IF_CONTROL_SET(*comm, hal_mb_serial_control);
        CYGACC_COMM_IF_DBG_ISR_SET(*comm, hal_mb_serial_isr);
        CYGACC_COMM_IF_GETC_TIMEOUT_SET(*comm, hal_mb_serial_getc_timeout);
    }

    // Restore original console
    CYGACC_CALL_IF_SET_CONSOLE_COMM(cur);

    // set debug channel baud rate if different
#if (CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD != CYGNUM_HAL_VIRTUAL_VECTOR_DEBUG_CHANNEL_BAUD)
   mb_ser_channels[CYGNUM_HAL_VIRTUAL_VECTOR_DEBUG_CHANNEL]->baud_rate =
        CYGNUM_HAL_VIRTUAL_VECTOR_DEBUG_CHANNEL_BAUD;
    update_baud_rate( &mb_ser_channels[CYGNUM_HAL_VIRTUAL_VECTOR_DEBUG_CHANNEL] );
#endif

}

void
cyg_hal_plf_comms_init(void)
{
    static int initialized = 0;

    if (initialized)
        return;

    initialized = 1;

    hal_mb_serial_init();
}

#endif

//=============================================================================
// Non-Virtual vector HAL diagnostics

#if !defined(CYGSEM_HAL_VIRTUAL_VECTOR_DIAG)

void
hal_mb_diag_init(void)
{
    hal_mb_serial_init( &mb_ser_channels[CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL] );
}

void
hal_mb_diag_putc(char c)
{
    hal_mb_serial_putc( &mb_ser_channels[CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL], c);
}

cyg_uint8
hal_mb_diag_getc(void)
{
    return hal_mb_serial_getc( &mb_ser_channels[CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL] );
}

#endif

//-----------------------------------------------------------------------------
// End of hal_diag.c
