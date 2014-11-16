#ifndef CYGONCE_DEVS_FRAMEBUF_ILI9325_H
#define CYGONCE_DEVS_FRAMEBUF_ILI9325_H
//=============================================================================
//
//      ili9325.h
//
//      ILI9325 LCD controller register definition
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
// Date:        2011-07-01
// Purpose:     ILI9325 LCD controller register definition
// Description:
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

#define ILI9325_CONTROLLER_DCR      0x00    // Driver Code Read
#define ILI9325_CONTROLLER_DOC1     0x01    // Driver Output Control 1
#define ILI9325_CONTROLLER_DC       0x02    // LCD Driving Control
#define ILI9325_CONTROLLER_EM       0x03    // Entry Mode
#define ILI9325_CONTROLLER_RC       0x04    // Resize Control
#define ILI9325_CONTROLLER_DC1      0x07    // Display Control 1
#define ILI9325_CONTROLLER_DC2      0x08    // Display Control 2
#define ILI9325_CONTROLLER_DC3      0x09    // Display Control 3
#define ILI9325_CONTROLLER_DC4      0x0A    // Display Control 3
#define ILI9325_CONTROLLER_DIC1     0x0C    // RGB Display Interface Control 1
#define ILI9325_CONTROLLER_FMP      0x0D    // Frame Maker Position
#define ILI9325_CONTROLLER_DIC2     0x0F    // RGB Display Interface Control 2
#define ILI9325_CONTROLLER_PC1      0x10    // Power Control 1
#define ILI9325_CONTROLLER_PC2      0x11    // Power Control 2
#define ILI9325_CONTROLLER_PC3      0x12    // Power Control 3
#define ILI9325_CONTROLLER_PC4      0x13    // Power Control 4
#define ILI9325_CONTROLLER_HGAS     0x20    // Horizontal GRAM Address Set
#define ILI9325_CONTROLLER_VGAS     0x21    // Vertical GRAM Address Set
#define ILI9325_CONTROLLER_WDG      0x22    // Write Data to GRAM
#define ILI9325_CONTROLLER_PC7      0x29    // Power Control 7
#define ILI9325_CONTROLLER_FR       0x2B    // Frame Rate
#define ILI9325_CONTROLLER_GC1      0x30    // Gamma Control 1
#define ILI9325_CONTROLLER_GC2      0x31    // Gamma Control 2
#define ILI9325_CONTROLLER_GC3      0x32    // Gamma Control 3
#define ILI9325_CONTROLLER_GC4      0x35    // Gamma Control 4
#define ILI9325_CONTROLLER_GC5      0x36    // Gamma Control 5
#define ILI9325_CONTROLLER_GC6      0x37    // Gamma Control 6
#define ILI9325_CONTROLLER_GC7      0x38    // Gamma Control 7
#define ILI9325_CONTROLLER_GC8      0x39    // Gamma Control 8
#define ILI9325_CONTROLLER_GC9      0x3C    // Gamma Control 9
#define ILI9325_CONTROLLER_GC10     0x3D    // Gamma Control 10
#define ILI9325_CONTROLLER_HASP     0x50    // Horizontal Address Start Position
#define ILI9325_CONTROLLER_HAEP     0x51    // Horizontal Address End Position
#define ILI9325_CONTROLLER_VASP     0x52    // Vertical Address Start Position
#define ILI9325_CONTROLLER_VAEP     0x53    // Vertical Address End Position
#define ILI9325_CONTROLLER_DOC2     0x60    // Driver Output Control 2
#define ILI9325_CONTROLLER_BIDC     0x61    // Base Image Display Control
#define ILI9325_CONTROLLER_VSC      0x6A    // Vertical Scroll Control
#define ILI9325_CONTROLLER_PI1DP    0x80    // Partial Image 1 Display Position
#define ILI9325_CONTROLLER_PI1AS    0x81    // Partial Image 1 Area (Start Line)
#define ILI9325_CONTROLLER_PI1AE    0x82    // Partial Image 1 Area (End Line)
#define ILI9325_CONTROLLER_PI2DP    0x83    // Partial Image 2 Display Position
#define ILI9325_CONTROLLER_PI2AS    0x84    // Partial Image 2 Area (Start Line)
#define ILI9325_CONTROLLER_PI2AE    0x85    // Partial Image 2 Area (End Line)
#define ILI9325_CONTROLLER_PIC1     0x90    // Panel Interface Control 1
#define ILI9325_CONTROLLER_PIC2     0x92    // Panel Interface Control 2
#define ILI9325_CONTROLLER_PIC4     0x95    // Panel Interface Control 3
#define ILI9325_CONTROLLER_OVPC     0xA1    // OTP VCM Programming Control
#define ILI9325_CONTROLLER_OVSE     0xA2    // OTP VCM Status and Enable
#define ILI9325_CONTROLLER_OVPI     0xA5    // OTP VCM Programming ID Key

#define ILI9325_CONTROLLER_TIM1     0xE3    // Internal Timing
#define ILI9325_CONTROLLER_TIM2     0xE7    // Internal Timing
#define ILI9325_CONTROLLER_TIM3     0xEF    // Internal Timing

#ifdef CYGPKG_KERNEL
# define FB_DELAY_MS(_msdelay_) cyg_thread_delay \
  (1 + ((1000 * _msdelay_ * CYGNUM_HAL_RTC_DENOMINATOR) / (CYGNUM_HAL_RTC_NUMERATOR / 1000)))

#else
# define FB_DELAY_MS(_msdelay_) CYGACC_CALL_IF_DELAY_US (_msdelay_ * 1000)
#endif

//-----------------------------------------------------------------------------
#endif // CYGONCE_DEVS_FRAMEBUF_ILI9325_H

// EOF of ili9325.h
