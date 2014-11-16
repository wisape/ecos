//==========================================================================
//
//      e500_secondary.s
//
//      HAL support to Freescale Quicc III based frescale CPUs
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
// Description:  Boot page for powerpc E500 secondary core
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#include <cyg/hal/ppc_regs.h>
#include <cyg/hal/arch.inc>
#include <cyg/hal/var_mem.h>
#include <cyg/hal/plf_mem.h>

.extern _start

#ifdef CYGPKG_HAL_SMP_SUPPORT
    .align 12
    .globl _cyg_hal_slave_trampoline
_cyg_hal_slave_trampoline:

    // Invalide cache
    hal_cpu_invalidate_L1_cache

    // Clear some registers
    hal_cpu_core_init

    // Create TLB 1:15 entry to access boot window in address space = 1
    lwi   r3, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGARC_TLB1_BOOT_PAGE))
    mtspr MAS0,r3
    lwi   r3, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_16M) | CYGARC_MAS1_IPROT | CYGARC_MAS1_TS)
    mtspr MAS1,r3
#if defined(CYGPKG_REDBOOT) && defined(CYG_HAL_STARTUP_ROM)
    lwi   r3, ((CYGPLF_REDBOOT_START & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    mtspr MAS2,r3
    lwi   r3, ((CYGPLF_REDBOOT_START & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
    mtspr MAS3,r3
#elif defined(CYG_HAL_STARTUP_ROM)
    lwi   r3, ((CYGMEM_REGION_rom & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    mtspr MAS2,r3
    lwi   r3, ((CYGMEM_REGION_rom & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
    mtspr MAS3,r3
#else
    lwi   r3, ((CYGMEM_REGION_ram & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    mtspr MAS2,r3
    lwi   r3, ((CYGMEM_REGION_ram & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
    mtspr MAS3,r3
#endif
    isync
    msync
    tlbwe

    lis r3, (CYGARC_STARTUP_MSR)@h
    ori r3,r3, (CYGARC_STARTUP_MSR)@l
    lis r4, _start@h
    ori r4,r4, _start@l

#if defined(CYGPKG_REDBOOT) && defined(CYG_HAL_STARTUP_ROM)
    lwi r5, CYGPLF_REDBOOT_START
#elif defined(CYG_HAL_STARTUP_ROM)
    lwi r5, CYGMEM_REGION_rom
#else
    lwi r5, CYGMEM_REGION_ram
#endif
    add r4,r4,r5

    mtspr SRR0,r4
    mtspr SRR1,r3
    rfi

    // Fill rest of the 4K page with 0
_cyg_hal_slave_trampoline_end:
    .rept (4096 - 4 - (_cyg_hal_slave_trampoline_end-_cyg_hal_slave_trampoline))
    .byte 0
    .endr
_resetvect_e500_secondary:
    b _cyg_hal_slave_trampoline

#endif
