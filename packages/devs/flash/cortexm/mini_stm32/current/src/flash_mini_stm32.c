//==========================================================================
//
//      flash_mini_stm32.c
//
//      SPI and Internal Flash support for MINI STM32 board
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
// Date:         2011-05-05
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifdef CYGPKG_KERNEL
#include <pkgconf/kernel.h>
#endif

#include <cyg/io/flash.h>
#include <cyg/io/flash_dev.h>
#include <cyg/hal/hal_io.h>

#include <pkgconf/system.h>

//--------------------------------------------------------------------------
// Internal flash

#if defined(CYGPKG_DEVS_FLASH_STM32)

#include <cyg/io/stm32_flash.h>

const cyg_stm32_flash_dev hal_stm32_flash_priv;

CYG_FLASH_DRIVER(hal_stm32_flash,
                 &cyg_stm32_flash_funs,
                 0,
                 0x08000000,
                 0,
                 0,
                 0,
                 &hal_stm32_flash_priv
);

#endif

// ------------------------------------------------------------------------
// There is also an SST25VF016B DataFlash on the SPI bus

#if defined(CYGPKG_IO_SPI) && defined(CYGPKG_DEVS_FLASH_SPI_SST25XX)

#include <cyg/io/spi.h>
#include <cyg/io/spi_stm32.h>
#include <cyg/io/sst25xx.h>

CYG_DEVS_SPI_CORTEXM_STM32_DEVICE (
    sst25vfxxx_spi_device, 1, 0, false, 0, 0, 25000000, 1, 1, 1
);


//-----------------------------------------------------------------------------
// Instantiate the SST25xx device driver.

CYG_DEVS_FLASH_SPI_SST25XX_DRIVER (
    sst25vfxxx_flash_device, 0x80000000, &sst25vfxxx_spi_device
);


#endif

// ------------------------------------------------------------------------
// EOF flash_mini_stm32.c
