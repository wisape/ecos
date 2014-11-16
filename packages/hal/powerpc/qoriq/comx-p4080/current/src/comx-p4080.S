//==========================================================================
//
//      COMX-P4080.s
//
//      HAL initialisation
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
// Description:  Initialisation for COMX-P4080 platform from Emerson
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================


#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#include <pkgconf/hal.h>
#include <cyg/hal/arch.inc>
#include <cyg/hal/plf_regs.h>
#include <cyg/hal/ppc_regs.h>
#include <cyg/hal/var_mem.h>
#include <cyg/hal/plf_mem.h>

    .extern _hal_hardware_init_done

    .globl  hal_hardware_init

hal_hardware_init:

#if  (defined(CYG_HAL_STARTUP_ROMRAM) || defined(CYG_HAL_STARTUP_ROM))

#ifdef CYGPKG_HAL_SMP_SUPPORT
    // Get CPU ID, only core0 initialised the IOs
    lwi   r21, CYGPKG_HAL_SMP_MAIN_CPU_ID
    mfspr r20, PIR
    cmpw  r20, r21
    bgt   hal_MMU_init_basics_asm
#endif

    // Rellocate CCSRBAR space
hal_rellocate_ccsrbar_asm:

    // Create TLB 0:0 entry to access the new boot CCSRBAR space place at
    // 0xd0000000 from address 0xd0000000
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB0 | CYGARC_MAS0_ESEL(0))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_CCSBAR_BOOT_TLB_SIZE) | CYGARC_MAS1_IPROT | CYGARC_MAS1_TS)
    lwi   r12, ((CYGARC_IMM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGARC_IMM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    // Create TLB 0:1 entry to access the old boot CCSRBAR space place at
    // 0xfe000000 from address 0xd0001000
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB0 | CYGARC_MAS0_ESEL(1))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_CCSBAR_BOOT_TLB_SIZE) | CYGARC_MAS1_IPROT | CYGARC_MAS1_TS)
    lwi   r12, (((CYGARC_IMM_BASE + CYGPLF_CCSBAR_BOOT_SIZE_BYTES) & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGARC_IMM_BASE_STARTUP & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    // Move CCSBAR from 0xfe000000 to 0xd0000000, the P4080 user manual specifies
    // how this should be done_
    // 1. Place a temporary LAW over the new planned CCSRBAR space
    // 2. Read the current CCSRBARH and CCSRBARL using load word instructions
    // 3. Follow this with an isync instruction
    // 4. Write the new values for CCSRBARH and CCSRBARL to their old locations
    // 5. Follow this with a sync instruction.
    // 6. Write a 1 to the commit bit (C) of CCSRAR at the old location
    // 7. Follow this with a sync instruction
    // 8. Disable temporary LAW

    #1
    lwi r30, CYGARC_IMM_BASE+CYGPLF_CCSBAR_BOOT_SIZE_BYTES
    li  r10, 0
    lwi r11, CYGARC_IMM_BASE

    stw r10, (CYGARC_REG_IMM_LBLAWBARH0)(r30)
    stw r11, (CYGARC_REG_IMM_LBLAWBARL0)(r30)

    lwi r12, CONFIG_SYS_CCSRBAR_LBLAWAR0
    stw r12, (CYGARC_REG_IMM_LBLAWAR0)(r30)
    isync

    /*
     * According to the P4080 User Manual, the last write (to LAWARx) should
     * be followed by a read of LAWARx before any devices try to use the window.
     * If the configuration is being performed by the core, the read of LAWARx
     * should be followed by an isync instruction.
     */
    lwz r12, (CYGARC_REG_IMM_LBLAWAR0)(r30)
    isync

    #2 and # 3
    lwz r2, CYGARC_REG_IMM_LCC_CCSRBARH(r30)
    lwz r2, CYGARC_REG_IMM_LCC_CCSRBARL(r30)
    isync

    #4 and #5
    stw r10, CYGARC_REG_IMM_LCC_CCSRBARH(r30)
    stw r11, CYGARC_REG_IMM_LCC_CCSRBARL(r30)
    sync

    #6 and #7
    lwi r12, CYGARC_REG_IMM_LCC_CCSRAR_C
    stw r12, CYGARC_REG_IMM_LCC_CCSRAR(r30)
    sync

    #8
    stw r10, (CYGARC_REG_IMM_LBLAWBARH0)(r11)
    stw r10, (CYGARC_REG_IMM_LBLAWBARL0)(r11)
    stw r10, (CYGARC_REG_IMM_LBLAWAR0)(r11)
    isync

    lwz r10, (CYGARC_REG_IMM_LBLAWAR0)(r11)
    isync

    // Set flash LAWBAR
hal_set_flash_law_asm:
    lwi r30, CYGARC_IMM_BASE

    lwi r10, CONFIG_SYS_LBLAWBARH0
    stw r10, (CYGARC_REG_IMM_LBLAWBARH0)(r30)

    lwi r10, CONFIG_SYS_LBLAWBARL0
    stw r10, (CYGARC_REG_IMM_LBLAWBARL0)(r30)

    lwi r10, CONFIG_SYS_LBLAWAR0
    stw r10, (CYGARC_REG_IMM_LBLAWAR0)(r30)
    isync

    lwz r10, (CYGARC_REG_IMM_LBLAWAR0)(r30)
    isync

    // Add entry for IMMR, flash and RAM
hal_MMU_init_basics_asm:

    // Create TLB 1:59
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_CSBAR_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_16M))
    lwi   r12, ((CYGARC_IMM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGARC_IMM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    // Create TLB 1:60
    // 256 MB is larger than real 128MB flash, maybe create 2 * 64 MB entries instead as TODO work
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_FLASH_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_256M))
    lwi   r12, ((CYGPLF_MEM_FLASH_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGPLF_MEM_FLASH_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    // Create TLB 1:61 (bottom half memory)
    // DDR shall be non-executable and guarded until we inflate the code from ROM to RAM
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_DDRAM_1_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_1G))
    lwi   r12, ((CYGPLF_MEM_RAM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_M | CYGARC_MAS2_G)
    lwi   r13, ((CYGPLF_MEM_RAM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
    // For secondary core, memory is executable and non guarded, code was inflated by primary CPU
#ifdef CYGPKG_HAL_SMP_SUPPORT
    lwi   r14, ((CYGPLF_MEM_RAM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_M)
    lwi   r15, ((CYGPLF_MEM_RAM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
#endif

    mtspr   MAS0,r10
    mtspr   MAS1,r11

#ifdef CYGPKG_HAL_SMP_SUPPORT
    // r20 and r21 are set when entering hal_hardware_init
    // Setting MAS2 and MAS3 depends if you are the primarly core or another core
    //
    cmpw  r20, r21
    bgt   1f
#endif

    mtspr   MAS2,r12
    mtspr   MAS3,r13
    b 2f

#ifdef CYGPKG_HAL_SMP_SUPPORT
1:
    mtspr   MAS2,r14
    mtspr   MAS3,r15
#endif

2:
    isync
    msync
    tlbwe

    // Create TLB 1:62 (top half memory)
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_DDRAM_2_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_1G))
    lwi   r12, ((CYGPLF_MEM_RAM_BASE_TOP_HALF & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_M | CYGARC_MAS2_G)
    lwi   r13, ((CYGPLF_MEM_RAM_BASE_TOP_HALF & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
    // For secondary core, memory is executable and non guarded, code was inflated by primary CPU
#ifdef CYGPKG_HAL_SMP_SUPPORT
    lwi   r14, ((CYGPLF_MEM_RAM_BASE_TOP_HALF & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_M)
    lwi   r15, ((CYGPLF_MEM_RAM_BASE_TOP_HALF & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)
#endif

    mtspr   MAS0,r10
    mtspr   MAS1,r11

#ifdef CYGPKG_HAL_SMP_SUPPORT
    // r20 and r21 are set when entering hal_hardware_init
    // Setting MAS2 and MAS3 depends if you are the primarly core or another core
    //
    cmpw  r20, r21
    bgt   1f
#endif

    mtspr   MAS2,r12
    mtspr   MAS3,r13
    b 2f

#ifdef CYGPKG_HAL_SMP_SUPPORT
1:
    mtspr   MAS2,r14
    mtspr   MAS3,r15
#endif

2:
    isync
    msync
    tlbwe

    // switch back to Address Space 0
    lis r3,(MSR_ME | MSR_DE)@h
    ori r3,r3,(MSR_ME | MSR_DE)@l
    mtmsr r3
    isync

    // Delete the boot entry from AS=1
hal_mmu_delete_boot_entries:

    lwi   r11, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGARC_TLB1_BOOT_PAGE))
    lwi   r12, 0
    lwi   r13, 0
    lwi   r14, 0

    mtspr   MAS0,r11
    mtspr   MAS1,r12
    mtspr   MAS2,r13
    mtspr   MAS3,r14
    isync
    msync
    tlbwe

#ifdef CYGPKG_HAL_SMP_SUPPORT
    // r20 and r21 are set when entering hal_hardware_init
    // Only primarly core continues that way, the other are done with the
    // initialisation phase
    cmpw  r20, r21
    bgt   _hardware_init_done_smp
#endif

    // Invalidate the CPC cache
    hal_cpu_invalidate_cpc_caches
    isync

    lwi r30, ( CYGARC_IMM_BASE + CYGARC_REG_IMM_ELBC )
    isync

    // Update flash OR0 / BR0 settings from default values
    lwi r3, CONFIG_SYS_FLASH_BR0
    stw r3, (CYGARC_REG_IMM_ELBC_BR0)(r30)
    isync

    lwi r3, CONFIG_SYS_FLASH_OR0_CLKDIV32
    stw r3, (CYGARC_REG_IMM_ELBC_OR0)(r30)
    isync

    //
    //  Init DDR1 controller
    //
    //
    lwi r30, ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DDRC1 )

    lwi r3, CONFIG_SYS_DDRC1_CS0_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS0_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS0_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS0_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS0_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS0_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS1_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS1_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS1_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS1_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS1_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS1_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS2_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS2_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS2_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS2_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS2_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS2_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS3_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS3_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC1_CS3_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS3_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS3_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS3_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_3
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_3)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_0
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_0)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_1
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_1)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_2
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_2)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_CFG_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG_2)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_1
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_2)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_3
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_3)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_4
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_4)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_5
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_5)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_6
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_6)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_7
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_7)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_8
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_8)(r30)

    lwi r3, CONFIG_SYS_DDR_MD_CNTL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_MD_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_INTERVAL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_INTERVAL)(r30)

    lwi r3, CONFIG_SYS_DDR_DATA_INIT
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_DATA_INIT)(r30)

    lwi r3, CONFIG_SYS_DDR_CLK_CTRL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CLK_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_4
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_4)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_5
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_5)(r30)

    lwi r3, CONFIG_SYS_DDR_ZQ_CNTL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_ZQ_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_WRLVL_CNTL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_WRLVL_CNTL_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL_2)(r30)

    lwi r3, CONFIG_SYS_DDR_WRLVL_CNTL_3
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL_3)(r30)

    lwi r3, (CONFIG_SYS_DDR_SDRAM_CFG & (~(CYGARC_REG_DDRC_SDRAM_CFG_MEM_EN)))
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG)(r30)

    lwi r3, CONFIG_SYS_DDR_RCW_1
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_RCW_1)(r30)

    lwi r3, CONFIG_SYS_DDR_RCW_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_RCW_2)(r30)

    isync

