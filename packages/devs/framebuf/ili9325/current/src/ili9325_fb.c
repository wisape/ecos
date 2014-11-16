//==========================================================================
//
//      ili9325_fb.c
//
//      Provide one framebuffer devices for the ILI9325 LCD controller.
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
//###DESCRIPTIONBEGIN####
//
// Author(s):     ccoutand
// Date:          2011-07-01
//
//###DESCRIPTIONEND####
//========================================================================

#include <cyg/io/framebuf.h>
#include <errno.h>
#include <string.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/hal_io.h>

#if defined(CYGPKG_KERNEL)
# include <cyg/kernel/kapi.h>
#endif

// The ILI9325 controller has support for partial display and resizing but does
// not support double buffering of the frame, whatever pixel is written to the
// GRAM will appear on the LCD "simultaneously".

// Set the DEBUG_LEVEL to 0 for no debugging, 2 for maximum debugging.
#define DEBUG_LEVEL 0
#define DEBUG(_level_, _str_, ...)              \
    CYG_MACRO_START                             \
    if (_level_ <= DEBUG_LEVEL) {               \
        diag_printf( _str_, ## __VA_ARGS__);    \
    }                                           \
    CYG_MACRO_END

void
_cyg_ili9325_fb_instantiate(struct cyg_fb* fb)
{
    HAL_PLF_LCD_INIT( (cyg_uint32)fb->fb_base, fb->fb_width, fb->fb_height );
}

static int
cyg_ili9325_fb_on(struct cyg_fb* fb)
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)fb->fb_base;
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_PC1, 0x1690 );
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0101 );
    FB_DELAY_MS(50);
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0121 );
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0123 );
    FB_DELAY_MS(50);
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0133 );
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_PC3, 0x001C );
    return 0;
}

static int
cyg_ili9325_fb_off(struct cyg_fb* fb)
{
	cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)fb->fb_base;
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0132 );
    FB_DELAY_MS(50);
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0122 );
    FB_DELAY_MS(50);
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_DC1, 0x0100 );
    FB_DELAY_MS(50);
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_PC1, 0x0600 );
    HAL_PLF_LCD_WRITE_REG( _lcd, ILI9325_CONTROLLER_PC3, 0x000C );
    return 0;
}

static int
cyg_ili9325_fb_ioctl(struct cyg_fb* fb, cyg_ucount16 key, void* data, size_t* len)
{
    int result  = ENOSYS;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)fb->fb_base;

    switch(key) {
    case CYG_FB_IOCTL_BACKLIGHT_SET:
       {
           cyg_fb_ioctl_backlight * backlight = (cyg_fb_ioctl_backlight*) data;
           if( backlight->fbbl_current == 0 )
               HAL_PLF_LCD_BACKLIGHT( _lcd, 0 );
           else
               HAL_PLF_LCD_BACKLIGHT( _lcd, 1 );
       }
       break;
    default:
       result  = ENOSYS;
       break;
    }
    return result;
}

// ----------------------------------------------------------------------------
// Landscape

void
cyg_fb_ili9325_write_pixel_landscape_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_fb_colour colour)
{
    cyg_fb_ili9325_write_pixel_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, colour);
}

cyg_fb_colour
cyg_fb_ili9325_read_pixel_landscape_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y)
{
    return cyg_fb_ili9325_read_pixel_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y);
}

void
cyg_fb_ili9325_write_hline_landscape_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len, cyg_fb_colour colour)
{
    cyg_fb_ili9325_write_hline_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, len, colour);
}

void
cyg_fb_ili9325_write_vline_landscape_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len, cyg_fb_colour colour)
{
    cyg_fb_ili9325_write_vline_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, len, colour);
}

void
cyg_fb_ili9325_fill_block_landscape_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height, cyg_fb_colour colour)
{
    cyg_fb_ili9325_fill_block_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, colour);
}

void
cyg_fb_ili9325_write_block_landscape_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height,
                             const void* source, cyg_ucount16 offset, cyg_ucount16 source_stride)
{
    cyg_fb_ili9325_write_block_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, source, offset, source_stride);
}

void
cyg_fb_ili9325_read_block_landscape_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height,
                            void* dest, cyg_ucount16 offset, cyg_ucount16 dest_stride)
{
    cyg_fb_ili9325_read_block_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, dest, offset, dest_stride);
}

void
cyg_fb_ili9325_move_block_landscape_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height,
                            cyg_ucount16 new_x, cyg_ucount16 new_y)
{
    cyg_fb_ili9325_move_block_landscape_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, new_x, new_y);
}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Portrait

