/*==========================================================================
//
//      platform.inc
//
//      Platform specific asm definitions
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
//#####DESCRIPTIONBEGIN####
//
// Author(s):    ccoutand
// Date:         2011-02-01
// Description:  
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/hal.h>

    .macro hal_plf_startup_init
#ifdef CYG_HAL_STARTUP_ROMRAM
        # Copy image from ROM to RAM
        addi    r3, r0, CYGHWR_MEMORY_FLASH_BASE_ADDRESS
        addi    r4, r0, CYGHWR_MEMORY_SDRAM_BASE_ADDRESS
        addi    r5, r0, __ram_data_end
        cmpu    r18, r4, r5                   // jump if no data to move
        blei    r18, 2f
        addik   r19, r0, 4
        rsub    r3, r19, r3                   // loop adjustments
        rsub    r4, r19, r4
1:
        lwi     r18, r3, 4
        swi     r18, r4, 4
        addik   r4,  r4, 4
        cmpu    r18, r4, r5                   // jump if no data to move
        blei    r18, 2f
        brid    1b
        addik   r3,  r3, 4
2:
#endif
    .endm

//==========================================================================
// EOF platform.inc