#ifndef CYGONCE_IO_GPIO_H
#define CYGONCE_IO_GPIO_H
//==========================================================================
//
//      include/gpio.h
//
//      Generic API definition for accessing GPIO
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2008 Free Software Foundation, Inc.
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
// Date:         2010-06-03
// Purpose:
// Description:  Generic API definition for accessing GPIO
//
//
//####DESCRIPTIONEND####
//==========================================================================

#ifdef __cplusplus
extern "C" {
#endif

#define cyg_gpio_t cyg_uint32

typedef enum {
    CYG_GPIO_IN,
    CYG_GPIO_OUT,
    CYG_GPIO_PULLUP,
    CYG_GPIO_PULLDOWN
} cyg_gpio_mode_t;

typedef enum {
    CYG_GPIO_RESET,
    CYG_GPIO_SET
} cyg_gpio_state_t;

// Change state of a GPIO
inline void cyg_hal_gpio_write( cyg_gpio_t, cyg_gpio_state_t );

// Read state of a GPIO
inline cyg_gpio_state_t cyg_hal_gpio_read( cyg_gpio_t );

// Configure GPIO properties
inline void cyg_hal_gpio_cfg( cyg_gpio_t, cyg_gpio_mode_t );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CYGONCE_IO_GPIO_H

