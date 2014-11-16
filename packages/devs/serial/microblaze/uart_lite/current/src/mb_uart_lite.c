//==========================================================================
//
//      mb_uart_lite.c
//
//      Serial driver
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
// Contributors:
// Date:         2011-03-05
// Purpose:
// Description:  Hardware driver for Microblaze processor
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/io_serial.h>
#include <pkgconf/io.h>

#include <cyg/io/io.h>
#include <cyg/hal/hal_intr.h>

// Register definition are in var_io.h but then it creates
// a dependence towards the variant HAL, should probably be removed
#include <cyg/hal/var_io.h>

#include <cyg/io/devtab.h>
#include <cyg/infra/diag.h>
#include <cyg/io/serial.h>

typedef struct mb_serial_info {
    CYG_ADDRWORD   base;
    CYG_WORD       int_num;
    cyg_interrupt  serial_interrupt;
    cyg_handle_t   serial_interrupt_handle;
    cyg_uint32     tx_pending;
} mb_serial_info;

static bool mb_serial_init(struct cyg_devtab_entry *tab);
static bool mb_serial_putc(serial_channel *chan, unsigned char c);
static Cyg_ErrNo mb_serial_lookup(struct cyg_devtab_entry **tab,
                                   struct cyg_devtab_entry *sub_tab,
                                   const char *name);
static unsigned char mb_serial_getc(serial_channel *chan);
static Cyg_ErrNo mb_serial_set_config(serial_channel *chan, cyg_uint32 key,
                                          const void *xbuf, cyg_uint32 *len);
static void mb_serial_start_xmit(serial_channel *chan);
static void mb_serial_stop_xmit(serial_channel *chan);

static cyg_uint32 mb_serial_ISR(cyg_vector_t vector, cyg_addrword_t data);
static void       mb_serial_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data);

static SERIAL_FUNS(mb_serial_funs,
                   mb_serial_putc,
                   mb_serial_getc,
                   mb_serial_set_config,
                   mb_serial_start_xmit,
                   mb_serial_stop_xmit
    );

#ifdef CYGPKG_IO_SERIAL_MICROBLAZE_SERIAL_A

static mb_serial_info mb_serial_info0 ={CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_A_IOBASE,
           CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_A_INT};
#if CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_A_BUFSIZE > 0
static unsigned char mb_serial_out_buf0[CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_A_BUFSIZE];
static unsigned char mb_serial_in_buf0[CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_A_BUFSIZE];

static SERIAL_CHANNEL_USING_INTERRUPTS(mb_serial_channel0,
                                       mb_serial_funs,
                                       mb_serial_info0,
                                       0, // N/A
                                       CYG_SERIAL_STOP_DEFAULT,
                                       CYG_SERIAL_PARITY_DEFAULT,
                                       CYG_SERIAL_WORD_LENGTH_DEFAULT,
                                       CYG_SERIAL_FLAGS_DEFAULT,
                                       &mb_serial_out_buf0[0],
                                       sizeof(mb_serial_out_buf0),
                                       &mb_serial_in_buf0[0],
                                       sizeof(mb_serial_in_buf0)
    );
#else
static SERIAL_CHANNEL(mb_serial_channel0,
                      mb_serial_funs,
                      mb_serial_info0,
                      CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_A_BAUD),
                      CYG_SERIAL_STOP_DEFAULT,
                      CYG_SERIAL_PARITY_DEFAULT,
                      CYG_SERIAL_WORD_LENGTH_DEFAULT,
                      CYG_SERIAL_FLAGS_DEFAULT
    );
#endif

DEVTAB_ENTRY(mb_serial_io0,
             CYGDAT_IO_SERIAL_MICROBLAZE_SERIAL_A_NAME,
             0,                 // Does not depend on a lower level interface
             &cyg_io_serial_devio,
             mb_serial_init,
             mb_serial_lookup,     // Serial driver may need initializing
             &mb_serial_channel0
    );
#endif

#ifdef CYGPKG_IO_SERIAL_MICROBLAZE_SERIAL_B
static mb_serial_info mb_serial_info1 ={CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_B_IOBASE,
               CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_B_INT};