#ifdef CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_A003474
    lwi r3,  0x00000015
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_DEBUG(12))(r30)

    lwi r3,  0x24000000
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_DEBUG(21))(r30)
#endif

//
//  Init DDR2 controller
//  
//
    lwi r30, CYGARC_IMM_BASE + CYGARC_REG_IMM_DDRC2

    lwi r3, CONFIG_SYS_DDRC2_CS0_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS0_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS0_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS0_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS0_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS0_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS1_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS1_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS1_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS1_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS1_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS1_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS2_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS2_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS2_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS2_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS2_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS2_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS3_BNDS
    stw r3, CYGARC_REG_IMM_DDRC_CS3_BNDS(r30)

    lwi r3, CONFIG_SYS_DDRC2_CS3_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC_CS3_CONFIG)(r30)

    lwi r3, CONFIG_SYS_DDR_CS3_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC_CS3_CONFIG_2)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_3
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_3)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_0
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_0)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_1
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_1)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_2
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_2)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_CFG_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG_2)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_1
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_2)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_3
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_3)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_4
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_4)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_5
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_5)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_6
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_6)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_7
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_7)(r30)

    lwi r3, CONFIG_SYS_DDR_SDRAM_MODE_8
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_8)(r30)

    lwi r3, CONFIG_SYS_DDR_MD_CNTL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_MD_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_INTERVAL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_INTERVAL)(r30)

    lwi r3, CONFIG_SYS_DDR_DATA_INIT
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_DATA_INIT)(r30)

    lwi r3, CONFIG_SYS_DDR_CLK_CTRL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CLK_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_4
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_4)(r30)

    lwi r3, CONFIG_SYS_DDR_TIMING_5
    stw r3, (CYGARC_REG_IMM_DDRC_TIMING_CFG_5)(r30)

    lwi r3, CONFIG_SYS_DDR_ZQ_CNTL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_ZQ_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_WRLVL_CNTL
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL)(r30)

    lwi r3, CONFIG_SYS_DDR_WRLVL_CNTL_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL_2)(r30)

    lwi r3, CONFIG_SYS_DDR_WRLVL_CNTL_3
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL_3)(r30)

    lwi r3, (CONFIG_SYS_DDR_SDRAM_CFG & (~(CYGARC_REG_DDRC_SDRAM_CFG_MEM_EN)))
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG)(r30)

    lwi r3, CONFIG_SYS_DDR_RCW_1
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_RCW_1)(r30)

    lwi r3, CONFIG_SYS_DDR_RCW_2
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_RCW_2)(r30)

    isync

