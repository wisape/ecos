//==========================================================================
//
//      readpng.c
//
//      Test PNG library -> display PNG image in framebuffer
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C)  2014 Free Software Foundation, Inc.
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
//#####DESCRIPTIONBEGIN####
//
// Author(s):           ccoutand
// Contributors:
// Date:                2014-05-30
// Purpose:             Test libPNG
// Description:         This test reads a PNG image in ROM FS and display the
//                      result using the eCos framebuf implementation
//                      This test therefore requires FS ROM package and active
//                      framebuffer device which can easily be added using the
//                      synthetic target
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/hal.h>
#include <pkgconf/io_fileio.h>
#include <pkgconf/isoinfra.h>
#include <pkgconf/system.h>
#include <pkgconf/fs_rom.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#include <cyg/fileio/fileio.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/testcase.h>
#include <cyg/infra/diag.h>            // HAL polled output

// Test ROMFS data. Two example data files are generated so that
// the test will work on both big-endian and little-endian targets.
#if (CYG_BYTEORDER == CYG_LSBFIRST)
# include <cyg/romfs/testromfs_le.h>
#else
# include <cyg/romfs/testromfs_be.h>
#endif

//==========================================================================

MTAB_ENTRY( romfs_mte1,
                   "/",
                   "romfs",
                   "",
                   (CYG_ADDRWORD) &filedata[0] );


//==========================================================================

#define SHOW_RESULT( _fn, _res ) \
  diag_printf("<FAIL>: " #_fn "() returned %d %s\n", (int)_res, _res<0?strerror(errno):"");

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cyg/libpng/png.h>

#include <pkgconf/io_framebuf.h>
#include <cyg/io/framebuf.h>

#if defined(CYGDAT_IO_FRAMEBUF_TEST_DEVICE)
# define FB         CYGDAT_IO_FRAMEBUF_TEST_DEVICE
#elif defined(CYGDAT_IO_FRAMEBUF_DEFAULT_TEST_DEVICE)
# define FB         CYGDAT_IO_FRAMEBUF_DEFAULT_TEST_DEVICE
#else
# define NA_MSG "No framebuffer test device selected"
#endif

#ifndef NA_MSG
# if (CYG_FB_DEPTH(FB) < 4)
#  define NA_MSG    "Testcase requires a display depth of at least 4 bits/pixel"
# else
#  define FRAMEBUF  CYG_FB_STRUCT(FB)
# endif
#endif


#define STRING1(_a_) # _a_
#define STRING(_a_) STRING1(_a_)

//==========================================================================
// main
static int
read_png_and_display(FILE *fp)
{
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
   png_infop info_ptr = NULL;
   png_bytep row = NULL, display = NULL;
   int ret = 0;

   if (png_ptr == NULL)
      return ret;

   if (setjmp(png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      if (row != NULL) free(row);
      if (display != NULL) free(display);
      return ret;
   }

   png_init_io(png_ptr, fp);

   info_ptr = png_create_info_struct(png_ptr);

   if (info_ptr == NULL)
       return ret;

   png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_ALWAYS, NULL, 0);

   png_read_info(png_ptr, info_ptr);

   png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
   row = malloc(rowbytes);
   display = malloc(rowbytes);

   if (row == NULL || display == NULL)
      return ret;

   {
     png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
     png_uint_32 width  = png_get_image_width(png_ptr, info_ptr);
     png_uint_32 bit_depth = png_get_bit_depth(png_ptr, info_ptr);
     int nb_channel = 3;

     if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
        nb_channel = 4;
        diag_printf("PNG image with RGBA components\n");
     }

     if( height != FRAMEBUF.fb_height ){
       diag_printf("PNG image height (%d) not equal to FB height, exit\n",
                   height);
       goto out;
     }

     if( width != FRAMEBUF.fb_width ){
       diag_printf("PNG image width (%d) not equal to FB width, exit\n",
                   width );
       goto out;
     }

     if( bit_depth != 8 ){
       diag_printf("PNG image pixel depth not equal to FB depth, exit\n");
       goto out;
     }

     png_start_read_image(png_ptr);
     while (height-- > 0){
        int i,k=0;
        png_read_row(png_ptr, row, display);
        for (i = 0; i < width; i++) {
            cyg_fb_colour color = CYG_FB_MAKE_COLOUR_32BPP_TRUE_0888(row[k], row[k+1], row[k+2]);
            cyg_fb_write_pixel(&FRAMEBUF,(cyg_uint16)i, (cyg_uint16)(FRAMEBUF.fb_height-height), color);
            k=k+nb_channel;
        }
     }
   }
   /* Make sure to read to the end of the file: */
   png_read_end(png_ptr, info_ptr);
   ret = 1;

out:
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   free(row);
   free(display);
   return ret;
}

int main( int argc, char **argv )
{
    int err;
    char address[16];
#if defined(CYGSEM_FILEIO_BLOCK_USAGE)
    struct cyg_fs_block_usage usage;
#endif
    FILE * fd;

    CYG_TEST_INIT();

    diag_printf("<INFO>: Mount ROMFS onto /mnt\n");
    sprintf( address, "%p", (void*)&filedata[0] );
    err = mount( address, "/mnt/", "romfs" );
    if( err < 0 ) SHOW_RESULT( mount, err );

    fd = fopen( "/var/ecos_320x240.png", "r" );
    //fd = fopen( "/var/320x240_landscape.png", "r" );
    if( fd < 0 ) SHOW_RESULT( open, fd );

    diag_printf("Frame buffer %s\n", STRING(FRAMEBUF));
    diag_printf("Depth %d, width %d, height %d\n", FRAMEBUF.fb_depth, FRAMEBUF.fb_width, FRAMEBUF.fb_height);

    if (FRAMEBUF.fb_depth != 32 ){
       diag_printf("This test requires a pixel depth of 32 bits, exit");
       goto done;
    }

    cyg_fb_on(&FRAMEBUF);

    if( read_png_and_display(fd) == 0 )
      diag_printf("PNG image processing has failed\n");

done:
    err = umount( "/" );
    if( err < 0 ) SHOW_RESULT( umount, err );

    CYG_TEST_PASS_FINISH("romfs1");
}

// -------------------------------------------------------------------------
// EOF readpng.c
