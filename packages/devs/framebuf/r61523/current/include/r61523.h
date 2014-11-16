#ifndef CYGONCE_DEVS_FRAMEBUF_R61523_H
#define CYGONCE_DEVS_FRAMEBUF_R61523_H
//=============================================================================
//
//      r61523.h
//
//      r61523 LCD controller register definition
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
// Date:        2014-05-26
// Purpose:     r61523 LCD controller register definition
// Description:
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

#define R61523_NOP                      0x00
#define R61523_SOFTWARE_RESET           0x01
#define R61523_READ_DDB_START           0x04
#define R61523_GET_POWER_MODE           0x0a
#define R61523_GET_ADDR_MODE            0x0b
#define R61523_GET_PIXEL_FORMAT         0x0c
#define R61523_GET_PIXEL_MODE           0x0d
#define R61523_GET_SIGNAL_MODE          0x0e
#define R61523_GET_DIAG_MODE            0x0f
#define R61523_SLEEP_IN                 0x10
#define R61523_SLEEP_OUT                0x11
#define R61523_PARTIAL_MODE_ON          0x12
#define R61523_NORMAL_DISPLAY_MODE_ON   0x13
#define R61523_DISPLAY_OFF              0x28
#define R61523_DISPLAY_ON               0x29
#define R61523_SET_COLUMN_ADDRESS       0x2a
#define R61523_SET_PAGE_ADDRESS         0x2b
#define R61523_MEMORY_WRITE             0x2c
#define R61523_MEMORY_READ              0x2e
#define R61523_PARTIAL_AREA             0x30
#define R61523_SET_TEAR_OFF             0x34
#define R61523_SET_TEAR_ON              0x35
#define R61523_SET_ADDRESS_MODE         0x36
#define R61523_IDLE_MODE_OFF            0x38
#define R61523_IDLE_MODE_ON             0x39
#define R61523_SET_PIXEL_FORMAT         0x3a
#define R61523_MEMORY_WRITE_CONT        0x3c
#define R61523_MEMORY_READ_CONT         0x3e
#define R61523_SET_TEAR_SCANLINE        0x44
#define R61523_GET_SCANLINE             0x45
#define R61523_READ_DDB_CONT            0xa8
#define R61523_MANUFACTURER_CMD         0xb0
#define R61523_LOW_POWER_MODE_CTRL      0xb1
#define R61523_SET_FRAME_AND_INTERFACE  0xb3
#define R61523_BACKLIGHT_CONTROL_1      0xb8
#define R61523_BACKLIGHT_CONTROL_2      0xb9
#define R61523_BACKLIGHT_CONTROL_3      0xba
#define R61523_DEVICE_CODE_READ         0xbf
#define R61523_PANEL_DRIVING_SETTING    0xc0
#define R61523_PANEL_TIM_SETTING_NPM    0xc1
#define R61523_PANEL_TIM_SETTING_IDLE   0xc3
#define R61523_PANEL_TIM_SETTING_GATE   0xc4
#define R61523_GAMMA_SET_A              0xc8
#define R61523_GAMMA_SET_B              0xc9
#define R61523_GAMMA_SET_C              0xca
#define R61523_POWER_SETTING_COMMON     0xd0
#define R61523_POWER_SETTING_VCOM       0xd1
#define R61523_POWER_SETTING_NPM        0xd2
#define R61523_POWER_SETTING_IDLE       0xd4
#define R61523_NVM_ACCESS_CTRL          0xe0
#define R61523_SET_DDB_WRITE_CTRL       0xe1
#define R61523_FRAME_MEM_ACCESS_SETTING 0xe2
#define R61523_READ_CHECKSUM_ECC        0xe3
#define R61523_DSI_CONTROL              0xe4

#ifdef CYGPKG_KERNEL
# define FB_DELAY_MS(_msdelay_) cyg_thread_delay \
  (1 + ((1000 * _msdelay_ * CYGNUM_HAL_RTC_DENOMINATOR) / (CYGNUM_HAL_RTC_NUMERATOR / 1000)))

#else
# define FB_DELAY_MS(_msdelay_) CYGACC_CALL_IF_DELAY_US (_msdelay_ * 1000)
#endif

//-----------------------------------------------------------------------------
#endif // CYGONCE_DEVS_FRAMEBUF_R61523_H

// EOF of r61523.h