#ifdef CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_A003474
    lwi r3,  0x00000015
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_DEBUG(12))(r30)

    lwi r3,  0x24000000
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_DEBUG(21))(r30)
#endif

    // Wait 500 us or more before enable
    lwi     r3,0x1000
    mtctr   r3
1:  nop
    bdnz    1b

    // Enable controller 1
    lwi r30, CYGARC_IMM_BASE + CYGARC_REG_IMM_DDRC1
    lwi r3, (CONFIG_SYS_DDR_SDRAM_CFG | CYGARC_REG_DDRC_SDRAM_CFG_MEM_EN)
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG)(r30)
    isync

    // Enable controller 2
    lwi r30, CYGARC_IMM_BASE + CYGARC_REG_IMM_DDRC2
    lwi r3, (CONFIG_SYS_DDR_SDRAM_CFG | CYGARC_REG_DDRC_SDRAM_CFG_MEM_EN)
    stw r3, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG)(r30)
    isync

    // Wait for controller 1 initialisation completed
    lwi r30, (CYGARC_IMM_BASE + CYGARC_REG_IMM_DDRC1)
1:
    lwz   r10, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG_2)(r30)
    andi. r11, r10, CYGARC_REG_DDRC_SDRAM_CFG2_INIT
    beq-  3f
    // Init not completed, wait few micro seconds
    lwi   r3,0x100
    mtctr r3
