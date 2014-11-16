//==========================================================================
//
//      mb_uart16550.inl
//
//      Microblaze serial driver (interrupt driven)
//      for use with 16550s UART
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
// Original:     jlarmour (i386)
// Date:         2011-02-18
// Purpose:      Microblaze serial driver (interrupt driven version)
// Description: 
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/hal/hal_intr.h>

//-----------------------------------------------------------------------------
// Baud rate specification

static unsigned short select_baud[] = {
           0,  // Unused
           0,  // 50
           0,  // 75
           0,  // 110
           0,  // 134.5
           0,  // 150
           0,  // 200
           0,  // 300
           0,  // 600
           0,  // 1200
           0,  // 1800
           0,  // 2400
           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_CLKFREQ / (  (3600)<<4),  // 3600
           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_CLKFREQ / (  (4800)<<4),  // 4800
           0,  // 7200
           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_CLKFREQ / (  (9600)<<4),  // 9600
           0,  // 14400
           0,  // 19200
           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_CLKFREQ / ( (38400)<<4),  // 38400
           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_CLKFREQ / ( (57600)<<4),  // 57600
           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_CLKFREQ / ((115200)<<4),  // 115200
           0   // 230400
};

#ifdef CYGPKG_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0

static pc_serial_info mb_serial_info0 = { (CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_IOBASE + 0x1003), // 0x1003 offset to first byte
                                           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_INT};
#if CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_BUFSIZE > 0
static unsigned char mb_serial_out_buf0[CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_BUFSIZE];
static unsigned char mb_serial_in_buf0[CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_BUFSIZE];

static SERIAL_CHANNEL_USING_INTERRUPTS(mb_serial_channel0,
                                       pc_serial_funs, 
                                       mb_serial_info0,
                                       CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_BAUD),
                                       CYG_SERIAL_STOP_DEFAULT,
                                       CYG_SERIAL_PARITY_DEFAULT,
                                       CYG_SERIAL_WORD_LENGTH_DEFAULT,
                                       CYG_SERIAL_FLAGS_DEFAULT,
                                       &mb_serial_out_buf0[0], sizeof(mb_serial_out_buf0),
                                       &mb_serial_in_buf0[0], sizeof(mb_serial_in_buf0)
    );
#else
static SERIAL_CHANNEL(mb_serial_channel0,
                      pc_serial_funs, 
                      mb_serial_info0,
                      CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_BAUD),
                      CYG_SERIAL_STOP_DEFAULT,
                      CYG_SERIAL_PARITY_DEFAULT,
                      CYG_SERIAL_WORD_LENGTH_DEFAULT,
                      CYG_SERIAL_FLAGS_DEFAULT
    );
#endif

DEVTAB_ENTRY(mb_serial_io0, 
             CYGDAT_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0_NAME,
             0,                     // Does not depend on a lower level interface
             &cyg_io_serial_devio, 
             pc_serial_init, 
             pc_serial_lookup,     // Serial driver may need initializing
             &mb_serial_channel0
    );
#endif //  CYGPKG_IO_SERIAL_MICROBLAZE_UART16550_SERIAL0

#ifdef CYGPKG_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1
static pc_serial_info mb_serial_info1 = { (CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_IOBASE + 0x1003), // 0x1003 offset to first byte
                                           CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_INT};
#if CYGNUM_IO_SERIAL_I386_PC_SERIAL1_BUFSIZE > 0
static unsigned char mb_serial_out_buf1[CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_BUFSIZE];
static unsigned char mb_serial_in_buf1[CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_BUFSIZE];

static SERIAL_CHANNEL_USING_INTERRUPTS(mb_serial_channel1,
                                       pc_serial_funs, 
                                       mb_serial_info1,
                                       CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_BAUD),
                                       CYG_SERIAL_STOP_DEFAULT,
                                       CYG_SERIAL_PARITY_DEFAULT,
                                       CYG_SERIAL_WORD_LENGTH_DEFAULT,
                                       CYG_SERIAL_FLAGS_DEFAULT,
                                       &mb_serial_out_buf1[0], sizeof(mb_serial_out_buf1),
                                       &mb_serial_in_buf1[0], sizeof(mb_serial_in_buf1)
    );
#else
static SERIAL_CHANNEL(mb_serial_channel1,
                      pc_serial_funs, 
                      mb_serial_info1,
                      CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_BAUD),
                      CYG_SERIAL_STOP_DEFAULT,
                      CYG_SERIAL_PARITY_DEFAULT,
                      CYG_SERIAL_WORD_LENGTH_DEFAULT,
                      CYG_SERIAL_FLAGS_DEFAULT
    );
#endif

DEVTAB_ENTRY(mb_serial_io1, 
             CYGDAT_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1_NAME,
             0,                     // Does not depend on a lower level interface
             &cyg_io_serial_devio, 
             pc_serial_init, 
             pc_serial_lookup,     // Serial driver may need initializing
             &mb_serial_channel1
    );
#endif //  CYGPKG_IO_SERIAL_MICROBLAZE_UART16550_SERIAL1

// EOF mb_uart16550.inl
