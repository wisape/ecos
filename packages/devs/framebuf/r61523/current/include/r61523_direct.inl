#ifndef CYGONCE_DEVS_FRAMEBUF_R61523_DIRECT_INL
#define CYGONCE_DEVS_FRAMEBUF_R61523_DIRECT_INL
//=============================================================================
//
//      r61523_direct.inl
//
//      R61523 LCD controller framebuffer driver
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
// Purpose:     R61523 LCD controller framebuffer driver
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

# define CYG_FB_SET_WINDOW_AREA( _lcd , _xs_, _xe_, _ys_, _ye_ )                      \
    CYG_MACRO_START                                                                   \
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_SET_COLUMN_ADDRESS);                           \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_xs_ >> 8));                                        \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_xs_ & 0xff));                                      \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_xe_ >> 8));                                        \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_xe_ & 0xff));                                      \
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_SET_PAGE_ADDRESS);                             \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_ys_ >> 8));                                        \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_ys_ & 0xff));                                      \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_ye_ >> 8));                                        \
    HAL_PLF_LCD_WRITE_DATA(_lcd, (_ye_ & 0xff));                                      \
    CYG_MACRO_END

#ifndef CONVERT_COLOUR_HOST2TARGET
# error "Missing CONVERT_COLOUR_HOST2TARGET from platform HAL"
#endif

#ifndef CONVERT_COLOUR_TARGET2HOST
# error "Missing CONVERT_COLOUR_TARGET2HOST from platform HAL"
#endif

// ----------------------------------------------------------------------------
// LCD in landscape mode

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_pixel_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_fb_colour _colour_))
{
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    cyg_fb_colour _c = CONVERT_COLOUR_HOST2TARGET(_colour_);
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, _x_, _y_, _y_);
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_WRITE);
    HAL_PLF_LCD_WRITE_DATA(_lcd,       (_c >> 16));
    HAL_PLF_LCD_WRITE_DATA(_lcd, (cyg_uint16)(_c));
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(cyg_fb_colour,
                         cyg_fb_r61523_read_pixel_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_))
{
    cyg_uint16  _pixel_h_;
    cyg_uint16  _pixel_l_;
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, _x_, _y_, _y_);
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_READ);
    HAL_PLF_LCD_READ_DATA(_lcd);
    _pixel_h_ = HAL_PLF_LCD_READ_DATA(_lcd);
    _pixel_l_ = HAL_PLF_LCD_READ_DATA(_lcd);
    return CONVERT_COLOUR_TARGET2HOST(((_pixel_h_ << 16) | (_pixel_l_)));
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_hline_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_uint16 _len;
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    cyg_fb_colour _c = CONVERT_COLOUR_HOST2TARGET(_colour_);
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, (_x_+_len_-1), _y_, _y_);
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_WRITE);
    for (_len = 0; _len < _len_; _len++) {
      HAL_PLF_LCD_WRITE_DATA(_lcd,       (_c >> 16));
      HAL_PLF_LCD_WRITE_DATA(_lcd, (cyg_uint16)(_c));
    }
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_vline_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_uint16 _len;
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    cyg_fb_colour _c = CONVERT_COLOUR_HOST2TARGET(_colour_);
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, _x_, _y_, (_y_+_len_-1));
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_WRITE);
    for (_len = 0; _len < _len_; _len++) {
      HAL_PLF_LCD_WRITE_DATA(_lcd,       (_c >> 16));
      HAL_PLF_LCD_WRITE_DATA(_lcd, (cyg_uint16)(_c));
    }
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_fill_block_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, cyg_fb_colour _colour_))
{
    cyg_uint16 _width, _height;
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    cyg_fb_colour _c = CONVERT_COLOUR_HOST2TARGET(_colour_);
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, (_x_+_width_-1), _y_, (_y_+_height_-1));
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_WRITE);
    for (_height = _height_ ; _height; _height--) {
      for (_width  = _width_ ; _width; _width--) {
        HAL_PLF_LCD_WRITE_DATA(_lcd,       (_c >> 16));
        HAL_PLF_LCD_WRITE_DATA(_lcd, (cyg_uint16)(_c));
      }
    }
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_block_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, const void* _source_, cyg_ucount16 _offset_, cyg_ucount16 _source_stride8_))
{
    cyg_fb_colour _c;
    cyg_uint16 _width, _height;
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    cyg_uint32* _src32_     = ((cyg_uint32*) _source_) + _offset_;
    cyg_uint32* _current32  = _src32_;
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, (_x_+_width_-1), _y_, (_y_+_height_-1));
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_WRITE);
    for (_height = _height_ ; _height; _height--) {
        for (_width  = _width_ ; _width; _width--) {
            _c = CONVERT_COLOUR_HOST2TARGET(*_current32);
            HAL_PLF_LCD_WRITE_DATA(_lcd,       (_c >> 16));
            HAL_PLF_LCD_WRITE_DATA(_lcd, (cyg_uint16)(_c));
            _current32++;
        }
        _current32 = _src32_ = (cyg_uint32*)(((cyg_uint8*)_src32_) + _source_stride8_);
    }
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_read_block_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, void* _dest_, cyg_ucount16 _offset_, cyg_ucount16 _dest_stride8_))
{
    cyg_r61523_lcd_t * _lcd = (cyg_r61523_lcd_t *)_fbaddr_;
    cyg_uint32* _dst32_     = ((cyg_uint32*) _dest_) + _offset_;
    cyg_uint32* _current32  = _dst32_;
    cyg_uint16 _width, _height, _pixel_h_, _pixel_l_;
    CYG_FB_SET_WINDOW_AREA(_lcd, _x_, (_x_+_width_-1), _y_, (_y_+_height_-1));
    HAL_PLF_LCD_WRITE_CMD(_lcd, R61523_MEMORY_READ);
    HAL_PLF_LCD_READ_DATA(_lcd);
    for (_height = _height_ ; _height; _height--) {
        for (_width  = _width_ ; _width; _width--) {
            _pixel_h_ = HAL_PLF_LCD_READ_DATA(_lcd);
            _pixel_l_ = HAL_PLF_LCD_READ_DATA(_lcd);
            *_current32 = CONVERT_COLOUR_TARGET2HOST(((_pixel_h_ << 16) | (_pixel_l_)));
            _current32++;
        }
        _current32 = _dst32_ = (cyg_uint32*)(((cyg_uint8*)_dst32_) + _dest_stride8_);
    }
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_move_block_landscape_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height, cyg_ucount16 new_x, cyg_ucount16 new_y))
{
    // TODO
}