2:  nop
    bdnz  2b
    b     1b
3:

    // Wait for controller 2 initialisation completed
    lwi r30, (CYGARC_IMM_BASE + CYGARC_REG_IMM_DDRC2)
4:
    lwz   r10, (CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG_2)(r30)
    andi. r11, r10, CYGARC_REG_DDRC_SDRAM_CFG2_INIT
    beq-  6f
    // Init not completed, wait few micro seconds
    lwi   r3,0x100
    mtctr r3
5:  nop
    bdnz  5b
    b     4b
6:

    // Set DDR3 memory LAWBAR
    lwi r30, CYGARC_IMM_BASE

    lwi r10, CONFIG_SYS_LBLAWBARH1
    stw r10, (CYGARC_REG_IMM_LBLAWBARH1)(r30)

    lwi r10, CONFIG_SYS_LBLAWBARL1
    stw r10, (CYGARC_REG_IMM_LBLAWBARL1)(r30)

    lwi r10, CONFIG_SYS_LBLAWAR1
    stw r10, (CYGARC_REG_IMM_LBLAWAR1)(r30)
    isync

    lwz r10, (CYGARC_REG_IMM_LBLAWAR1)(r30)
    isync

#endif // ROM/ROMRAM

#ifdef CYG_HAL_STARTUP_ROMRAM
    // Copy image from ROM to RAM
    mflr    r3
    lwi     r4,(CYGPLF_REDBOOT_START&0xFFF00000)
    lwi     r5,0x000FFFFF     // ROM/FLASH base
    and     r3,r3,r5          // segment relative
    lwi     r6,_hal_hardware_init_done
    mtlr    r6
    sub     r6,r3,r6          // Absolute address
    add     r6,r6,r4          // FLASH address
    lwi     r7,0x0            // where to copy to
    lwi     r8,__ram_data_end

10:   lwz     r5,0(r6)
    stw     r5,0(r7)
    addi    r6,r6,4
    addi    r7,r7,4
    cmplw   r7,r8
    bne     10b
#endif

#if  (defined(CYG_HAL_STARTUP_ROMRAM) || defined(CYG_HAL_STARTUP_ROM))
    // Finally set memory to executable and non-guarded
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_DDRAM_1_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_1G))
    lwi   r12, ((CYGPLF_MEM_RAM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_M)
    lwi   r13, ((CYGPLF_MEM_RAM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_DDRAM_2_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_1G))
    lwi   r12, ((CYGPLF_MEM_RAM_BASE_TOP_HALF & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_M)
    lwi   r13, ((CYGPLF_MEM_RAM_BASE_TOP_HALF & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe
#endif

hardware_init_done:
        blr

#ifdef CYGPKG_HAL_SMP_SUPPORT
_hardware_init_done_smp:
        lwi     r6,_hal_hardware_init_done
        mtlr    r6
        blr
#endif



