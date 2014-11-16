//=============================================================================
//
//      ili9325_plf.c
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

#include <pkgconf/hal.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_if.h> 
#include <cyg/infra/diag.h>

#if defined(CYGPKG_KERNEL)
# include <cyg/kernel/kapi.h>
#endif

#include <ili9325.h>
#include CYGPKG_DEVS_FRAMEBUF_ILI9325_PLF_INL

// Declare LCD 0
cyg_ili9325_lcd_t cyg_ili9325_lcd_0;

// Initialize LCD controller
externC void 
hal_plf_lcd_controller_init(cyg_uint32 _b, cyg_uint16 _w, cyg_uint16 _h)
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_b;

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

        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_TIM1,   0x3008); // Set internal timing
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_TIM2,   0x0012); // Set internal timing
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_TIM3,   0x1231); // Set internal timing

        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DCR,    0x0001); // Start Oscillation
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DOC1,   0x0100); // Set SS and SM bit
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC,     0x0700); // Set 1 line inversion

        // 0x1010
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_EM,     0x1030); // Set GRAM write direction and BGR=0, 65536 colors, 1 transfers/pixel TRI.
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_RC,     0x0000); // Resize register

        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC2,    0x0202); // Set the back porch and front porch
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC3,    0x0000); // Set non-display area refresh cycle ISC[3:0]
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC4,    0x0000); // FMARK function

        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DIC1,   0x0000); // RGB interface setting
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_FMP,    0x0000); // Frame marker Position
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DIC2,   0x0000); // RGB interface polarity
        //Power On sequence
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC1,    0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC2,    0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC3,    0x0000); // VREG1OUT voltage
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC4,    0x0000); // VDV[4:0] for VCOM amplitude

        hal_delay_us(200000);                                                          // Dis-charge capacitor power voltage

        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC1,    0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC2,    0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
        FB_DELAY_MS(50); // Delay 50ms
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC3,    0x001C); // External reference voltage= Vci;
        FB_DELAY_MS(50); // Delay 50ms
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC4,    0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC7,    0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_FR,     0x000D); // Frame Rate = 91Hz
        FB_DELAY_MS(50); // Delay 50ms
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_HGAS,   0x0000); // GRAM horizontal Address
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_VGAS,   0x0000); // GRAM Vertical Address

        // ----------- Adjust the Gamma Curve ----------//
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC1,    0x0006);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC2,    0x0101);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC3,    0x0003);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC4,    0x0106);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC5,    0x0b02);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC6,    0x0302);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC7,    0x0707);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC8,    0x0007);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC9,    0x0600);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_GC10,   0x020b);

        //------------------ Set GRAM area ---------------//
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_HASP,   0x0000);  // Horizontal GRAM Start Address
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_VASP,   0x0000);  // Vertical GRAM Start Address
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DOC2,   0xA700);  // Gate Scan Line
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_BIDC,   0x0001);  // NDL,VLE, REV
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_VSC,    0x0000);  // Set scrolling line

        if( _w > _h ) {
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_HAEP, (_h-1));  // Horizontal GRAM End Address
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_VAEP, (_w-1));  // Vertical GRAM Start Address
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_EM,   0x1038);
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DOC1,  0x0000);
        } else {
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_HAEP, (_w-1));  // Horizontal GRAM End Address
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_VAEP, (_h-1));  // Vertical GRAM Start Address
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_EM,    0x1030);
            hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DOC1,  0x0100);
        }

        //-------------- Partial Display Control ---------//
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PI1DP,  0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PI1AS,  0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PI1AE,  0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PI2DP,  0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PI2AS,  0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PI2AE,  0x0000);

        //-------------- Panel Control -------------------//
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PIC1,   0x0010);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PIC2,   0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, 0x0093, 0x0003);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PIC4,   0x0110);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, 0x0097, 0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, 0x0098, 0x0000);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC1,    0x0133);

        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_HGAS,   0);
        hal_plf_lcd_write_reg(CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_VGAS,   0);

        //---------------- Turn OFF the screen ------------//
        hal_plf_lcd_write_reg( CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC1, 0x0132 );
        hal_delay_us(50000);
        hal_plf_lcd_write_reg( CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC1, 0x0122 );
        hal_delay_us(50000);
        hal_plf_lcd_write_reg( CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_DC1, 0x0100 );
        hal_delay_us(50000);
        hal_plf_lcd_write_reg( CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC1, 0x0600 );
        hal_plf_lcd_write_reg( CYG_PLF_FB_fb0_BASE, ILI9325_CONTROLLER_PC3, 0x000C );

    }
}

// EOF of ili9325_plf.c
