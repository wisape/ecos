//=============================================================================
//
//      plf_mem.h
//
//      Platform HAL memory details
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2012 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   ccoutand
// Contributors:
// Date:        2012-07-20
// Purpose:     
// Description: COMX-P4080 platform memory settings
// Usage:       
//
//####DESCRIPTIONEND####
//
//=============================================================================

#ifndef CYGONCE_PLF_MEM_H
#define CYGONCE_PLF_MEM_H

#ifdef CYGPKG_HAL_SMP_SUPPORT
#define CYGPKG_HAL_SMP_MAIN_CPU_ID                0x00000000
#endif

// Flash configuration
#define CYGPLF_MEM_FLASH_BASE                     0xf8000000
#define CYGPLF_MEM_FLASH_SIZE                     0x08000000  // 128 MB

// IMMR configuration
#define CYGPLF_MEM_CCSRBAR_SIZE                   0x00100000  //   1 MB

// Limit to 2GB
#define CYGPLF_MEM_DDR1_RAM_SIZE                  0x40000000  // 1024 MB
#define CYGPLF_MEM_DDR2_RAM_SIZE                  0x40000000  // 1024 MB

// RAM configuration (Maximum memory is 4GB, only 2GB used by eCos)
#define CYGPLF_MEM_RAM_BASE                       0x00000000
#define CYGPLF_MEM_RAM_SIZE                       ( CYGPLF_MEM_DDR1_RAM_SIZE+CYGPLF_MEM_DDR2_RAM_SIZE )

#define CYGPLF_MEM_RAM_BASE_TOP_HALF              ( CYGPLF_MEM_RAM_BASE + CYGPLF_MEM_DDR1_RAM_SIZE )

#if CYGPLF_MEM_RAM_SIZE==0
#error No RAM memory configured for this platform
#endif

#define CYGPLF_MEM_CSBAR_TLB    59
#define CYGPLF_MEM_FLASH_TLB    60
#define CYGPLF_MEM_DDRAM_1_TLB  61  // 2 TLBs required to support 2GB memory
#define CYGPLF_MEM_DDRAM_2_TLB  62

// Place redboot at memory end 
#define CYGPLF_REDBOOT_START                      0xfff00000


#endif //  CYGONCE_PLF_MEM_H
// End of plf_mem.h
