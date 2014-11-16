//=============================================================================
//
//      plf_regs.h
//
//      Platform HAL register details
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2009, 2010 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   ccoutand
// Contributors:
// Date:        2009-11-01
// Purpose:     
// Description: Freescale MPC8572DS registers settings
// Usage:       
//
//####DESCRIPTIONEND####
//
//=============================================================================

#ifndef CYGONCE_HAL_PLF_REGS_H
#define CYGONCE_HAL_PLF_REGS_H

#if CYGHWR_HAL_POWERPC_BUS_SPEED != 533
#error DDRAM configuration unknown for this bus frequency
#endif

#ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR_INTERLEAVED

# ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR2_CONTROLLER
#define CONFIG_SYS_DDRC1_CS0_BNDS   0x0000003f
#define CONFIG_SYS_DDRC2_CS0_BNDS   0x0000003f
#else
#define CONFIG_SYS_DDRC1_CS0_BNDS   0x0000001f
#endif
#define CONFIG_SYS_DDR_CS0_CONFIG   0xa2040202

#else // ! CYGHWR_HAL_POWERPC_MPC8572DS_DDR_INTERLEAVED

#define CONFIG_SYS_DDRC1_CS0_BNDS   0x0000001f
#define CONFIG_SYS_DDRC2_CS0_BNDS   0x0020003f
#define CONFIG_SYS_DDR_CS0_CONFIG   0x80010202

#endif // CYGHWR_HAL_POWERPC_MPC8572DS_DDR_INTERLEAVED

#define CONFIG_SYS_DDR_CS0_CONFIG2  0x00000000
#define CONFIG_SYS_DDR_TIMING_3     0x00010000
#define CONFIG_SYS_DDR_TIMING_0     0x00260802
#define CONFIG_SYS_DDR_TIMING_1     0x5F59B534
#define CONFIG_SYS_DDR_TIMING_2     0x052074CD
#define CONFIG_SYS_DDR_CFG_2        0x24400010
#define CONFIG_SYS_DDR_MODE_1       0x00440452
#define CONFIG_SYS_DDR_MODE_2       0x00000000
#define CONFIG_SYS_DDR_MD_CNTL      0x00000000
#define CONFIG_SYS_DDR_INTERVAL     0x0A280100
#define CONFIG_SYS_DDR_DATA_INIT    0xdeadbabe
#define CONFIG_SYS_DDR_CLK_CTRL     0x03800000
#define CONFIG_SYS_DDR_CFG          0x63008000
#define CONFIG_SYS_DDR_CFG_DDR_EN   0xE3008000

#if !defined(CYGHWR_HAL_POWERPC_MPC8572DS_DDR_INTERLEAVED)

 // SDRAM Local access window
 #define CONFIG_SYS_LBLAWBAR1       0x00000000   // 512 MB starting from @ 0x0
 #define CONFIG_SYS_LBLAWAR1        0x80F0001C

 #define CONFIG_SYS_LBLAWBAR2       0x00020000   // 512 MB starting from @ 0x20000000
 #define CONFIG_SYS_LBLAWAR2        0x8160001C

#else

 // SDRAM Local access window
 #define CONFIG_SYS_LBLAWBAR1       0x00000000   // 1024 MB starting from @ 0x0
 #define CONFIG_SYS_LBLAWAR1        0x80B0001D

#endif

// FLASH Local access window
#define CONFIG_SYS_LBLAWBAR3        0x000e8000   // 128 MB starting from @ 0xe8000000
#define CONFIG_SYS_LBLAWAR3         0x8040001A

// FLash settings
#define CONFIG_SYS_FLASH_OR0        0xf8000f80
#define CONFIG_SYS_FLASH_BR0        0xe8001001

//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLF_REGS_H
// End of plf_regs.h
