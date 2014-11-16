/*==========================================================================
//
//      variant.inc
//
//      Variant specific asm definitions
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

#include <pkgconf/hal_mb_generic.h>

#include <cyg/hal/arch.inc>
#include <cyg/hal/platform.inc>

##-----------------------------------------------------------------------------
## Variant CPU initialization
##
    .macro hal_cpu_init
       # Shall I set the MSR?, it seems that the default value
       # at startup is selected in HW
       msrclr r3, CYGARC_REG_MSR_IE
    .endm

##-----------------------------------------------------------------------------
## Push CPU registers on stack
##
    .macro hal_variant_save sp
    .endm

    .macro hal_variant_load sp
    .endm

##-----------------------------------------------------------------------------
## Decode interrupt
##

#ifdef CYGPKG_HAL_MICROBLAZE_IPC
    .macro  hal_intc_decode dreg,state
       addik   \dreg, r0, CYGPKG_HAL_MICROBLAZE_IPC_BLOCK_BASE
       lwi     \dreg, \dreg, CYGHWR_HAL_MB_XPS_INTC_IVR
       swi     \dreg, \state, CYGARC_MBREG_VECTOR
       addk    \dreg, \dreg, \dreg   # shift vector by 2 bits ( * 4) 
       addk    \dreg, \dreg, \dreg
    .endm
#else
     # no interrupt controller, return 0
    .macro  hal_intc_decode dreg,state
       addik   \dreg, r0, 0
       swi     \dreg, \state, CYGARC_MBREG_VECTOR
    .endm
#endif
//==========================================================================
// EOF variant.inc

