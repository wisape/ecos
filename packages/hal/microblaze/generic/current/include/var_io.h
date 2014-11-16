#ifndef CYGONCE_HAL_VAR_IO_H
#define CYGONCE_HAL_VAR_IO_H
//=============================================================================
//
//      var_io.h
//
//      Variant specific registers
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
// Purpose:     Microblaze variant specific registers
// Description: 
// Usage:       #include <cyg/hal/var_io.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal_mb_generic.h>

#include <cyg/hal/plf_io.h>

//-------------------------------------------------------------
// Interrupt Controller
//

#define CYGHWR_HAL_MB_XPS_INTC_ISR                    0x00
#define CYGHWR_HAL_MB_XPS_INTC_IPR                    0x04
#define CYGHWR_HAL_MB_XPS_INTC_IER                    0x08
#define CYGHWR_HAL_MB_XPS_INTC_IAR                    0x0C
#define CYGHWR_HAL_MB_XPS_INTC_SIE                    0x10
#define CYGHWR_HAL_MB_XPS_INTC_CIE                    0x14
#define CYGHWR_HAL_MB_XPS_INTC_IVR                    0x18
#define CYGHWR_HAL_MB_XPS_INTC_MER                    0x1C

// MER register
#define CYGHWR_HAL_MB_XPS_INTC_MER_HIE                BIT_(1)
#define CYGHWR_HAL_MB_XPS_INTC_MER_ME                 BIT_(0)

//-------------------------------------------------------------
// XPS Timer
//
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR0                 0x00
#define CYGHWR_HAL_MB_XPS_TIMER_TLR0                  0x04
#define CYGHWR_HAL_MB_XPS_TIMER_TCR0                  0x08
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR1                 0x10
#define CYGHWR_HAL_MB_XPS_TIMER_TLR1                  0x14
#define CYGHWR_HAL_MB_XPS_TIMER_TCR1                  0x18
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR                  CYGHWR_HAL_MB_XPS_TIMER_TCSR0
#define CYGHWR_HAL_MB_XPS_TIMER_TLR                   CYGHWR_HAL_MB_XPS_TIMER_TLR0
#define CYGHWR_HAL_MB_XPS_TIMER_TCR                   CYGHWR_HAL_MB_XPS_TIMER_TCR0


// TCSR register
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_ENALL            BIT_(10)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_PWMA             BIT_(9)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_TOIN             BIT_(8)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_EN               BIT_(7)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_ENI              BIT_(6)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_LOAD             BIT_(5)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_ARHT             BIT_(4)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_CAP              BIT_(3)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_GEN              BIT_(2)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_UP               BIT_(1)
#define CYGHWR_HAL_MB_XPS_TIMER_TCSR_MD               BIT_(0)


//-------------------------------------------------------------
// UART Lite
//
#define CYGHWR_HAL_MB_XPS_UARTLITE_RX_FIFO            0x00
#define CYGHWR_HAL_MB_XPS_UARTLITE_TX_FIFO            0x04
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT               0x08
#define CYGHWR_HAL_MB_XPS_UARTLITE_CTRL               0x0C

// Controller register
#define CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_RST_TXFIFO    BIT_(0)
#define CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_RST_RXFIFO    BIT_(1)
#define CYGHWR_HAL_MB_XPS_UARTLITE_CTRL_IRQ_EN        BIT_(4)

// Status register
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_FVD        BIT_(0)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_FFULL      BIT_(1)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_TX_FEMPTY     BIT_(2)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_TX_FFULL      BIT_(3)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_IRQ_EN        BIT_(4)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_OV_ERR     BIT_(5)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_FRAME_ERR  BIT_(6)
#define CYGHWR_HAL_MB_XPS_UARTLITE_STAT_RX_PAR_ERR    BIT_(7)