#if CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_B_BUFSIZE > 0
static unsigned char mb_serial_out_buf1[CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_B_BUFSIZE];
static unsigned char mb_serial_in_buf1[CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_B_BUFSIZE];

static SERIAL_CHANNEL_USING_INTERRUPTS(mb_serial_channel1,
                                       mb_serial_funs,
                                       mb_serial_info1,
                                       0, // N/A
                                       CYG_SERIAL_STOP_DEFAULT,
                                       CYG_SERIAL_PARITY_DEFAULT,
                                       CYG_SERIAL_WORD_LENGTH_DEFAULT,
                                       CYG_SERIAL_FLAGS_DEFAULT,
                                       &mb_serial_out_buf1[0],
                                       sizeof(mb_serial_out_buf1),
                                       &mb_serial_in_buf1[0],
                                       sizeof(mb_serial_in_buf1)
    );
#else
static SERIAL_CHANNEL(mb_serial_channel1,
                      mb_serial_funs,
                      mb_serial_info1,
                      CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_MICROBLAZE_SERIAL_B_BAUD),
                      CYG_SERIAL_STOP_DEFAULT,
                      CYG_SERIAL_PARITY_DEFAULT,
                      CYG_SERIAL_WORD_LENGTH_DEFAULT,
                      CYG_SERIAL_FLAGS_DEFAULT
    );
#endif

DEVTAB_ENTRY(mb_serial_io1,
             CYGDAT_IO_SERIAL_MICROBLAZE_SERIAL_B_NAME,
             0,                   // Does not depend on a lower level interface
             &cyg_io_serial_devio,
             mb_serial_init,
             mb_serial_lookup,     // Serial driver may need initializing
             &mb_serial_channel1
    );
#endif


// Internal function to actually configure the hardware to desired baud rate, etc.
static bool
mb_serial_config_port(serial_channel *chan, cyg_serial_info_t *new_config, bool init)
{
    mb_serial_info *mb_chan = (mb_serial_info *)chan->dev_priv;
    cyg_addrword_t port = mb_chan->base;
    cyg_uint32 cr1 = ( CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_RST_TXFIFO | CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_RST_RXFIFO | CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_IRQ_EN );

    if (init) {
        mb_chan->tx_pending = 0;
        // Reset and Enable interrupt
        HAL_WRITE_UINT32(port + CYGHWR_HAL_MB_XPS_UARTLITE_CTRL, cr1);
    }

    if (new_config != &chan->config) {
        chan->config = *new_config;
    }
    return true;
}

// Function to initialize the device.  Called at bootstrap time.
static bool
mb_serial_init(struct cyg_devtab_entry *tab)
{
    serial_channel *chan = (serial_channel *)tab->priv;
    mb_serial_info *mb_chan = (mb_serial_info *)chan->dev_priv;
#ifdef CYGDBG_IO_INIT
    diag_printf("mb SERIAL init - dev: %x.%d\n", mb_chan->base, mb_chan->int_num);
#endif
    (chan->callbacks->serial_init)(chan);  // Really only required for interrupt driven devices
    if (chan->out_cbuf.len != 0) {
        cyg_drv_interrupt_create(mb_chan->int_num,
                                 8,         // can change IRQ0 priority
                                 (cyg_addrword_t)chan,   //  Data item passed to interrupt handler
                                 mb_serial_ISR,
                                 mb_serial_DSR,
                                 &mb_chan->serial_interrupt_handle,
                                 &mb_chan->serial_interrupt);
        cyg_drv_interrupt_attach(mb_chan->serial_interrupt_handle);
        cyg_drv_interrupt_unmask(mb_chan->int_num);
    }
    mb_serial_config_port(chan, &chan->config, true);
    return true;
}

// This routine is called when the device is "looked" up (i.e. attached)
static Cyg_ErrNo
mb_serial_lookup(struct cyg_devtab_entry **tab,
                  struct cyg_devtab_entry *sub_tab,
                  const char *name)
{
    serial_channel *chan = (serial_channel *)(*tab)->priv;
    (chan->callbacks->serial_init)(chan);  // Really only required for interrupt driven devices
    return ENOERR;
}

