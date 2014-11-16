//==========================================================================
//
//      e500mc.s
//
//      HAL support to Freescale QorIQ
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 2012 Free Software Foundation, Inc.
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
// Date:         2012-07-20
// Purpose:
// Description:  Boot page for powerpc E500MC primary core
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#include <pkgconf/hal.h>
#include <cyg/hal/ppc_regs.h>
#include <cyg/hal/arch.inc>
#include <cyg/hal/var_mem.h>
#include <cyg/hal/plf_mem.h>

.extern _start
.extern rom_vectors

// When the e500mc core boots up, the only thing we have is a 4K page configured in
// TLB1:0 from 0xfffff000

    .section ".bootpage","ax"
    .globl  _start_e500mc_primary

_start_e500mc_primary:
#if  (defined(CYG_HAL_STARTUP_ROMRAM) || defined(CYG_HAL_STARTUP_ROM))

#ifdef CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_A004510_A004511
    hal_cpu_erratum_A004510_A004511
#endif

    // Invalide caches
    hal_cpu_invalidate_caches

    // Clear some registers
    hal_cpu_core_init

    // Create TLB 1:63 entry to access boot window in address space = 1
    lwi   r3, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGARC_TLB1_BOOT_PAGE))
    lwi   r4, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_1M) | CYGARC_MAS1_IPROT | CYGARC_MAS1_TS)
#if defined(CYGPKG_REDBOOT)
    lwi   r5, ((CYGPLF_REDBOOT_START & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
#else
    lwi   r5, ((CYGMEM_REGION_rom & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
#endif
    lwi   r6, ((0xfff00000 & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SR)
    li    r7, 0
    mtspr MAS0,r3
    mtspr MAS1,r4
    mtspr MAS2,r5
    mtspr MAS3,r6
    mtspr MAS7,r7
    isync
    msync
    tlbwe

    lis r3, (CYGARC_STARTUP_MSR)@h
    ori r3,r3, (CYGARC_STARTUP_MSR)@l
    lis r4, _start@h
    ori r4,r4, _start@l

#if defined(CYG_HAL_STARTUP_ROMRAM)
#if defined(CYGPKG_REDBOOT)
      lwi   r5, CYGPLF_REDBOOT_START
#else
      lwi   r5, CYGMEM_REGION_rom
#endif
      add  r4,r4,r5
#endif

    mtspr   SRR0,r4
    mtspr   SRR1,r3
    rfi

#endif
