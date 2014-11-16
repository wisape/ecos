#ifndef CYGONCE_DEVS_FRAMEBUF_ILI9325_DIRECT_INL
#define CYGONCE_DEVS_FRAMEBUF_ILI9325_DIRECT_INL
//=============================================================================
//
//      ili9325_direct.inl
//
//      ILI9325 LCD controller framebuffer driver
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
// Purpose:     ILI9325 LCD controller framebuffer driver
// Description: 
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================


// The always_inline attribute must be a applied to a declaration, not a
// definition, so combine the two via a single macro.
#define CYG_FB_DEVS_FRAMEBUF_INLINE_FN(_type_, _name_, _args_)                        \
static __inline__ _type_ _name_ _args_ __attribute__((__always_inline__));            \
static __inline__ _type_ _name_ _args_

# define CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ )                                       \
    CYG_MACRO_START                                                                   \
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;                         \
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP,  0);                         \
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP,  0);                         \
    if ( _lcd->width > _lcd->height ) {                                               \
       HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, (_lcd->height-1));        \
       HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, (_lcd->width-1));         \
    } else {                                                                          \
       HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, (_lcd->height-1));        \
       HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, (_lcd->width-1));         \
    }                                                                                 \
    CYG_MACRO_END

// ----------------------------------------------------------------------------
// LCD in landscape mode

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_pixel_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_fb_colour _colour_))
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_);
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_);
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(cyg_fb_colour,
                         cyg_fb_ili9325_read_pixel_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_))
{
    cyg_uint16  _pixel_;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_);
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_);
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    _pixel_ = HAL_PLF_LCD_READ_RAM( _lcd );
    return _pixel_;
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_hline_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_uint32 pixels;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_);
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_);
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (pixels = 0; pixels < _len_; pixels++)
        HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_vline_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_uint16 _len;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _y_ + _len_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_ );
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (_len  = _len_ ; _len; _len--) {
      HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_fill_block_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, cyg_fb_colour _colour_))
{
    cyg_uint16 _width, _height;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _y_ + _height_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _x_ +  _width_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_ );
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (_height = _height_ ; _height; _height--) {
      for (_width  = _width_ ; _width; _width--) {
        HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
      }
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_block_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, const void* _source_, cyg_ucount16 _offset_, cyg_ucount16 _source_stride8_))
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    cyg_uint16* _src16_     = ((cyg_uint16*) _source_) + _offset_;
    cyg_uint16* _current16  = _src16_;
    cyg_uint16 _width, _height;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _y_ + _height_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _x_ +  _width_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_ );
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (_height = _height_ ; _height; _height--) {
        for (_width  = _width_ ; _width; _width--) {
            HAL_PLF_LCD_WRITE_RAM(_lcd, *_current16);
            _current16++;
        }
        _current16 = _src16_ = (cyg_uint16*)(((cyg_uint8*)_src16_) + _source_stride8_);
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_read_block_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, void* _dest_, cyg_ucount16 _offset_, cyg_ucount16 _dest_stride8_))
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    cyg_uint16* _dst16_ = ((cyg_uint16*)_dest_) + _offset_;
    cyg_uint16* _current16  = _dst16_;
    cyg_uint16 _width, _height;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _y_ + _height_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _x_ +  _width_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _x_ );
    for (_height = _height_ ; _height; _height--) {
        for (_width  = _width_ ; _width; _width--) {
            *_current16 = HAL_PLF_LCD_READ_RAM(_lcd);
            _current16++;
        }
        _current16 = _dst16_ = (cyg_uint16*)(((cyg_uint8*)_dst16_) + _dest_stride8_);
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_move_block_landscape_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height, cyg_ucount16 new_x, cyg_ucount16 new_y))
{
    // TODO
}