// ----------------------------------------------------------------------------
// LCD in portrait mode

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_pixel_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_fb_colour _colour_))
{
    cyg_fb_r61523_write_pixel_landscape_32_inl(_fbaddr_, _stride_, _x_, _y_, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(cyg_fb_colour,
                         cyg_fb_r61523_read_pixel_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_))
{
    return cyg_fb_r61523_read_pixel_landscape_32_inl(_fbaddr_, _stride_, _x_, _y_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_hline_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_fb_r61523_write_hline_landscape_32_inl(_fbaddr_, _stride_, _x_, _y_, _len_, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_vline_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride_, cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _len_, cyg_fb_colour _colour_))
{
    cyg_fb_r61523_write_vline_landscape_32_inl(_fbaddr_, _stride_, _x_, _y_, _len_, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_fill_block_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, cyg_fb_colour _colour_))
{
    cyg_fb_r61523_fill_block_landscape_32_inl(_fbaddr_, _stride8_, _x_, _y_, _width_, _height_, _colour_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_write_block_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, const void* _source_, cyg_ucount16 _offset_, cyg_ucount16 _source_stride8_))
{
    cyg_fb_r61523_write_block_landscape_32_inl(_fbaddr_, _stride8_, _x_, _y_, _width_, _height_, _source_, _offset_, _source_stride8_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_read_block_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 _x_, cyg_ucount16 _y_, cyg_ucount16 _width_, cyg_ucount16 _height_, void* _dest_, cyg_ucount16 _offset_, cyg_ucount16 _dest_stride8_))
{
    cyg_fb_r61523_read_block_landscape_32_inl(_fbaddr_, _stride8_, _x_, _y_, _width_, _height_, _dest_, _offset_, _dest_stride8_);
}

CYG_FB_DEVS_FRAMEBUF_INLINE_FN(void,
                         cyg_fb_r61523_move_block_portrait_32_inl,
                        (void* _fbaddr_, cyg_ucount16 _stride8_,
                         cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width, cyg_ucount16 height, cyg_ucount16 new_x, cyg_ucount16 new_y))
{
    // TODO
}

// ----------------------------------------------------------------------------

#define CYG_FB_R61523_PIXELx_VAR_32(_fb_, _which_)  cyg_uint16  _cyg_fb_xpixelpos32_ ## _fb_ ## _which_; cyg_uint16  _cyg_fb_ypixelpos32_ ## _fb_ ## _which_; cyg_r61523_lcd_t * _cyg_fb_lcd_ ## _fb_ ## _which_ = NULL

#define CYG_FB_R61523_PIXELx_SET_32(_fb_, _which_, _fbaddr_, _stride8_, _x_, _y_)                                                \
    CYG_MACRO_START                                                                                                              \
    _cyg_fb_lcd_ ## _fb_ ## _which_ = (cyg_r61523_lcd_t *)_fbaddr_;                                                              \
    _cyg_fb_xpixelpos32_ ## _fb_ ## _which_  = (_x_);                                                                            \
    _cyg_fb_ypixelpos32_ ## _fb_ ## _which_  = (_y_);                                                                            \
    CYG_MACRO_END

#define CYG_FB_R61523_PIXELx_GET_32(_fb_, _which_, _fbaddr_, _stride8_, _x_ ,_y_)                                                \
    CYG_MACRO_START                                                                                                              \
    _y_ = _cyg_fb_ypixelpos32_ ## _fb_ ## _which_;                                                                               \
    _x_ = _cyg_fb_xpixelpos32_ ## _fb_ ## _which_;                                                                               \
    CYG_MACRO_END

#define CYG_FB_R61523_PIXELx_ADDX_32(_fb_, _which_, _stride8_, _incr_)                                                           \
    CYG_MACRO_START                                                                                                              \
    _cyg_fb_xpixelpos32_ ## _fb_ ## _which_ += (_incr_);                                                                         \
    CYG_MACRO_END

#define CYG_FB_R61523_PIXELx_ADDY_32(_fb_, _which_, _stride8_, _incr_)                                                           \
    CYG_MACRO_START                                                                                                              \
    _cyg_fb_ypixelpos32_ ## _fb_ ## _which_ += (_incr_);                                                                         \
    CYG_MACRO_END

// ----------------------------------------------------------------------------
// LCD in landscape mode

#define CYG_FB_R61523_PIXELx_WRITE_LANDSCAPE_32(_fb_, _which_, _colour_)                                                         \
    CYG_MACRO_START                                                                                                              \
    cyg_fb_colour _c = CONVERT_COLOUR_HOST2TARGET(_colour_);                                                                     \
    cyg_uint16 _x = _cyg_fb_xpixelpos32_ ## _fb_ ## _which_;                                                                     \
    cyg_uint16 _y = _cyg_fb_ypixelpos32_ ## _fb_ ## _which_;                                                                     \
    CYG_FB_SET_WINDOW_AREA(_cyg_fb_lcd_ ## _fb_ ## _which_, _x, _x, _y , _y);                                                    \
    HAL_PLF_LCD_WRITE_CMD(_cyg_fb_lcd_ ## _fb_ ## _which_, R61523_MEMORY_WRITE);                                                 \
    HAL_PLF_LCD_WRITE_DATA(_cyg_fb_lcd_ ## _fb_ ## _which_,       (_c >> 16));                                                   \
    HAL_PLF_LCD_WRITE_DATA(_cyg_fb_lcd_ ## _fb_ ## _which_, (cyg_uint16)(_c));                                                   \
    CYG_MACRO_END

#define CYG_FB_R61523_PIXELx_READ_LANDSCAPE_32(_fb_, _which_)                                                                    \
    ({                                                                                                                           \
    cyg_fb_colour _pixel;                                                                                                        \
    cyg_uint16 _pixel_h_, _pixel_l_;                                                                                             \
    cyg_uint16 _x = _cyg_fb_xpixelpos32_ ## _fb_ ## _which_;                                                                     \
    cyg_uint16 _y = _cyg_fb_ypixelpos32_ ## _fb_ ## _which_;                                                                     \
    CYG_FB_SET_WINDOW_AREA(_cyg_fb_lcd_ ## _fb_ ## _which_, _x, _x, _y , _y);                                                    \
    HAL_PLF_LCD_WRITE_CMD(_cyg_fb_lcd_ ## _fb_ ## _which_, R61523_MEMORY_READ);                                                  \
    HAL_PLF_LCD_READ_DATA(_cyg_fb_lcd_ ## _fb_ ## _which_);                                                                      \
    _pixel_h_ = HAL_PLF_LCD_READ_DATA(_cyg_fb_lcd_ ## _fb_ ## _which_);                                                          \
    _pixel_l_ = HAL_PLF_LCD_READ_DATA(_cyg_fb_lcd_ ## _fb_ ## _which_);                                                          \
    _pixel = CONVERT_COLOUR_TARGET2HOST(((_pixel_h_ << 16) | (_pixel_l_)));                                                      \
    _pixel;                                                                                                                      \
    })

// ----------------------------------------------------------------------------
// LCD in portrait mode

#define CYG_FB_R61523_PIXELx_WRITE_PORTRAIT_32(_fb_, _which_, _colour_)                                                                                \
    CYG_FB_R61523_PIXELx_WRITE_LANDSCAPE_32(_fb_, _which_, _colour_)

#define CYG_FB_R61523_PIXELx_READ_PORTRAIT_32(_fb_, _which_)                                                                                           \
    CYG_FB_R61523_PIXELx_READ_LANDSCAPE_32(_fb_, _which_) 

#define CYG_FB_R61523_PIXELx_FLUSHABS_32(_fb_, _which_, _x0_, _y0_, _x1_, _y1_)                                                                        \
    CYG_MACRO_START                                                                                                                                    \
    CYG_MACRO_END

#define CYG_FB_R61523_PIXELx_FLUSHREL_32(_fb_, _which_, _x0_, _y0_, _dx_, _dy_)                                                                        \
    CYG_MACRO_START                                                                                                                                    \
    CYG_MACRO_END


#endif // CYGONCE_DEVS_FRAMEBUF_R61523_DIRECT_INL
