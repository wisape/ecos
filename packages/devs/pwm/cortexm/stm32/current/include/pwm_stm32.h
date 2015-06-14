//==========================================================================
//
//      pwm_stm32.h
//
//      STM32 PWM driver
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
// Author(s):    Wisape
// Contributors:
// Date:         2015-06-14
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================
#ifndef CYG_STM32_PWM_H
#define CYG_STM32_PWM_H

#include <pkgconf/system.h>
#include <pkgconf/devs_pwm_cortexm_stm32.h>
#include <cyg/io/devtab.h>

struct stm32_pwm_data {
};

static Cyg_ErrNo stm32_pwm_write(cyg_io_handle_t handle,
				const void *buf,
				cyg_uint32 *len);
static Cyg_ErrNo stm32_pwm_read(cyg_io_handle_t handle,
				void *buf,
				cyg_uint32 *len);
static Cyg_ErrNo stm32_pwm_set_config(cyg_io_handle_t handle,
				cyg_uint32 cmd,
				const void *buf,
				cyg_uint32 *len);
static Cyg_ErrNo stm32_pwm_get_config(cyg_io_handle_t handle,
				cyg_uint32 cmd,
				void *buf,
				cyg_uint32 *len);
static Cyg_ErrNo stm32_pwm_lookup(struct cyg_devtab_entry **tab,
				struct cyg_devtab_entry  *sub_tab,
				const char *name);
static bool stm32_pwm_init(struct cyg_devtab_entry *tab);

#endif