// ----------------------------------------------------------------------------
// LCD in portrait mode

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_pixel_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_fb_colour _colour_))
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_);
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_);
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(cyg_fb_colour,
                         cyg_fb_ili9325_read_pixel_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_))
{
    cyg_uint16  _pixel_;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_);
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_);
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    _pixel_ = HAL_PLF_LCD_READ_RAM( _lcd );
    return _pixel_;
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_hline_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_uint32 pixels;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_);
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_);
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (pixels = 0; pixels < _len_; pixels++)
        HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_vline_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_uint16 _len;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _y_ + _len_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_ );
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (_len  = _len_ ; _len; _len--) {
      HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_fill_block_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, cyg_fb_colour _colour_))
{
    cyg_uint16 _width, _height;
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _y_ + _height_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _x_ +  _width_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_ );
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (_height = _height_ ; _height; _height--) {
      for (_width  = _width_ ; _width; _width--) {
        HAL_PLF_LCD_WRITE_RAM(_lcd, _colour_);
      }
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_write_block_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, const void* _source_, cyg_ucount16 _offset_, cyg_ucount16 _source_stride8_))
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    cyg_uint16* _src16_ = ((cyg_uint16*) _source_) + _offset_;
    cyg_uint16* _current16  = _src16_;
    cyg_uint16 _width, _height;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _y_ + _height_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _x_ +  _width_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_ );
    HAL_PLF_LCD_WRITE_IR (_lcd, ILI9325_CONTROLLER_WDG);
    for (_height = _height_ ; _height; _height--) {
      for (_width  = _width_ ; _width; _width--) {
         HAL_PLF_LCD_WRITE_RAM(_lcd, *_current16);
         _current16++;
       }
       _current16 = _src16_ = (cyg_uint16*)(((cyg_uint8*)_src16_) + _source_stride8_);
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_read_block_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, void* _dest_, cyg_ucount16 _offset_, cyg_ucount16 _dest_stride8_))
{
    cyg_ili9325_lcd_t * _lcd = (cyg_ili9325_lcd_t *)_fbaddr_;
    cyg_uint16* _dst16_     = ((cyg_uint16*)_dest_) + _offset_;
    cyg_uint16* _current16  = _dst16_;
    cyg_uint16 _width, _height;
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VASP, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VAEP, _y_ + _height_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HASP, _x_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HAEP, _x_ +  _width_ - 1 );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_VGAS, _y_ );
    HAL_PLF_LCD_WRITE_REG(_lcd, ILI9325_CONTROLLER_HGAS, _x_ );
    for (_height = _height_ ; _height; _height--) {
      for (_width  = _width_ ; _width; _width--) {
         *_current16 = HAL_PLF_LCD_READ_RAM(_lcd);
          _current16++;
      }
      _current16 = _dst16_ = (cyg_uint16*)(((cyg_uint8*)_dst16_) + _dest_stride8_);
    }
    CYG_FB_DEFAULT_WINDOW_AREA( _fbaddr_ );
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_ili9325_move_block_portrait_16_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height, cyg_ucount16 new_x, cyg_ucount16 new_y))
{
    // TODO
}

// ----------------------------------------------------------------------------

#define CYG_FB_ILI9325_PIXELx_VAR_16(_fb_, _which_)  cyg_uint16  _cyg_fb_xpixelpos16_ ## _fb_ ## _which_; cyg_uint16  _cyg_fb_ypixelpos16_ ## _fb_ ## _which_; cyg_ili9325_lcd_t * _cyg_fb_lcd_ ## _fb_ ## _which_ = NULL

#define CYG_FB_ILI9325_PIXELx_SET_16(_fb_, _which_, _fbaddr_, _stride8_, _x_, _y_)                                               \
    CYG_MACRO_START                                                                                                              \
    _cyg_fb_lcd_ ## _fb_ ## _which_ = (cyg_ili9325_lcd_t *)_fbaddr_;                                                             \
    _cyg_fb_xpixelpos16_ ## _fb_ ## _which_  = (_x_);                                                                            \
    _cyg_fb_ypixelpos16_ ## _fb_ ## _which_  = (_y_);                                                                            \
    CYG_MACRO_END

#define CYG_FB_ILI9325_PIXELx_GET_16(_fb_, _which_, _fbaddr_, _stride8_, _x_ ,_y_)                                               \
    CYG_MACRO_START                                                                                                              \
    _y_ = _cyg_fb_ypixelpos16_ ## _fb_ ## _which_;                                                                               \
    _x_ = _cyg_fb_xpixelpos16_ ## _fb_ ## _which_;                                                                               \
    CYG_MACRO_END

