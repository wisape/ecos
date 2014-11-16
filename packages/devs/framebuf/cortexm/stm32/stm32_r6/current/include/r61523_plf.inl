#ifndef CYGONCE_DEVS_PLF_FRAMEBUF_R61523_INL
#define CYGONCE_DEVS_PLF_FRAMEBUF_R61523_INL
//=============================================================================
//
//      r61523_fb.inl
//
//      R61523 LCD controller platform specific code (STM32 R6)
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2014 Free Software Foundation, Inc.
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
// Date:        2014-05-27
// Purpose:     R61523 LCD controller platform specific code (STM32 R6)
// Description: 
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_if.h> 
#include <cyg/infra/diag.h>

// Internal structure for the platform dependent operations
typedef struct cyg_r61523_lcd_t
{
    cyg_uint32   base;     /* base address */
    cyg_uint32   width;    /* width */
    cyg_uint32   height;   /* height */
    cyg_uint32   pwm_io;   /* IO controlling the LCD backlight */
} cyg_r61523_lcd_t;

// Declare the LCD 0 screen
externC cyg_r61523_lcd_t cyg_r61523_lcd_0;

#define CYG_PLF_FB_fb0_BASE        &cyg_r61523_lcd_0
#define HAL_PLF_LCD_DATA_OFFSET    0x20000   // 0x20000 = bit A16

externC void hal_delay_us( cyg_int32 );

// -----------------------------------------------------------------------------
// This section defines a set of low level routines to access
// the LCD controller(s).
//
static __inline__ void hal_plf_lcd_backlight( cyg_r61523_lcd_t *, cyg_uint32 ) __attribute__((__always_inline__));
static __inline__ void hal_plf_lcd_write_cmd( cyg_r61523_lcd_t * , cyg_uint16 ) __attribute__((__always_inline__));
static __inline__ void hal_plf_lcd_write_data( cyg_r61523_lcd_t * , cyg_uint16 ) __attribute__((__always_inline__));
static __inline__ cyg_uint16 hal_plf_lcd_read_data( cyg_r61523_lcd_t * ) __attribute__((__always_inline__));

externC void hal_plf_lcd_controller_init(cyg_uint32 , cyg_uint16 , cyg_uint16 );

// Write command
static __inline__ void hal_plf_lcd_write_cmd( cyg_r61523_lcd_t * lcd, cyg_uint16 _v )
{
    HAL_WRITE_UINT16( lcd->base, _v );
}

// Write data
static __inline__ void hal_plf_lcd_write_data( cyg_r61523_lcd_t * lcd, cyg_uint16 _v )
{
    HAL_WRITE_UINT16( (lcd->base + HAL_PLF_LCD_DATA_OFFSET), _v );
}

// Read data
static __inline__ cyg_uint16 hal_plf_lcd_read_data( cyg_r61523_lcd_t * lcd )
{
    cyg_uint16 _v;
    HAL_READ_UINT16( (lcd->base + HAL_PLF_LCD_DATA_OFFSET), _v );
    return ( _v );
}

// Control backlight PWM
static __inline__ void hal_plf_lcd_backlight( cyg_r61523_lcd_t * lcd, cyg_uint32 _pwm_ )
{
    // Only support ON / OFF
    if(_pwm_)
      CYGHWR_HAL_STM32_GPIO_OUT (lcd->pwm_io, 1);
    else
      CYGHWR_HAL_STM32_GPIO_OUT (lcd->pwm_io, 0);
}

#define HAL_PLF_LCD_READ_DATA     hal_plf_lcd_read_data
#define HAL_PLF_LCD_INIT          hal_plf_lcd_controller_init
#define HAL_PLF_LCD_BACKLIGHT     hal_plf_lcd_backlight
#define HAL_PLF_LCD_WRITE_CMD     hal_plf_lcd_write_cmd
#define HAL_PLF_LCD_WRITE_DATA    hal_plf_lcd_write_data

#define CONVERT_COLOUR_HOST2TARGET(_c)                                                \
({                                                                                    \
  cyg_fb_colour _c_ = ((_c >> 16) | (_c & 0x0000ff00) | ((_c & 0x000000ff) << 16));   \
  _c_;                                                                                \
})

#define CONVERT_COLOUR_TARGET2HOST(_c)                                                \
({                                                                                    \
  cyg_fb_colour _c_ = ((_c >> 16) | (_c & 0x0000ff00) | ((_c & 0x000000ff) << 16));   \
  _c_;                                                                                \
})

//-----------------------------------------------------------------------------
#endif // CYGONCE_DEVS_PLF_FRAMEBUF_R61523_INL

// EOF of r61523_plf.inl
