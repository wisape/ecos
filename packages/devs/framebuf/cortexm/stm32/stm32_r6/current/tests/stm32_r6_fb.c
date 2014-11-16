//==========================================================================
//
//      fbmacro.c
//
//      Framebuffer testcase
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
//###DESCRIPTIONBEGIN####
//
// Author(s):     bartv
// Date:          2008-10-06
//
//###DESCRIPTIONEND####
//========================================================================

#include <pkgconf/system.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/testcase.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
#include <pkgconf/io_framebuf.h>
#include <cyg/io/framebuf.h>

#if defined(CYGDAT_IO_FRAMEBUF_TEST_DEVICE)
# define FRAMEBUF   CYGDAT_IO_FRAMEBUF_TEST_DEVICE
#elif defined(CYGDAT_IO_FRAMEBUF_DEFAULT_TEST_DEVICE)
# define FRAMEBUF   CYGDAT_IO_FRAMEBUF_DEFAULT_TEST_DEVICE
#else
# define NA_MSG "No framebuffer test device selected"
#endif

#ifdef NA_MSG
void
cyg_start(void)
{
    CYG_TEST_INIT();
    CYG_TEST_NA(NA_MSG);
}
#else

#define STRING1(_a_) # _a_
#define STRING(_a_) STRING1(_a_)

// The colours used by this test code. Default to the standard palette
// but if running on a true colour display then adjust.
static cyg_ucount32 colours[16]  = {
    CYG_FB_DEFAULT_PALETTE_BLACK,
    CYG_FB_DEFAULT_PALETTE_BLUE,
    CYG_FB_DEFAULT_PALETTE_GREEN,
    CYG_FB_DEFAULT_PALETTE_CYAN,
    CYG_FB_DEFAULT_PALETTE_RED,
    CYG_FB_DEFAULT_PALETTE_MAGENTA,
    CYG_FB_DEFAULT_PALETTE_BROWN,
    CYG_FB_DEFAULT_PALETTE_LIGHTGREY,
    CYG_FB_DEFAULT_PALETTE_DARKGREY,
    CYG_FB_DEFAULT_PALETTE_LIGHTBLUE,
    CYG_FB_DEFAULT_PALETTE_LIGHTGREEN,
    CYG_FB_DEFAULT_PALETTE_LIGHTCYAN,
    CYG_FB_DEFAULT_PALETTE_LIGHTRED,
    CYG_FB_DEFAULT_PALETTE_LIGHTMAGENTA,
    CYG_FB_DEFAULT_PALETTE_YELLOW,
    CYG_FB_DEFAULT_PALETTE_WHITE
};

#define BLACK        colours[0x00]
#define BLUE         colours[0x01]
#define GREEN        colours[0x02]
#define CYAN         colours[0x03]
#define RED          colours[0x04]
#define MAGENTA      colours[0x05]
#define BROWN        colours[0x06]
#define LIGHTGREY    colours[0x07]
#define DARKGREY     colours[0x08]
#define LIGHTBLUE    colours[0x09]
#define LIGHTGREEN   colours[0x0A]
#define LIGHTCYAN    colours[0x0B]
#define LIGHTRED     colours[0x0C]
#define LIGHTMAGENTA colours[0x0D]
#define YELLOW       colours[0x0E]
#define WHITE        colours[0x0F]

static void
reset_colours_to_true(void)
{
#if (CYG_FB_FLAGS0_TRUE_COLOUR & CYG_FB_FLAGS0(FRAMEBUF))
    int i;
    
    for (i = 0; i < 16; i++) {
        colours[i] = CYG_FB_MAKE_COLOUR(FRAMEBUF,
                                        cyg_fb_palette_vga[i + i + i], cyg_fb_palette_vga[i + i + i + 1],cyg_fb_palette_vga[i + i + i + 2]);
    }
#endif
}

void
cyg_start(void)
{
    int             i, j;
    int             result;

#define DEPTH   CYG_FB_DEPTH(FRAMEBUF)
#define WIDTH   CYG_FB_WIDTH(FRAMEBUF)
#define HEIGHT  CYG_FB_HEIGHT(FRAMEBUF)

#define GRID_X_SIZE             625
#define GRID_Y_SIZE             325
#define GRID_ABSOLUTE_X_POS     5
#define GRID_ABSOLUTE_Y_POS     19
#define GRID_STEP               25
#define MAX_X                   (GRID_X_SIZE + GRID_ABSOLUTE_X_POS)
#define MAX_Y                   (GRID_Y_SIZE + GRID_ABSOLUTE_Y_POS)

    CYG_TEST_INIT();

    diag_printf("Frame buffer %s\n", STRING(FRAMEBUF));
    diag_printf("Depth %d, width %d, height %d\n", DEPTH, WIDTH, HEIGHT);

    result = CYG_FB_ON(FRAMEBUF);

    if (CYG_FB_FLAGS0(FRAMEBUF) & CYG_FB_FLAGS0_TRUE_COLOUR) {
        reset_colours_to_true();
    }

    // A white background
    CYG_FB_FILL_BLOCK(FRAMEBUF, 0, 0, WIDTH, HEIGHT, WHITE);
    CYG_FB_FILL_BLOCK(FRAMEBUF, 0, 0, WIDTH, HEIGHT, WHITE);
    // A back grid
    for (j = GRID_ABSOLUTE_Y_POS; j <= MAX_Y; j += GRID_STEP)
    {
       for (i = GRID_ABSOLUTE_X_POS; i < MAX_X + 1; i += 5)
       {
          CYG_FB_WRITE_PIXEL(FRAMEBUF, i, j, BLACK);
       }
    }
    for (i = GRID_ABSOLUTE_X_POS; i <= MAX_X; i += GRID_STEP)
    {
       for (j = GRID_ABSOLUTE_Y_POS; j <= MAX_Y; j += 5)
       {
           CYG_FB_WRITE_PIXEL(FRAMEBUF, i, j, BLACK);
       }
    }
    CYG_TEST_EXIT("Done");
}
#endif  // NA_MSG