//-------------------------------------------------------------
// UART 16550
//
#define CYGHWR_HAL_MB_XPS_UART16550_RBR               0x1000
#define CYGHWR_HAL_MB_XPS_UART16550_THR               0x1000
#define CYGHWR_HAL_MB_XPS_UART16550_IER               0x1004
#define CYGHWR_HAL_MB_XPS_UART16550_IIR               0x1008
#define CYGHWR_HAL_MB_XPS_UART16550_FCR               0x1008
#define CYGHWR_HAL_MB_XPS_UART16550_LCR               0x100C
#define CYGHWR_HAL_MB_XPS_UART16550_MCR               0x1010
#define CYGHWR_HAL_MB_XPS_UART16550_LSR               0x1014
#define CYGHWR_HAL_MB_XPS_UART16550_MSR               0x1018
#define CYGHWR_HAL_MB_XPS_UART16550_SCR               0x101C
#define CYGHWR_HAL_MB_XPS_UART16550_DLL               0x1000
#define CYGHWR_HAL_MB_XPS_UART16550_DLM               0x1004

// Interrupt Enable Register
#define CYGHWR_HAL_MB_XPS_UART16550_IER_ERBFI         BIT_(0)
#define CYGHWR_HAL_MB_XPS_UART16550_IER_ETBEI         BIT_(1)
#define CYGHWR_HAL_MB_XPS_UART16550_IER_ELSI          BIT_(2)
#define CYGHWR_HAL_MB_XPS_UART16550_IER_EDSSI         BIT_(3)

// Interrupt Identification Register
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_INTPEND       BIT_(0)
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_INTID_RLS     VALUE_(1,3)
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_INTID_RDA     VALUE_(1,2)
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_INTID_CT      VALUE_(1,6)
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_INTID_THRE    VALUE_(1,1)
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_INTID_MS      VALUE_(1,0)
#define CYGHWR_HAL_MB_XPS_UART16550_IIR_FIFOEN        VALUE_(6,0)

// FIFO Control Register
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_FE            BIT_(0)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_CRF           BIT_(1)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_CTF           BIT_(2)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_DMA           BIT_(3)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_RT1           VALUE_(6,0)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_RT4           VALUE_(6,1)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_RT8           VALUE_(6,2)
#define CYGHWR_HAL_MB_XPS_UART16550_FCR_RT14          VALUE_(6,3)

// Line Control Register
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_WL5           VALUE_(0,0)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_WL6           VALUE_(0,1)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_WL7           VALUE_(0,2)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_WL8           VALUE_(0,3)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_STOP_1        VALUE_(2,0)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_STOP_2        VALUE_(2,1)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_PEN           BIT_(3)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_PEVEN         BIT_(4)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_STICKP        BIT_(5)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_SET_BREAK     BIT_(6)
#define CYGHWR_HAL_MB_XPS_UART16550_LCR_DL            BIT_(7)

// Line Status Register
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_DR            BIT_(0)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_OE            BIT_(1)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_PE            BIT_(2)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_FE            BIT_(3)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_BI            BIT_(4)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_THRE          BIT_(5)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_TEMT          BIT_(6)
#define CYGHWR_HAL_MB_XPS_UART16550_LSR_FIE           BIT_(7)

// Modem Control Register
#define CYGHWR_HAL_MB_XPS_UART16550_MCR_DTR           BIT_(0)
#define CYGHWR_HAL_MB_XPS_UART16550_MCR_RTS           BIT_(1)
#define CYGHWR_HAL_MB_XPS_UART16550_MCR_OUT1          BIT_(2)
#define CYGHWR_HAL_MB_XPS_UART16550_MCR_OUT2          BIT_(3)
#define CYGHWR_HAL_MB_XPS_UART16550_MCR_LOOP          BIT_(4)

// Modem Status Register
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_DCTS          BIT_(0)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_DDSR          BIT_(1)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_TERI          BIT_(2)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_DDCD          BIT_(3)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_CTS           BIT_(4)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_DSR           BIT_(5)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_RI            BIT_(6)
#define CYGHWR_HAL_MB_XPS_UART16550_MSR_DCD           BIT_(7)

#endif // CYGONCE_HAL_VAR_IO_H

// end of var_io.h

