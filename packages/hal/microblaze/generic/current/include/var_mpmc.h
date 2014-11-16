#ifndef CYGONCE_HAL_MPMC_H
#define CYGONCE_HAL_MPMC_H
//=============================================================================
//
//      var_mpmc.h
//
//      Multi-port Memory Controller registers
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
//              the Xilinx Multi-port Memory Controller
// Description:
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

#define CYGHWR_HAL_MB_XPS_MPMC_SERVICE_BASE(base, port)  ( base + ( port * 0x80 ) )

//-------------------------------------------------------------
// Soft Direct Memory Access (SDMA) registers
//

// TX descriptors
#define CYGHWR_HAL_MB_XPS_SDMA_TX_NXTDESC_PTR             0x00
#define CYGHWR_HAL_MB_XPS_SDMA_TX_CURBUF_ADDR             0x04
#define CYGHWR_HAL_MB_XPS_SDMA_TX_CURBUF_LEN              0x08
#define CYGHWR_HAL_MB_XPS_SDMA_TX_CURDESC_PTR             0x0C
#define CYGHWR_HAL_MB_XPS_SDMA_TX_TAILDESC_PTR            0x10
#define CYGHWR_HAL_MB_XPS_SDMA_TX_CH_CTRL                 0x14
#define CYGHWR_HAL_MB_XPS_SDMA_TX_IRQ                     0x18
#define CYGHWR_HAL_MB_XPS_SDMA_TX_CH_STATUS               0x1C
// RX descriptors
#define CYGHWR_HAL_MB_XPS_SDMA_RX_NXTDESC_PTR             0x20
#define CYGHWR_HAL_MB_XPS_SDMA_RX_CURBUF_ADDR             0x24
#define CYGHWR_HAL_MB_XPS_SDMA_RX_CURBUF_LEN              0x28
#define CYGHWR_HAL_MB_XPS_SDMA_RX_CURDESC_PTR             0x2C
#define CYGHWR_HAL_MB_XPS_SDMA_RX_TAILDESC_PTR            0x30
#define CYGHWR_HAL_MB_XPS_SDMA_RX_CH_CTRL                 0x34
#define CYGHWR_HAL_MB_XPS_SDMA_RX_IRQ                     0x38
#define CYGHWR_HAL_MB_XPS_SDMA_RX_CH_STATUS               0x3C
// DMA Control Register
#define CYGHWR_HAL_MB_XPS_SDMA_DMA_CTRL                   0x40

// Channel Control Register
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_IRQ_COAL_EN        BIT_(0)                 // Interrupt on Count Enable
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_IRQ_DLY_EN         BIT_(1)                 // Interrupt on Delay Enable
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_IRQ_ERR_EN         BIT_(2)                 // Interrupt on Error Enable
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_IRQ_EN             BIT_(7)                 // Master Interrupt Enable
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_LOAD_IRQ_CNT       BIT_(8)                 // Load IRQ Count
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_UIOE               BIT_(9)                 // Use Interrupt On End
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_1BIT_CNT           BIT_(10)                // Use 1 Bit Wide Counters. Currently Not Used.
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_IRQ_CNT(_x)        VALUE_(16, _x)          // Interrupt Coalescing Threshold Count Value
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CTRL_IRQ_TIMEOUT(_x)    VALUE_(24, _x)          // Interrupt Delay Time-out Value

// Interrupt Status Register
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_COAL                   BIT_(0)                 // Coalesce Interrupt Event
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_DLY                    BIT_(1)                 // Delay Interrupt Event
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_ERR                    BIT_(2)                 // Error Interrupt Event
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_DLY_CNT(_r)           ((_r >> BIT_(8)  & 0x3)  // Delay Interrupt Count
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_CLSC_CNT(_r)          ((_r >> BIT_(10) & 0xf)  // Coalesce Interrupt Count
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_CLSC_CNT_VAL(_r)      ((_r >> BIT_(16) & 0xff) // Coalesce Counter Value
#define CYGHWR_HAL_MB_XPS_SDMA_IRQ_CLSC_DLY_TMR_VAL(_r)  ((_r >> BIT_(24) & 0xff) // Delay Timer Value

// Channel Status Register
#define CYGHWR_HAL_MB_XPS_SDMA_CH_ENGINE_BUSY             BIT_(1)                 // Engine Busy
#define CYGHWR_HAL_MB_XPS_SDMA_CH_EOP                     BIT_(2)                 // End of Packet (EOP)
#define CYGHWR_HAL_MB_XPS_SDMA_CH_SOP                     BIT_(3)                 // Start of Packet (SOP)
#define CYGHWR_HAL_MB_XPS_SDMA_CH_COMPLETE                BIT_(4)                 // Complete
#define CYGHWR_HAL_MB_XPS_SDMA_CH_SOE                     BIT_(5)                 // Stop On End
#define CYGHWR_HAL_MB_XPS_SDMA_CH_IOE                     BIT_(6)                 // Interrupt On End
#define CYGHWR_HAL_MB_XPS_SDMA_CH_ERR                     BIT_(7)                 // DMA Error
#define CYGHWR_HAL_MB_XPS_SDMA_CH_BSY                     BIT_(16)                // Busy Write Error
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CPE                     BIT_(17)                // Current Descriptor Pointer Error
#define CYGHWR_HAL_MB_XPS_SDMA_CH_NPE                     BIT_(18)                // Next Descriptor Pointer Error
#define CYGHWR_HAL_MB_XPS_SDMA_CH_AE                      BIT_(19)                // Address Error
#define CYGHWR_HAL_MB_XPS_SDMA_CH_CE                      BIT_(20)                // Complete Error
#define CYGHWR_HAL_MB_XPS_SDMA_CH_TPE                     BIT_(21)                // Tail Pointer Error

// DMA Control Register
#define CYGHWR_HAL_MB_XPS_SDMA_DMA_CTR_SW_RST             BIT_(0)

// BDs
struct mdmc_sdma_bd {
  volatile struct mdmc_sdma_bd_t *next;
  cyg_uint8  *buf;
  cyg_uint32 len;
  cyg_uint32 sys_ctrl_app0;
  cyg_uint32 app1;
  cyg_uint32 app2;
  cyg_uint32 app3;
  cyg_uint32 app4;
};


#endif // CYGONCE_HAL_VAR_MPMC_H

// end of var_mpmc.h