// Send a character to the device output buffer.
// Return 'true' if character is sent to device
static bool
mb_serial_putc(serial_channel *chan, unsigned char c)
{
    mb_serial_info *mb_chan = (mb_serial_info *)chan->dev_priv;
    cyg_addrword_t port = mb_chan->base;
    cyg_uint32 _sr;

    HAL_READ_UINT32(port + CYGHWR_HAL_MB_XPS_UARTLITE_STAT, _sr);
    if ( (_sr & CYGHWR_HAL_MB_XPS_UARTLITE_STAT_TX_FFULL) == 0 ) {
// Transmit buffer is empty
        cyg_drv_dsr_lock();
        HAL_WRITE_UINT32(port + CYGHWR_HAL_MB_XPS_UARTLITE_TX_FIFO, c);
        mb_chan->tx_pending = 1;
        cyg_drv_dsr_unlock();
        return true;
    } else {
// No space
        return false;
    }
}

// Fetch a character from the device input buffer, waiting if necessary
static unsigned char
mb_serial_getc(serial_channel *chan)
{
    unsigned char c;
    mb_serial_info *mb_chan = (mb_serial_info *)chan->dev_priv;
    cyg_addrword_t port = mb_chan->base;
    cyg_uint8 _sr;

    do {
        HAL_READ_UINT32(port + CYGHWR_HAL_MB_XPS_UARTLITE_STAT , _sr);
    } while ((_sr & CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_FVD) == 0);

    HAL_READ_UINT32(port + CYGHWR_HAL_MB_XPS_UARTLITE_RX_FIFO, c);
    return c;
}

// Set up the device characteristics; baud rate, etc.
static Cyg_ErrNo
mb_serial_set_config(serial_channel *chan, cyg_uint32 key,
                         const void *xbuf, cyg_uint32 *len)
{
    switch (key) {
    case CYG_IO_SET_CONFIG_SERIAL_INFO:
      {
        cyg_serial_info_t *config = (cyg_serial_info_t *)xbuf;
        if ( *len < sizeof(cyg_serial_info_t) ) {
            return -EINVAL;
        }
        *len = sizeof(cyg_serial_info_t);
        if ( true != mb_serial_config_port(chan, config, false) )
            return -EINVAL;
      }
      break;
    default:
        return -EINVAL;
    }
    return ENOERR;
}

// Enable the transmitter on the device
static void
mb_serial_start_xmit(serial_channel *chan)
{
     // Can't stop/start the TX interrupt, it is the same as the RX interrupt
    (chan->callbacks->xmt_char)(chan);
}

// Disable the transmitter on the device
static void
mb_serial_stop_xmit(serial_channel *chan)
{
    // Can't stop/start the TX interrupt, it is the same as the RX interrupt
}

// Serial I/O - low level interrupt handler (ISR)
static cyg_uint32
mb_serial_ISR(cyg_vector_t vector, cyg_addrword_t data)
{
    serial_channel *chan = (serial_channel *)data;
    mb_serial_info *mb_chan = (mb_serial_info *)chan->dev_priv;
    cyg_drv_interrupt_mask(mb_chan->int_num);
    cyg_drv_interrupt_acknowledge(mb_chan->int_num);
    return CYG_ISR_CALL_DSR;  // Cause DSR to be run
}

// Serial I/O - high level interrupt handler (DSR)
static void
mb_serial_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
{
    serial_channel *chan = (serial_channel *)data;
    mb_serial_info *mb_chan = (mb_serial_info *)chan->dev_priv;
    cyg_addrword_t port = mb_chan->base;
    cyg_uint32 _stats;

    HAL_READ_UINT32(port+CYGHWR_HAL_MB_XPS_UARTLITE_STAT, _stats);

    if ( mb_chan->tx_pending & ( CYGHWR_HAL_MB_XPS_UARTLITE_STAT_TX_FEMPTY & _stats ) ) {
         mb_chan->tx_pending = 0;
        (chan->callbacks->xmt_char)(chan);
    }
    if ( CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_FVD & _stats ) {
        cyg_uint32 _c;
        HAL_READ_UINT32(port+CYGHWR_HAL_MB_XPS_UARTLITE_RX_FIFO, _c);
        (chan->callbacks->rcv_char)(chan, (cyg_uint8)_c);
    }
    cyg_drv_interrupt_unmask(mb_chan->int_num);

}

// EOF mb_uart_lite.c
