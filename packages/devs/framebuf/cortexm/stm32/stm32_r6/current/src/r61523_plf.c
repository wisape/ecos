//=============================================================================
//
//      r61523_plf.c
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

#include <pkgconf/hal.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_if.h> 
#include <cyg/infra/diag.h>

#if defined(CYGPKG_KERNEL)
# include <cyg/kernel/kapi.h>
#endif

#include <r61523.h>
#include CYGPKG_DEVS_FRAMEBUF_R61523_PLF_INL

// Declare LCD 0
cyg_r61523_lcd_t cyg_r61523_lcd_0;

// Initialize LCD controller
externC void 
hal_plf_lcd_controller_init(cyg_uint32 _b, cyg_uint16 _w, cyg_uint16 _h)
{
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_b;
    cyg_uint32 device_id, pixels;
    cyg_uint8  tmp;

    if( _b == (cyg_uint32) CYG_PLF_FB_fb0_BASE ){

        // Populate the LCD data
        _lcd->base   = CYGHWR_HAL_STM32_FSMC_BANK1_BASE;
        _lcd->width  = _w;
        _lcd->height = _h;
        _lcd->pwm_io = LCD_PWM_PIN;

        // Setup IOs
        CYGHWR_HAL_STM32_GPIO_SET( LCD_RST_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_OE_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_WE_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D0_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D1_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D2_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D3_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D4_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D5_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D6_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D7_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D8_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D9_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D10_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D11_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D12_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D13_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D14_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_D15_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_PWM_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_CS_PIN );
        CYGHWR_HAL_STM32_GPIO_SET( LCD_RS_PIN );

        CYGHWR_HAL_STM32_GPIO_OUT (LCD_RST_PIN, 0);
        CYGHWR_HAL_STM32_GPIO_OUT (LCD_PWM_PIN, 1);
 
        hal_delay_us(100000);
        CYGHWR_HAL_STM32_GPIO_OUT (LCD_RST_PIN, 1);
        hal_delay_us(100000);

        // Enable access to all commands from manufacturer
        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_MANUFACTURER_CMD);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x04);

        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_NORMAL_DISPLAY_MODE_ON);

        // Set the pixel format to 24bpp
        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_SET_PIXEL_FORMAT);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x07);

        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_DEVICE_CODE_READ);
        hal_plf_lcd_read_data(CYG_PLF_FB_fb0_BASE);

        tmp = hal_plf_lcd_read_data(CYG_PLF_FB_fb0_BASE);
        device_id  = (tmp << 24);
        tmp = hal_plf_lcd_read_data(CYG_PLF_FB_fb0_BASE);
        device_id |= (tmp << 16);
        tmp = hal_plf_lcd_read_data(CYG_PLF_FB_fb0_BASE);
        device_id |= (tmp <<  8);
        tmp = hal_plf_lcd_read_data(CYG_PLF_FB_fb0_BASE);
        device_id |= (tmp <<  0);

        if( device_id != 0x01221523 )
           diag_printf("Device ID expected (0x01221523) but read %x\n", device_id);
        else
           diag_printf("Device ID expected (0x01221523)\n");

        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_GET_PIXEL_FORMAT);
        diag_printf("Pixel format %x\n", hal_plf_lcd_read_data(CYG_PLF_FB_fb0_BASE));

        // Set DFM to 1 (2 transfers = 1 pixel)
        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_SET_FRAME_AND_INTERFACE);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x80);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x10);

        // TBD gamma settings

        // Set orientation
        if( _w > _h ) {
          // Landscape
          hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_SET_ADDRESS_MODE);
          hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0xe0);
        } else {
          // Portrait
          hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_SET_ADDRESS_MODE);
          hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x40);
        }

        // Set default area
        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_SET_COLUMN_ADDRESS);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x0);           // start
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x0);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, ((_w-1) >> 8)); // end
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, ((_w-1) & 0xff));
        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_SET_PAGE_ADDRESS);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x0);           // start
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x0);
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, ((_h-1) >> 8)); // end
        hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, ((_h-1) & 0xff));

        // Fill memory
        hal_plf_lcd_write_cmd(CYG_PLF_FB_fb0_BASE, R61523_MEMORY_WRITE);
        pixels = _h * _w;
        while(pixels--) {
           hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x0000);
           hal_plf_lcd_write_data(CYG_PLF_FB_fb0_BASE, 0x0000);
        }

    }
}

// EOF of r61523_plf.c
