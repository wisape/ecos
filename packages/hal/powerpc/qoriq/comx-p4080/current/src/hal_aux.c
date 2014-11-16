//=============================================================================
//
//      hal_aux.c
//
//      HAL auxiliary objects and code; per platform
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2002, 2003 Gary Thomas
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
// Author(s):   hmt
// Contributors:hmt, gthomas
// Date:        1999-06-08
// Purpose:     HAL aux objects: startup tables.
// Description: Tables for per-platform initialization
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <cyg/hal/hal_mem.h>            // HAL memory definitions
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/qoriq.h>             // For IMM structures
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/plf_regs.h>
#include <cyg/hal/ppc_regs.h>

#ifdef CYGPKG_KERNEL
#include <cyg/kernel/kapi.h>
#endif

externC void hal_enable_L2_cache( void );
externC void cyg_hal_enable_cpc( void );
externC void hal_qoriq_parse_rcw( void );

unsigned long long sec_tick;

// FIXME

static __inline__ unsigned long
_le32(unsigned long val)
{
    return (((val & 0x000000FF) << 24) |
            ((val & 0x0000FF00) <<  8) |
            ((val & 0x00FF0000) >>  8) |
            ((val & 0xFF000000) >> 24));
}

static __inline__ unsigned short
_le16(unsigned short val)
{
    return (((val & 0x000000FF) << 8) |
            ((val & 0x0000FF00) >> 8));
}

#define HAL_WRITE_UINT32LE(_addr_, _val_) \
  HAL_WRITE_UINT32(_addr_, _le32(_val_))
#define HAL_WRITE_UINT16LE(_addr_, _val_) \
  HAL_WRITE_UINT16(_addr_, _le16(_val_))
#define HAL_WRITE_UINT8LE(_addr_, _val_) \
  HAL_WRITE_UINT8(_addr_, _val_)
#define HAL_READ_UINT32LE(_addr_, _val_)        \
  {                                             \
      HAL_READ_UINT32(_addr_, _val_);           \
      _val_ = _le32(_val_);                     \
  }
#define HAL_READ_UINT16LE(_addr_, _val_)        \
  {                                             \
      HAL_READ_UINT16(_addr_, _val_);           \
      _val_ = _le16(_val_);                     \
  }
#define HAL_READ_UINT8LE(_addr_, _val_)        \
  HAL_READ_UINT8(_addr_, _val_)

// FIXME

// The memory map is weakly defined, allowing the application to redefine
// it if necessary. The regions defined below are the minimum requirements.
// FLASH, IMMR and RAM are not part of this table. They must be configured 
// very early since the e500 core starts with MMU enable
CYGARC_MEMDESC_TABLE CYGBLD_ATTRIB_WEAK = {
    // FIXME, Add entry for PCI, NAND flash etc...
    CYGARC_MEMDESC_TABLE_END
};


//--------------------------------------------------------------------------
// Platform init code.
void
hal_platform_init(void)
{
#if !defined(CYG_HAL_STARTUP_ROM)
    cyg_uint32 reg;
#endif

#ifdef CYGHWR_HAL_POWERPC_ENABLE_L2_CACHE
    // Setup L2 cache
    hal_enable_L2_cache();
#endif

    cyg_hal_enable_cpc();

#ifndef CYGSEM_HAL_USE_ROM_MONITOR
    // Start up system I/O
    hal_if_init();
#endif // CYGSEM_HAL_USE_ROM_MONITOR

#if defined(CYG_HAL_STARTUP_ROM) ||       \
        defined(CYG_HAL_STARTUP_ROMRAM)
    hal_qoriq_parse_rcw();
#endif

#if !defined(CYG_HAL_STARTUP_ROM)
    //
    // Update local bus frequency and and NOR flash timing

    // NOTE: It is critical that no transactions are being executed via
    // the local bus while CLKDIV is being modified. As such, prior to
    // modification, the user must ensure that code is not executing out of
    // the local bus. Once LCRR[CLKDIV] is written, the register should be
    // read, and then an isync should be executed.
    HAL_WRITE_UINT32(
      ( CYGARC_IMM_BASE + CYGARC_REG_IMM_ELBC + CYGARC_REG_IMM_ELBC_OR0 ),
      ( CONFIG_SYS_FLASH_OR0 ) );

    // Set local bus clock to PLF Clk / 8 = 800 / 8 = 100MHz
    HAL_READ_UINT32(
      ( CYGARC_IMM_BASE + CYGARC_REG_IMM_ELBC + CYGARC_REG_IMM_ELBC_LCRR ),
        reg);
    reg &= ~CYGARC_REG_IMM_ELBC_LCRR_CLKDIV_MASK;
    reg |= CYGARC_REG_IMM_ELBC_LCRR_CLKDIV_8;
    HAL_WRITE_UINT32(
      ( CYGARC_IMM_BASE + CYGARC_REG_IMM_ELBC + CYGARC_REG_IMM_ELBC_LCRR ),
        reg);

    // Read back / isync
    HAL_READ_UINT32(
      ( CYGARC_IMM_BASE + CYGARC_REG_IMM_ELBC + CYGARC_REG_IMM_ELBC_LCRR ),
        reg);
    asm volatile("isync;");
#endif
}

/******************************* functions for RTC ***************************/
// ms to ticks.
#define CALC_TICKS(ms)  (((cyg_tick_count_t)(ms)) * (CYGNUM_HAL_RTC_PERIOD * 100)) / 1000000

// FIXME
// unsigned long rtc_zero(void){}
// unsigned long rtc_read(void){}
// unsigned long rtc_read_sec(void){}

// EOF hal_aux.c
