#ifndef CYGONCE_DEVS_PLF_FRAMEBUF_ILI9325_INL
#define CYGONCE_DEVS_PLF_FRAMEBUF_ILI9325_INL
//=============================================================================
//
//      ili9325_fb.inl
//
//      ILI9325 LCD controller platform specific code (MINI STM32)
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
// Purpose:     ILI9325 LCD controller platform specific code (MINI STM32)
// Description: 
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_if.h> 

// Internal structure for the platform dependent operations
typedef struct cyg_ili9325_lcd_t
{
    cyg_uint32   base;     /* base address */
    cyg_uint32   width;    /* width */
    cyg_uint32   height;   /* height */
    cyg_uint32   pwm_io;   /* IO controlling the LCD backlight */
} cyg_ili9325_lcd_t;

// Declare the LCD 0 screen
externC cyg_ili9325_lcd_t cyg_ili9325_lcd_0;

#define CYG_PLF_FB_fb0_BASE        &cyg_ili9325_lcd_0
#define HAL_PLF_LCD_DATA_OFFSET    0x20000   // 0x20000 = bit A16


// -----------------------------------------------------------------------------
// This section defines a set of low level routines to access
// the LCD controller(s).
//

static __inline__ void hal_plf_lcd_write_reg( cyg_ili9325_lcd_t * , cyg_uint16 , cyg_uint16 ) __attribute__((__always_inline__));
static __inline__ void hal_plf_lcd_write_ir( cyg_ili9325_lcd_t * , cyg_uint16 ) __attribute__((__always_inline__));
static __inline__ void hal_plf_lcd_write_ram( cyg_ili9325_lcd_t * , cyg_uint16 ) __attribute__((__always_inline__));
static __inline__ cyg_uint16 hal_plf_lcd_read_ram( cyg_ili9325_lcd_t * ) __attribute__((__always_inline__));
static __inline__ void hal_plf_lcd_backlight( cyg_ili9325_lcd_t *, cyg_uint32 ) __attribute__((__always_inline__));

externC void hal_plf_lcd_controller_init(cyg_uint32 , cyg_uint16 , cyg_uint16 );

// Write _v to register _r
static __inline__ void hal_plf_lcd_write_reg( cyg_ili9325_lcd_t * lcd, cyg_uint16 _r, cyg_uint16 _v )
{
    HAL_WRITE_UINT16(  lcd->base, _r );
    HAL_WRITE_UINT16( (lcd->base + HAL_PLF_LCD_DATA_OFFSET), _v );
}

// Write to Index Register (IR)
static __inline__ void hal_plf_lcd_write_ir( cyg_ili9325_lcd_t * lcd, cyg_uint16 _v )
{
    HAL_WRITE_UINT16( lcd->base, _v );
}

// Write to GRAM
static __inline__ void hal_plf_lcd_write_ram( cyg_ili9325_lcd_t * lcd, cyg_uint16 _v )
{
    HAL_WRITE_UINT16( (lcd->base + HAL_PLF_LCD_DATA_OFFSET), _v );
}


// Read from GRAM
static __inline__ cyg_uint16 hal_plf_lcd_read_ram( cyg_ili9325_lcd_t * lcd )
{
    cyg_uint16 _v, r, g, b;
    HAL_READ_UINT16( (lcd->base + HAL_PLF_LCD_DATA_OFFSET), _v ); // Dummy read, investigate interface timing?
    HAL_READ_UINT16( (lcd->base + HAL_PLF_LCD_DATA_OFFSET), _v );
    r = ( _v >> 0 )  & 0x1f;
    g = ( _v >> 5 )  & 0x3f;
    b = ( _v >> 11 ) & 0x1f;
    return ( (r<<11) + (g<<5) + (b<<0) );
}

// Control backlight PWM
static __inline__ void hal_plf_lcd_backlight( cyg_ili9325_lcd_t * lcd, cyg_uint32 _pwm_ )
{
    // Only support ON / OFF
    if(_pwm_)
      CYGHWR_HAL_STM32_GPIO_OUT (lcd->pwm_io, 1);
    else
      CYGHWR_HAL_STM32_GPIO_OUT (lcd->pwm_io, 0);
}

#define HAL_PLF_LCD_WRITE_REG     hal_plf_lcd_write_reg
#define HAL_PLF_LCD_WRITE_RAM     hal_plf_lcd_write_ram
#define HAL_PLF_LCD_WRITE_IR      hal_plf_lcd_write_ir
#define HAL_PLF_LCD_READ_RAM      hal_plf_lcd_read_ram
#define HAL_PLF_LCD_INIT          hal_plf_lcd_controller_init
#define HAL_PLF_LCD_BACKLIGHT     hal_plf_lcd_backlight

//-----------------------------------------------------------------------------
#endif // CYGONCE_DEVS_PLF_FRAMEBUF_ILI9325_INL

// EOF of ili9325_plf.inl
