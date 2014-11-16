# ====================================================================
#
#      convert_png2rgb.py
#
#      Utility program to convert PNG image to RGB array
#
# ====================================================================
# ####ECOSGPLCOPYRIGHTBEGIN####                                             
# -------------------------------------------                               
# This file is part of eCos, the Embedded Configurable Operating System.    
# Copyright (C) 2014 Free Software Foundation, Inc.
#
# eCos is free software; you can redistribute it and/or modify it under     
# the terms of the GNU General Public License as published by the Free      
# Software Foundation; either version 2 or (at your option) any later       
# version.                                                                  
#
# eCos is distributed in the hope that it will be useful, but WITHOUT       
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License     
# for more details.                                                         
#
# You should have received a copy of the GNU General Public License         
# along with eCos; if not, write to the Free Software Foundation, Inc.,     
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             
#
# As a special exception, if other files instantiate templates or use       
# macros or inline functions from this file, or you compile this file       
# and link it with other works to produce a work based on this file,        
# this file does not by itself cause the resulting work to be covered by    
# the GNU General Public License. However the source code for this file     
# must still be made available in accordance with section (3) of the GNU    
# General Public License v2.                                                
#
# This exception does not invalidate any other reasons why a work based     
# on this file might be covered by the GNU General Public License.          
# -------------------------------------------                               
# ####ECOSGPLCOPYRIGHTEND####                                               
# ====================================================================
######DESCRIPTIONBEGIN####
#
# Author(s):     ccoutand
# Date:          2014-05-29
#
#####DESCRIPTIONEND####
#========================================================================

from PIL import Image
import sys
import struct
import numpy as np

file_in           = "320x240_portrait.png"
file_out          = "320x240_portrait.h"
format_out        = "32BPP_TRUE_0888"          # "16BPP_TRUE_565"
target_endianness = "little_endian"            # "big_endian"

img = Image.open(file_in)
fid = open(file_out, "w");

pixel = img.load()

fid.write("const cyg_uint8 pixel_array[] = {\n")

print img.size[0], img.size[1]

def uint16_to_byte(uint16):
    uint8_h = (uint16 / 256)
    uint8_l = (uint16 % 256)
    return [uint8_h, uint8_l]

def uint32_to_byte(uint32):
    uint16_h = uint32 / 65536
    uint16_l = uint32 % 65536
    uint8_hh = (uint16_h / 256)
    uint8_hl = (uint16_h % 256)
    uint8_lh = (uint16_l / 256)
    uint8_ll = (uint16_l % 256)
    return [uint8_hh, uint8_hl, uint8_lh, uint8_ll]

def write_byte(byte, endianness, fid):
    if target_endianness == "little_endian":
        byte = byte[::-1]
    for i in range(len(byte)):
        fid.write(hex(byte[i]))
        fid.write(', ')

cnt = 0

if format_out == "32BPP_TRUE_0888":
    cnt_max = 3
else:
    cnt_max = 7

for j in range(img.size[1]):    # for every pixel:
    for i in range(img.size[0]):
        if format_out == "32BPP_TRUE_0888":
            r = pixel[i,j][0]
            g = pixel[i,j][1]
            b = pixel[i,j][2]
            value = r * 65536 + g * 256 + b
        elif format_out == "16BPP_TRUE_565":
            r = pixel[i,j][0] / 8
            g = pixel[i,j][1] / 4
            b = pixel[i,j][2] / 8
            value = r * 2048 + g * 32 + b
        else:
            sys.exit("Unknown output format\n")
        byte = uint32_to_byte(value)
        write_byte(byte, target_endianness, fid)
        if cnt == cnt_max:
            cnt = 0
            fid.write('\n')
        else:
            cnt = cnt + 1

fid.write('};')

print "conversion completed"
fid.close()
