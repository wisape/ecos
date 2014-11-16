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
// Copyright (C) 2012 Free Software Foundation, Inc.
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
// Date:        2012-07-20
// Purpose:     
// Description: COMX-P4080 platform registers settings
// Usage:       
//
//####DESCRIPTIONEND####
//
//=============================================================================

#ifndef CYGONCE_HAL_PLF_REGS_H
#define CYGONCE_HAL_PLF_REGS_H

#if CYGHWR_HAL_POWERPC_QORIQ_DDR_CLK != 1300000000
#error DDRAM configuration unknown for this bus frequency
#endif

#define CONFIG_SYS_DDRC1_CS0_BNDS     0x000000ff
#define CONFIG_SYS_DDRC1_CS1_BNDS     0x00000000
#define CONFIG_SYS_DDRC1_CS2_BNDS     0x000000ff
#define CONFIG_SYS_DDRC1_CS3_BNDS     0x000000ff

#define CONFIG_SYS_DDRC2_CS0_BNDS     0x000000ff
#define CONFIG_SYS_DDRC2_CS1_BNDS     0x00000000
#define CONFIG_SYS_DDRC2_CS2_BNDS     0x000000ff
#define CONFIG_SYS_DDRC2_CS3_BNDS     0x000000ff

#define CONFIG_SYS_DDRC1_CS0_CONFIG   0xa0014202
#define CONFIG_SYS_DDRC1_CS1_CONFIG   0x80014202
#define CONFIG_SYS_DDRC1_CS2_CONFIG   0x00000000
#define CONFIG_SYS_DDRC1_CS3_CONFIG   0x00000000

#define CONFIG_SYS_DDRC2_CS0_CONFIG   0xa0014202
#define CONFIG_SYS_DDRC2_CS1_CONFIG   0x80014202
#define CONFIG_SYS_DDRC2_CS2_CONFIG   0x00000000
#define CONFIG_SYS_DDRC2_CS3_CONFIG   0x00000000

#define CONFIG_SYS_DDR_CS0_CONFIG2    0x00000000
#define CONFIG_SYS_DDR_CS1_CONFIG2    0x00000000
#define CONFIG_SYS_DDR_CS2_CONFIG2    0x00000000
#define CONFIG_SYS_DDR_CS3_CONFIG2    0x00000000

#define CONFIG_SYS_DDR_TIMING_3       0x01041000
#define CONFIG_SYS_DDR_TIMING_0       0x40550104
#define CONFIG_SYS_DDR_TIMING_1       0x98910a45
#define CONFIG_SYS_DDR_TIMING_2       0x0fb8a914
#define CONFIG_SYS_DDR_TIMING_4       0x00000001
#define CONFIG_SYS_DDR_TIMING_5       0x02401400
#define CONFIG_SYS_DDR_SDRAM_CFG_2    0x24401010
#define CONFIG_SYS_DDR_SDRAM_MODE_1   0x00421a50
#define CONFIG_SYS_DDR_SDRAM_MODE_2   0x00100000
#define CONFIG_SYS_DDR_SDRAM_MODE_3   0x00000000
#define CONFIG_SYS_DDR_SDRAM_MODE_4   0x00000000
#define CONFIG_SYS_DDR_SDRAM_MODE_5   0x00000000
#define CONFIG_SYS_DDR_SDRAM_MODE_6   0x00000000
#define CONFIG_SYS_DDR_SDRAM_MODE_7   0x00000000
#define CONFIG_SYS_DDR_SDRAM_MODE_8   0x00000000
#define CONFIG_SYS_DDR_RCW_1          0x00000000
#define CONFIG_SYS_DDR_RCW_2          0x00000000
#define CONFIG_SYS_DDR_MD_CNTL        0x00000000
#define CONFIG_SYS_DDR_INTERVAL       0x13ce0100
#define CONFIG_SYS_DDR_DATA_INIT      0xdeadbabe
#define CONFIG_SYS_DDR_CLK_CTRL       0x02800000
#define CONFIG_SYS_DDR_ZQ_CNTL        0x89080600
#define CONFIG_SYS_DDR_WRLVL_CNTL     0x8675a607
#define CONFIG_SYS_DDR_SDRAM_CFG      0x67044000
#define CONFIG_SYS_DDR_WRLVL_CNTL_2   0x00000000
#define CONFIG_SYS_DDR_WRLVL_CNTL_3   0x00000000

 // DDR3 Local access window, limited to 2GB while physical memory is 4GB
#define CONFIG_SYS_LBLAWBARH1         0x00000000
#define CONFIG_SYS_LBLAWBARL1         0x00000000   // 2048 MB starting from @ 0x0
#define CONFIG_SYS_LBLAWAR1           0x8140001E

// FLASH Local access window
#define CONFIG_SYS_LBLAWBARH0         0x00000000
#define CONFIG_SYS_LBLAWBARL0         (CYGPLF_MEM_FLASH_BASE)   // 128 MB starting from @ 0xf8000000
#define CONFIG_SYS_LBLAWAR0           0x81F0001A                // TRGT_ID = eLBC, SIZE = 128MB

// FLash settings
#define CONFIG_SYS_FLASH_OR0          0xf8000ff7
#define CONFIG_SYS_FLASH_OR0_CLKDIV32 0xf8000f47
#define CONFIG_SYS_FLASH_BR0          (CYGPLF_MEM_FLASH_BASE | 0x00001001)

#define CYGARC_IMM_BASE               0xe4000000

//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLF_REGS_H
// End of plf_regs.h
