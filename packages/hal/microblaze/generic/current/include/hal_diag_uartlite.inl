/*=============================================================================
//
//      hal_diag.inl
//
//      HAL diagnostic output routine for UART Lite IP block
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
// Purpose:     HAL diagnostic output
// Description: HAL diagnostic output routine for UART Lite IP block
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

#ifndef CYGONCE_HAL_MICROBLAZE_DEBUG_CHANNEL_INL
#define CYGONCE_HAL_MICROBLAZE_DEBUG_CHANNEL_INL

static channel_data_t mb_ser_channels[] = {
#ifdef CYGPKG_HAL_MICROBLAZE_UART0
    { 0, CYGPKG_HAL_MICROBLAZE_UART0_BASE, 1000, \
      CYGNUM_HAL_MICROBLAZE_UART0_IRQNUM, CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD },
#endif
#ifdef CYGPKG_HAL_MICROBLAZE_UART1
    { 1, CYGPKG_HAL_MICROBLAZE_UART1_BASE, 1000, \
      CYGNUM_HAL_MICROBLAZE_UART1_IRQNUM, CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD },
#endif
};

//-----------------------------------------------------------------------------

static void
hal_mb_serial_init_channel(void* __ch_data)
{
    channel_data_t *chan = (channel_data_t*)__ch_data;
    CYG_ADDRESS base = chan->base;
    cyg_uint32 c = CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_RST_TXFIFO | CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_RST_RXFIFO;

    // Reset FIFOs
     HAL_WRITE_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_CTRL, c);

    return;
}

void
hal_mb_serial_putc(void *__ch_data, char c)
{
    CYG_ADDRESS base = ((channel_data_t*)__ch_data)->base;
    cyg_uint32 sr;
    CYGARC_HAL_SAVE_GP();

     do {
         HAL_READ_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_STAT, sr);
     } while ( sr & CYGHWR_HAL_MB_XPS_UARTLITE_STAT_TX_FFULL );

     HAL_WRITE_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_TX_FIFO, c);

    CYGARC_HAL_RESTORE_GP();
}

static cyg_bool
hal_mb_serial_getc_nonblock(void* __ch_data, cyg_uint8* ch)
{
    CYG_ADDRESS base = ((channel_data_t*)__ch_data)->base;
    cyg_uint32  sr;
    cyg_uint32  c;
    CYGARC_HAL_SAVE_GP();

    HAL_READ_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_STAT , sr);

    if( (sr & CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_FVD) == 0 )
        return false;

    HAL_READ_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_RX_FIFO, c);

    *ch = (cyg_uint8)c;

    CYGARC_HAL_RESTORE_GP();

    return true;
}

//=============================================================================
// Virtual vector HAL diagnostics

#if defined(CYGSEM_HAL_VIRTUAL_VECTOR_DIAG)

static int
hal_mb_serial_control(void *__ch_data, __comm_control_cmd_t __func, ...)
{
    channel_data_t* chan = (channel_data_t*)__ch_data;
    CYG_ADDRESS base = ((channel_data_t*)__ch_data)->base;
    int ret = 0;
    cyg_uint32 cr1;

    va_list ap;

    CYGARC_HAL_SAVE_GP();

    va_start(ap, __func);

    switch (__func) {
    case __COMMCTL_IRQ_ENABLE:
        chan->irq_state = 1;
        HAL_INTERRUPT_ACKNOWLEDGE( chan->isr_vector );
        HAL_INTERRUPT_UNMASK( chan->isr_vector );
        HAL_READ_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_CTRL, cr1);
        cr1 |= CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_IRQ_EN;
        HAL_WRITE_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_CTRL, cr1);
        break;
    case __COMMCTL_IRQ_DISABLE:
        ret = chan->irq_state;
        chan->irq_state = 0;
        HAL_INTERRUPT_MASK( chan->isr_vector );
        HAL_READ_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_CTRL, cr1);
        cr1 &= ~CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_IRQ_EN;
        HAL_WRITE_UINT32(base + CYGHWR_HAL_MB_XPS_UARTLITE_CTRL, cr1);
        break;
    case __COMMCTL_DBG_ISR_VECTOR:
        ret = chan->isr_vector;
        break;
    case __COMMCTL_SET_TIMEOUT:
    {
        va_list ap;

        va_start(ap, __func);

        ret = chan->msec_timeout;
        chan->msec_timeout = va_arg(ap, cyg_uint32);

        va_end(ap);
    }        
    case __COMMCTL_GETBAUD:
        ret = chan->baud_rate;
        break;
    case __COMMCTL_SETBAUD:
        chan->baud_rate = va_arg(ap, cyg_int32);
        // UART Lite has fixed baud rate
        ret = 0;
        break;
    default:
        break;
    }
    va_end(ap);
    CYGARC_HAL_RESTORE_GP();
    return ret;
}

#endif

#endif // CYGONCE_HAL_MICROBLAZE_DEBUG_CHANNEL_INL

//-----------------------------------------------------------------------------
// End of hal_diag.inl