void
cyg_fb_ili9325_write_pixel_portrait_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_fb_colour colour)
{
    cyg_fb_ili9325_write_pixel_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, colour);
}

cyg_fb_colour
cyg_fb_ili9325_read_pixel_portrait_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y)
{
    return cyg_fb_ili9325_read_pixel_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y);
}

void
cyg_fb_ili9325_write_hline_portrait_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len, cyg_fb_colour colour)
{
    cyg_fb_ili9325_write_hline_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, len, colour);
}

void
cyg_fb_ili9325_write_vline_portrait_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len, cyg_fb_colour colour)
{
    cyg_fb_ili9325_write_vline_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, len, colour);
}

void
cyg_fb_ili9325_fill_block_portrait_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height, cyg_fb_colour colour)
{
    cyg_fb_ili9325_fill_block_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, colour);
}

void
cyg_fb_ili9325_write_block_portrait_16(cyg_fb* fb,
                             cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height,
                             const void* source, cyg_ucount16 offset, cyg_ucount16 source_stride)
{
    cyg_fb_ili9325_write_block_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, source, offset, source_stride);
}

void
cyg_fb_ili9325_read_block_portrait_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height,
                            void* dest, cyg_ucount16 offset, cyg_ucount16 dest_stride)
{
    cyg_fb_ili9325_read_block_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, dest, offset, dest_stride);
}

void
cyg_fb_ili9325_move_block_portrait_16(cyg_fb* fb,
                            cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height,
                            cyg_ucount16 new_x, cyg_ucount16 new_y)
{
    cyg_fb_ili9325_move_block_portrait_16_inl(fb->fb_base, fb->fb_stride, x, y, width, height, new_x, new_y);
}
// ----------------------------------------------------------------------------



#define FB_ILI9325_MAKE_FN1(_fn_, _orientation_, _suffix_)  cyg_fb_ili9325_ ## _fn_ ## _ ## _orientation_ ## _ ##_suffix_
#define FB_ILI9325_MAKE_FN( _fn_, _orientation_, _suffix_)  FB_ILI9325_MAKE_FN1(_fn_, _orientation_, _suffix_)

#define FB_ILI9325_COLOUR_FN1(_fn_, _suffix_)  cyg_fb_dev_ ## _fn_ ## _ ## _suffix_
#define FB_ILI9325_COLOUR_FN( _fn_, _suffix_)  FB_ILI9325_COLOUR_FN1(_fn_, _suffix_)

#ifdef CYGPKG_DEVS_FRAMEBUF_ILI9325_FB0

extern cyg_fb CYG_FB_fb0_STRUCT;

CYG_FB_FRAMEBUFFER(CYG_FB_fb0_STRUCT,
                   CYG_FB_fb0_DEPTH,
                   CYG_FB_fb0_FORMAT,
                   CYG_FB_fb0_WIDTH,
                   CYG_FB_fb0_HEIGHT,
                   CYG_FB_fb0_VIEWPORT_WIDTH,
                   CYG_FB_fb0_VIEWPORT_HEIGHT,
                   CYG_FB_fb0_BASE,
                   CYG_FB_fb0_STRIDE,
                   CYG_FB_fb0_FLAGS0,
                   CYG_FB_fb0_FLAGS1,
                   CYG_FB_fb0_FLAGS2,
                   CYG_FB_fb0_FLAGS3,
                   (CYG_ADDRWORD) 0,
                   (CYG_ADDRWORD) 0,
                   (CYG_ADDRWORD) 0,
                   (CYG_ADDRWORD) 0,
                   &cyg_ili9325_fb_on,
                   &cyg_ili9325_fb_off,
                   &cyg_ili9325_fb_ioctl,
                   &cyg_fb_nop_synch,
                   &cyg_fb_nop_read_palette,
                   &cyg_fb_nop_write_palette,
                   &FB_ILI9325_COLOUR_FN(make_colour, 16bpp_true_565),
                   &FB_ILI9325_COLOUR_FN(break_colour, 16bpp_true_565),
                   FB_ILI9325_MAKE_FN(write_pixel, CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(read_pixel,  CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(write_hline, CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(write_vline, CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(fill_block,  CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(write_block, CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(read_block,  CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   FB_ILI9325_MAKE_FN(move_block,  CYG_FB_fb0_ORIENTATION, CYG_FB_fb0_DEPTH),
                   0, 0, 0, 0       // Spare0 -> spare3
    );

#endif