#define CYG_FB_ILI9325_PIXELx_ADDX_16(_fb_, _which_, _stride8_, _incr_)                                                          \
    CYG_MACRO_START                                                                                                              \
    _cyg_fb_xpixelpos16_ ## _fb_ ## _which_ += (_incr_);                                                                         \
    CYG_MACRO_END

#define CYG_FB_ILI9325_PIXELx_ADDY_16(_fb_, _which_, _stride8_, _incr_)                                                          \
    CYG_MACRO_START                                                                                                              \
    _cyg_fb_ypixelpos16_ ## _fb_ ## _which_ += (_incr_);                                                                         \
    CYG_MACRO_END

// ----------------------------------------------------------------------------
// LCD in landscape mode

#define CYG_FB_ILI9325_PIXELx_WRITE_LANDSCAPE_16(_fb_, _which_, _colour_)                                                        \
    CYG_MACRO_START                                                                                                              \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_VGAS, _cyg_fb_xpixelpos16_ ## _fb_ ## _which_ );   \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_HGAS, _cyg_fb_ypixelpos16_ ## _fb_ ## _which_ );   \
    HAL_PLF_LCD_WRITE_IR (_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_WDG);                                              \
    HAL_PLF_LCD_WRITE_RAM(_cyg_fb_lcd_ ## _fb_ ## _which_, _colour_);                                                            \
    CYG_MACRO_END

#define CYG_FB_ILI9325_PIXELx_READ_LANDSCAPE_16(_fb_, _which_)                                                                   \
    ({                                                                                                                           \
    cyg_uint16 _pixel;                                                                                                           \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_VGAS, _cyg_fb_xpixelpos16_ ## _fb_ ## _which_ );   \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_HGAS, _cyg_fb_ypixelpos16_ ## _fb_ ## _which_ );   \
    _pixel = HAL_PLF_LCD_READ_RAM(_lcd);                                                                                         \
    _pixel;                                                                                                                      \
    })

// ----------------------------------------------------------------------------
// LCD in portrait mode

#define CYG_FB_ILI9325_PIXELx_WRITE_PORTRAIT_16(_fb_, _which_, _colour_)                                                         \
    CYG_MACRO_START                                                                                                              \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_HGAS, _cyg_fb_xpixelpos16_ ## _fb_ ## _which_ );   \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_VGAS, _cyg_fb_ypixelpos16_ ## _fb_ ## _which_ );   \
    HAL_PLF_LCD_WRITE_IR (_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_WDG);                                              \
    HAL_PLF_LCD_WRITE_RAM(_cyg_fb_lcd_ ## _fb_ ## _which_, _colour_);                                                            \
    CYG_MACRO_END

#define CYG_FB_ILI9325_PIXELx_READ_PORTRAIT_16(_fb_, _which_)                                                                    \
    ({                                                                                                                           \
    cyg_uint16 _pixel;                                                                                                           \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_HGAS, _cyg_fb_xpixelpos16_ ## _fb_ ## _which_ );   \
    HAL_PLF_LCD_WRITE_REG(_cyg_fb_lcd_ ## _fb_ ## _which_, ILI9325_CONTROLLER_VGAS, _cyg_fb_ypixelpos16_ ## _fb_ ## _which_ );   \
    _pixel = HAL_PLF_LCD_READ_RAM(_lcd);                                                                                         \
    _pixel;                                                                                                                      \
    })

#define CYG_FB_ILI9325_PIXELx_FLUSHABS_16(_fb_, _which_, _x0_, _y0_, _x1_, _y1_)                                                 \
    CYG_MACRO_START                                                                                                              \
    CYG_MACRO_END

#define CYG_FB_ILI9325_PIXELx_FLUSHREL_16(_fb_, _which_, _x0_, _y0_, _dx_, _dy_)                                                 \
    CYG_MACRO_START                                                                                                              \
    CYG_MACRO_END


#endif // CYGONCE_DEVS_FRAMEBUF_ILI9325_DIRECT_INL
