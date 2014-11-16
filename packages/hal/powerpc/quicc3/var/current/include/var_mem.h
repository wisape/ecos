//==========================================================================
//
//      var_mem.h
//
//      Variant HAL MMU register definition
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 
// 2008, 2009, 2010 Free Software Foundation, Inc.
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
// Contributors: 
// Date:         2009-11-01
// Purpose:      
// Description:  Variant HAL MMU register definition for Freescale Quicc III
//               target
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_VAR_MEM_H
#define CYGONCE_VAR_MEM_H

#include <cyg/hal/plf_mem.h>

// 16 TLB1 entry available
#define CYGARC_TLB1_MAX          16 
#define CYGARC_TLB0_MAX          4

// 15 TLB1 available during boot, TLB1:15 is used
// for mapping the boot page
#define CYGARC_TLB1_BOOT_MAX     15 
#define CYGARC_TLB1_BOOT_PAGE    15

// 12 TLB1 available for the application, TLB1:12 / TLB1:13 / TLB1:14 are 
// used for mapping the RAM, FLASH and CCSBAR space
#ifndef CYGARC_TLB1_APP_MAX
#define CYGARC_TLB1_APP_MAX      12 
#endif

#define CYGARC_CYG_MEM_1KB       1024
#define CYGARC_TLB_MIN_KB_SIZE   4 /* 4 KB */

// Possible page size
#define CYGARC_TLB_PAGES_4K      1
#define CYGARC_TLB_PAGES_16K     2
#define CYGARC_TLB_PAGES_64K     3
#define CYGARC_TLB_PAGES_256K    4
#define CYGARC_TLB_PAGES_1M      5
#define CYGARC_TLB_PAGES_4M      6
#define CYGARC_TLB_PAGES_16M     7
#define CYGARC_TLB_PAGES_64M     8
#define CYGARC_TLB_PAGES_256M    9
#define CYGARC_TLB_PAGES_1G     10
#define CYGARC_TLB_PAGES_4G     11

// MAS register definition
#define CYGARC_MAS0_TLBSEL_TLB0             0x00000000                          
#define CYGARC_MAS0_TLBSEL_TLB1             0x10000000                          
#define CYGARC_MAS0_ESEL(x)                 ((x << 16) & 0x0FFF0000)            

#define CYGARC_MAS1_V                       0x80000000                          
#define CYGARC_MAS1_IPROT                   0x40000000                          
#define CYGARC_MAS1_TID(x)                  ((x << 16) & 0x3FFF0000)
#define CYGARC_MAS1_TS                      0x00001000
#define CYGARC_MAS1_TSIZE(x)                ((x << 8) & 0x00000F00)

#define CYGARC_MAS2_EPN_MASK                 0xFFFFF000                          
#define CYGARC_MAS2_X0                       0x00000040
#define CYGARC_MAS2_X1                       0x00000020
#define CYGARC_MAS2_W                        0x00000010                          
#define CYGARC_MAS2_I                        0x00000008                          
#define CYGARC_MAS2_M                        0x00000004                          
#define CYGARC_MAS2_G                        0x00000002                          
#define CYGARC_MAS2_E                        0x00000001                          

#define CYGARC_MAS3_RPN_MASK                 0xFFFFF000                          
#define CYGARC_MAS3_U0                       0x00000200                          
#define CYGARC_MAS3_U1                       0x00000100                          
#define CYGARC_MAS3_U2                       0x00000080                          
#define CYGARC_MAS3_U3                       0x00000040                          
#define CYGARC_MAS3_UX                       0x00000020                          
#define CYGARC_MAS3_SX                       0x00000010                          
#define CYGARC_MAS3_UW                       0x00000008                          
#define CYGARC_MAS3_SW                       0x00000004                          
#define CYGARC_MAS3_UR                       0x00000002                          
#define CYGARC_MAS3_SR                       0x00000001                          

#define CYGARC_STARTUP_MSR (CYGARC_REG_MSR_DE | CYGARC_REG_MSR_ME | CYGARC_REG_MSR_IS | CYGARC_REG_MSR_DS)

#endif //  CYGONCE_VAR_MEM_H
// End of var_mem.h
