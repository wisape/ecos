//==========================================================================
//
//      mpc8572ds.s
//
//      HAL initialisation
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 2009, 2010 Free Software Foundation, Inc.
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
// Description:  Initialisation for MPC8572DS platform from Freescale
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

    // Create TLB 0:0 entry to access boot CCSRBAR space
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB0 | CYGARC_MAS0_ESEL(0))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_4K) | CYGARC_MAS1_TS)
    lwi   r12, ((CYGARC_IMM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGARC_IMM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    // Create TLB 0:1 entry to access boot CCSRBAR space
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB0 | CYGARC_MAS0_ESEL(1))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_4K) | CYGARC_MAS1_TS)
    lwi   r12, (((CYGARC_IMM_BASE + 0x1000) & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGARC_IMM_BASE_STARTUP & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe
      
    // Move CCSBAR from 0xff700000 to 0xd0000000, read value first, update and read outside
    sync
    lwi r11, (CYGARC_IMM_BASE + 0x1000)
    lwz r10, 0(r11)
    isync

    lwi r12 , (CYGARC_IMM_BASE  >> 12)
    stw r12, CYGARC_REG_IMM_CCSRBAR(r11)
    isync

    lwi r11, (CYGARC_IMM_BASE)
    lwz r10, 0(r11)
    isync
     
    // Set flash LAWBAR
hal_set_flash_law_asm:
    lwi r30, CYGARC_IMM_BASE
    isync

    lwi r10, CONFIG_SYS_LBLAWBAR3
    stw r10, (CYGARC_REG_IMM_LBLAWBAR3)(r30)
    isync

    lwi r10, CONFIG_SYS_LBLAWAR3
    stw r10, (CYGARC_REG_IMM_LBLAWAR3)(r30)
    isync


    // Add entry for IMMR, flash and RAM
hal_MMU_init_basics_asm:

    // Create TLB 1:12
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_CSBAR_TLB))
    lwi   r11, (CYGARC_MAS1_V | CYGARC_MAS1_TSIZE(CYGARC_TLB_PAGES_1M))
    lwi   r12, ((CYGARC_IMM_BASE & CYGARC_MAS2_EPN_MASK) | CYGARC_MAS2_I | CYGARC_MAS2_G)
    lwi   r13, ((CYGARC_IMM_BASE & CYGARC_MAS3_RPN_MASK) | CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR)

    mtspr   MAS0,r10
    mtspr   MAS1,r11
    mtspr   MAS2,r12
    mtspr   MAS3,r13
    isync
    msync
    tlbwe

    // Create TLB 1:13
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

    // Create TLB 1:14
    // DDR shall be non-executable and guarded until we inflate the code from ROM to RAM
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_DDRAM_TLB))
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
#endif


#if  (defined(CYG_HAL_STARTUP_ROMRAM) || defined(CYG_HAL_STARTUP_ROM))

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
    cmpw  r20, r21
    bgt   _hardware_init_done_smp
#endif

    lwi r30, CYGARC_IMM_BASE
    isync

    // Update flash OR0 / BR0 settings from default values
    lwi r3, CONFIG_SYS_FLASH_BR0
    stw r3, (CYGARC_REG_IMM_BR0)(r30)
    isync

    lwi r3, CONFIG_SYS_FLASH_OR0
    stw r3, (CYGARC_REG_IMM_OR0)(r30)
    isync

    hal_cpu_invalidate_L2_cache

    //
    //  Init DDR1 controller
    //
    //
#ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR1_CONTROLLER
    lwi r3, CONFIG_SYS_DDRC1_CS0_BNDS
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_CS0_BNDS)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CS0_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_CS0_CONFIG)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CS0_CONFIG2
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_CS0_CONFIG_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_0
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_TIMING_CFG_0)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_1
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_TIMING_CFG_1)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_2
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_TIMING_CFG_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_3
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_TIMING_CFG_3)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CFG
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_CFG)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CFG_2
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_CFG_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_MODE_1
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_MODE)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_MODE_2
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_MODE_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_INTERVAL
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_INTERVAL)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_DATA_INIT
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_DATA_INIT)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CLK_CTRL
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_CLK_CNTL)(r30)
    isync

#endif



//
//  Init DDR2 controller
//  
//
#ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR2_CONTROLLER
    lwi r3, CONFIG_SYS_DDRC2_CS0_BNDS
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_CS0_BNDS)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CS0_CONFIG
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_CS0_CONFIG)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_0
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_TIMING_CFG_0)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_1
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_TIMING_CFG_1)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_2
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_TIMING_CFG_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_TIMING_3
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_TIMING_CFG_3)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CFG
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_CFG)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CFG_2
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_CFG_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_MODE_1
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_MODE)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_MODE_2
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_MODE_2)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_INTERVAL
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_INTERVAL)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_DATA_INIT
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_DATA_INIT)(r30)
    isync

    lwi r3, CONFIG_SYS_DDR_CLK_CTRL
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_CLK_CNTL)(r30)
    isync
#endif


#if defined(CYGHWR_HAL_POWERPC_MPC8572DS_DDR1_CONTROLLER) || \
        defined(CYGHWR_HAL_POWERPC_MPC8572DS_DDR2_CONTROLLER)
    // Wait before enable
    lwi     r3,0x10000
    mtctr   r3
99:   nop
    bdnz    99b

#ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR1_CONTROLLER
    lwi r3, CONFIG_SYS_DDR_CFG_DDR_EN
    stw r3, (CYGARC_REG_IMM_DDRC1+CYGARC_REG_IMM_DDR_SDRAM_CFG)(r30)
    isync
#endif

#ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR2_CONTROLLER
    lwi r3, CONFIG_SYS_DDR_CFG_DDR_EN
    stw r3, (CYGARC_REG_IMM_DDRC2+CYGARC_REG_IMM_DDR_SDRAM_CFG)(r30)
    isync
#endif

    // Wait for data init
    lwi     r3,0x10000
    mtctr   r3
99:   nop
    bdnz    99b

#ifdef CYGHWR_HAL_POWERPC_MPC8572DS_DDR1_CONTROLLER
    lwi r3, CONFIG_SYS_LBLAWBAR1
    stw r3, (CYGARC_REG_IMM_LBLAWBAR1)(r30)
    isync

    lwi r3, CONFIG_SYS_LBLAWAR1
    stw r3, (CYGARC_REG_IMM_LBLAWAR1)(r30)
    isync
#endif

#if defined(CYGHWR_HAL_POWERPC_MPC8572DS_DDR2_CONTROLLER) && \
       !defined(CYGHWR_HAL_POWERPC_MPC8572DS_DDR_INTERLEAVED)
    lwi r3, CONFIG_SYS_LBLAWBAR2
    stw r3, (CYGARC_REG_IMM_LBLAWBAR2)(r30)
    isync

    lwi r3, CONFIG_SYS_LBLAWAR2
    stw r3, (CYGARC_REG_IMM_LBLAWAR2)(r30)
    isync
#endif

#endif

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
    lwi   r10, (CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(CYGPLF_MEM_DDRAM_TLB))
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
#endif

hardware_init_done:
        blr

#ifdef CYGPKG_HAL_SMP_SUPPORT
_hardware_init_done_smp:
        lwi     r6,_hal_hardware_init_done
        mtlr    r6
        blr
#endif



