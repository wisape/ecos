#ifndef CYGONCE_HAL_VAR_LL_FIFO_H
#define CYGONCE_HAL_VAR_LL_FIFO_H
//=============================================================================
//
//      var_ll_fifo.h
//
//      Local Link FIFO registers
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2012 Free Software Foundation, Inc.
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
// Date:        2012-02-15
// Purpose:     Registers and helper macro definition to access
//              Xilinx Local Link FIFO
// Description:
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

//-------------------------------------------------------------
// LL FIFO registers
//
#define CYGHWR_HAL_MB_XPS_LL_FIFO_ISR                 0x00
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IER                 0x04
#define CYGHWR_HAL_MB_XPS_LL_FIFO_TDFR                0x08
#define CYGHWR_HAL_MB_XPS_LL_FIFO_TDFV                0x0C
#define CYGHWR_HAL_MB_XPS_LL_FIFO_TDFD                0x10
#define CYGHWR_HAL_MB_XPS_LL_FIFO_TLF                 0x14
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RDFR                0x18
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RDFO                0x1C
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RDFD                0x20
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RLF                 0x24
#define CYGHWR_HAL_MB_XPS_LL_FIFO_LLR                 0x28

// Interrupt (ISR / IER ) register
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPUR            BIT_(31)   // Receive Packet Underrun Read bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPOR            BIT_(30)   // Receive Packet Overrun bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPU             BIT_(29)   // Receive Packet Underrun bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TPO             BIT_(28)   // Transmit Packet Overrun bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TC              BIT_(27)   // Transmit Complete bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RC              BIT_(26)   // Receive Complete bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TS              BIT_(25)   // Transmit Size bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TR              BIT_(24)   // Transmit Reset bit
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RR              BIT_(23)   // Receive reset bit

// Interrupt mask
#define CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_MASK                    \
                       ( CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPUR | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPOR | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPU  | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TPO  | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TC   | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RC   | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TS   | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TR   | \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RR )

// Transmit Data FIFO Reset Register
#define CYGHWR_HAL_MB_XPS_LL_FIFO_TDFR_RESET_KEY      0xA5

// Transmit Data vacancy
#define CYGHWR_HAL_MB_XPS_LL_FIFO_TDFV_MASK           0x01FF

// Receive Data FIFO Reset Register
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RDFR_RESET_KEY      0xA5

// Receive Data vacancy
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RDFV_MASK           0x01FF

// Local Link FIFO Reset Register
#define CYGHWR_HAL_MB_XPS_LL_FIFO_LLR_RESET_KEY       0xA5

//-------------------------------------------------------------
// LL FIFO helper functions
//
#define CYGHWR_HAL_MB_XPS_LL_FIFO_RESET_RX( base )                     \
{                                                                      \
  HAL_WRITE_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_RDFR,             \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_RDFR_RESET_KEY );   \
}

#define CYGHWR_HAL_MB_XPS_LL_FIFO_RESET_TX( base )                     \
{                                                                      \
  HAL_WRITE_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_TDFR,             \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_TDFR_RESET_KEY );   \
}

#define CYGHWR_HAL_MB_XPS_LL_FIFO_INIT( base )                         \
{                                                                      \
  CYGHWR_HAL_MB_XPS_LL_FIFO_RESET_RX( base );                          \
  CYGHWR_HAL_MB_XPS_LL_FIFO_RESET_TX( base );                          \
  HAL_WRITE_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_LLR,              \
                         CYGHWR_HAL_MB_XPS_LL_FIFO_LLR_RESET_KEY );    \
}

#define CYGHWR_HAL_MB_XPS_LL_FIFO_GET_TX_WORD_VACANCY( base )          \
({                                                                     \
  cyg_uint32 _d;                                                       \
  HAL_READ_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_TDFV, _d );        \
  _d;                                                                  \
})

#define CYGHWR_HAL_MB_XPS_LL_FIFO_SET_TX_BYTE_COUNT( base , cnt )      \
{                                                                      \
  HAL_WRITE_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_TLF, cnt );       \
}

#define CYGHWR_HAL_MB_XPS_LL_FIFO_WRITE_WORD( base , data )            \
{                                                                      \
  HAL_WRITE_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_TDFD, data );     \
}

#define CYGHWR_HAL_MB_XPS_LL_FIFO_GET_RX_WORD_OCCUPANCY( base )        \
({                                                                     \
  cyg_uint32 _d;                                                       \
  HAL_READ_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_RDFO, _d );        \
  _d;                                                                  \
})

#define CYGHWR_HAL_MB_XPS_LL_FIFO_GET_RX_BYTE_COUNT( base )            \
({                                                                     \
  cyg_uint32 _d;                                                       \
  HAL_READ_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_RLF, _d );         \
  _d;                                                                  \
})

#define CYGHWR_HAL_MB_XPS_LL_FIFO_GET_WORD( base )                     \
({                                                                     \
  cyg_uint32 _d;                                                       \
  HAL_READ_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_RDFD, _d );        \
  _d;                                                                  \
})

#define CYGHWR_HAL_MB_XPS_LL_FIFO_READ_WORD( base, data )              \
({                                                                     \
  HAL_READ_UINT32( base + CYGHWR_HAL_MB_XPS_LL_FIFO_RDFD, data );      \
})

#endif // CYGONCE_HAL_VAR_LL_FIFO_H

// end of var_ll_fifo.h

