# ==========================================================================
#
#      variant.inc
#
#      Freescale Quicc III family assembler header file
#
##==========================================================================
## ####ECOSGPLCOPYRIGHTBEGIN####                                            
## -------------------------------------------                              
## This file is part of eCos, the Embedded Configurable Operating System.   
## Copyright (C) 2008 Free Software Foundation, Inc.                        
##
## eCos is free software; you can redistribute it and/or modify it under    
## the terms of the GNU General Public License as published by the Free     
## Software Foundation; either version 2 or (at your option) any later      
## version.                                                                 
##
## eCos is distributed in the hope that it will be useful, but WITHOUT      
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
## for more details.                                                        
##
## You should have received a copy of the GNU General Public License        
## along with eCos; if not, write to the Free Software Foundation, Inc.,    
## 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
##
## As a special exception, if other files instantiate templates or use      
## macros or inline functions from this file, or you compile this file      
## and link it with other works to produce a work based on this file,       
## this file does not by itself cause the resulting work to be covered by   
## the GNU General Public License. However the source code for this file    
## must still be made available in accordance with section (3) of the GNU   
## General Public License v2.                                               
##
## This exception does not invalidate any other reasons why a work based    
## on this file might be covered by the GNU General Public License.         
## -------------------------------------------                              
## ####ECOSGPLCOPYRIGHTEND####                                              
##==========================================================================
######DESCRIPTIONBEGIN####
#
# Author(s):    ccoutand
# Contributors: 
# Date:         2009-11-01
# Purpose:      
# Description:  Variant package for Freescale Quicc III architecture
#
#####DESCRIPTIONEND####
#
# ==========================================================================

#ifndef CYGONCE_HAL_VARIANT_INC
#define CYGONCE_HAL_VARIANT_INC

#include <pkgconf/hal.h>      

#include <cyg/hal/arch.inc>
#include <cyg/hal/var_regs.h>           // For CYGARC_IMM_BASE
#include <cyg/hal/plf_mem.h>

##-----------------------------------------------------------------------------
## Quicc III exception vector
##
      .macro hal_extra_vectors
      exception_vector        spe_unavailable
      exception_vector        spe_floating_point_data_exception
      exception_vector        spe_floating_point_round_exception
      exception_vector        performance_monitor
      .endm

        
      //define the reset words
      .macro hal_reserved_vector_00000
      b _start
      .endm

#define CYG_HAL_RESERVED_VECTOR_00000 hal_reserved_vector_00000
##-----------------------------------------------------------------------------   


##-----------------------------------------------------------------------------
## Quicc III CPU initialization
##        
      .macro hal_cpu_init 
#if defined(CYG_HAL_STARTUP_RAM)   
      ## Set up MSR
      lwi   r3,CYG_MSR
      mtmsr r3
      isync    

      ## Setup interrupt vectors
      lwi   r3,0x00000000
      mtspr IPVR,r3
      isync
#endif        

      ## Clear timer registers
      lwi   r3,0x00000000
      mtspr CYGARC_REG_TCR,r3
      mtspr CYGARC_REG_DEC,r3
      ## Write-one-to clear timer/watchdog/decrementer 
      ## status bits
      lis   r3,0xFC00
      mtspr CYGARC_REG_TSR,r3
      .endm

##-----------------------------------------------------------------------------
## Quicc III CPU initialization
## 
      .macro hal_cpu_core_init 
      ## Clear timer registers
      lwi   r3,0x00000000
      mtspr CYGARC_REG_TCR,r3
      mtspr CYGARC_REG_DEC,r3
      ## Write-one-to clear timer/watchdog/decrementer 
      ## status bits
      lis   r3,0xFC00
      mtspr CYGARC_REG_TSR,r3

      ## Clear Exceptions registers
      lwi   r3,0x00000000
      ## Exception syndrome register
      mtspr CYGARC_REG_ESR,  r3
      ## Machine check syndrome register
      mtspr CYGARC_REG_MCSR, r3
      ## Integer Exception register
      mtspr CYGARC_REG_XER,  r3
      ## Data exception address register
      mtspr CYGARC_REG_DEAR, r3
      ## Clear debug register
      lwi   r3,0xFFC80000
      mtspr CYGARC_REG_DBSR, r3
      isync

      ## Enable Machine check
      lis   r3,HID0_EMCP@h
      ori   r3,r3,HID0_TBEN@l
      mtspr HID0,r3
      isync

      ## Enable address broadcast
      lwi   r3,0x00000000
      li    r3,(HID1_ASTME | HID1_ABE)@l
      mtspr CYGARC_REG_HID1,r3
      isync

      ## Invalidate and enable branch prediction
      li    r3,0x201
      mtspr CYGARC_REG_BUCSR,r3
      isync
      .endm

##-----------------------------------------------------------------------------
## Quicc III CPU IVOR init
##    
      .macro hal_var_ivor_init
      ## Common BOOK_E IVOR registers are set in vector.S
      lwi     r4,0x0000FFF0
      hal_ivor_init   IVOR32,spe_unavailable
      hal_ivor_init   IVOR33,spe_floating_point_data_exception
      hal_ivor_init   IVOR34,spe_floating_point_round_exception
      hal_ivor_init   IVOR35,performance_monitor
      .endm       

##-----------------------------------------------------------------------------
## Quicc III CPU L1 cache
##
      .macro hal_cpu_invalidate_L1_cache
      # Invalidate L1 cache, wait operation completed
      msync
      isync
      li    r3,(L1CSR0_CFI | L1CSR0_CLFR)
      mtspr L1CSR0,r3
      isync
      msync
1:
      mfspr r4,L1CSR0
      and.  r5,r3,r4
      bne   1b

      li    r3,(L1CSR1_ICFI | L1CSR1_ICLFR)
      msync
      isync
      mtspr L1CSR1,r3
      isync
      msync
1:
      mfspr r4,L1CSR1
      and.  r5,r3,r4
      bne   1b
      .endm

##-----------------------------------------------------------------------------
## Quicc-III CPU L2 cache
##
      .macro hal_cpu_invalidate_L2_cache
      mbar
      isync

      # Disable L2 cache
      lwi r3, (CYGARC_IMM_BASE + CYGARC_REG_IMM_L2CACHE)
      li  r4, 0
      stw r4, (CYGARC_REG_L2CTL)(r3)
      mbar
      isync

      # Invalidate L2 cache
      lis r4, L2CTL_I@H
      stw r4, (CYGARC_REG_L2CTL)(r3)
      lwz r5, (CYGARC_REG_L2CTL)(r3)
      mbar
      isync

1:
      lwz   r5, (CYGARC_REG_L2CTL)(r3)
      and.  r6,r4,r5
      bne   1b
      .endm

##-----------------------------------------------------------------------------
## Quicc III monitor initialization

#ifndef CYGPKG_HAL_PPC_MON_DEFINED

#if     defined(CYG_HAL_STARTUP_ROM) ||                 \
        defined(CYG_HAL_STARTUP_ROMRAM) ||              \
        (       defined(CYG_HAL_STARTUP_RAM) &&         \
                !defined(CYGSEM_HAL_USE_ROM_MONITOR))

        .macro  hal_mon_init

#ifdef CYGSEM_HAL_POWERPC_COPY_VECTORS

        hal_var_ivor_init

        lwi     r3,0x10000
        mtctr   r3
101:     nop
        bdnz    101b



        # If we are starting up from ROM and want vectors in RAM
        # or we are starting in RAM and NOT using a ROM monitor,
        # copy exception handler code to 0.
        lwi     r3,rom_vectors          # r3 = rom start
        lwi     r4,0                    # r4 = ram start
        lwi     r5,rom_vectors_end      # r5 = rom end
        cmplw   r3,r5                   # skip if no vectors
        beq     2f



        subi    r3,r3,4
        subi    r4,r4,4
        subi    r5,r5,4
1:
        lwzu    r0,4(r3)                # get word from ROM
        stwu    r0,4(r4)                # store in RAM
        cmplw   r3,r5                   # compare
        blt     1b                      # loop if not yet done
2:

        # Next initialize the VSR table. This happens whether the
        # vectors were copied to RAM or not.

        # First fill with exception handlers
        lwi     r3,cyg_hal_default_exception_vsr
        lwi     r4,hal_vsr_table
        subi    r4,r4,4
        li      r5,CYGNUM_HAL_VSR_COUNT
1:      stwu    r3,4(r4)
        subi    r5,r5,1
        cmpwi   r5,0
        bne     1b

#endif

        # Then fill in the special vectors
        lwi     r3,cyg_hal_default_interrupt_vsr
        lwi     r4,hal_vsr_table
        stw     r3,CYGNUM_HAL_VECTOR_INTERRUPT*4(r4)
        stw     r3,CYGNUM_HAL_VECTOR_DECREMENTER*4(r4)
#ifdef CYGSEM_HAL_QUICC3_WATCHDOG_ENABLE
        lwi     r3,cyg_hal_default_critical_interrupt_vsr
        stw     r3,CYGNUM_HAL_VECTOR_WATCHDOG*4(r4)
#endif
        .endm

#elif defined(CYG_HAL_STARTUP_RAM) && defined(CYGSEM_HAL_USE_ROM_MONITOR)

        # Initialize the VSR table entries
        # We only take control of the interrupt vectors,
        # the rest are left to the ROM for now...

        .macro  hal_mon_init
        lwi     r3,cyg_hal_default_interrupt_vsr
        lwi     r4,hal_vsr_table
        stw     r3,CYGNUM_HAL_VECTOR_INTERRUPT*4(r4)
        stw     r3,CYGNUM_HAL_VECTOR_DECREMENTER*4(r4)
#ifdef CYGSEM_HAL_QUICC3_WATCHDOG_ENABLE
        lwi     r3,cyg_hal_default_critical_interrupt_vsr
        stw     r3,CYGNUM_HAL_VECTOR_WATCHDOG*4(r4)
#endif
        .endm


#else

        .macro  hal_mon_init
        .endm

#endif


#define CYGPKG_HAL_PPC_MON_DEFINED

#endif // CYGPKG_HAL_PPC_MON_DEFINED


##-----------------------------------------------------------------------------
## Quicc III exception state handling
        .macro hal_variant_save regs
        .endm

        .macro hal_variant_load regs
        .endm

##-----------------------------------------------------------------------------
## Indicate that the ISR tables are defined in variant.S

#define CYG_HAL_PPC_ISR_TABLES_DEFINED

##-----------------------------------------------------------------------------
## Quicc III interrupt handling.

#ifndef CYGPKG_HAL_POWERPC_INTC_DEFINED

## First level decoding of Quicc III PIC interrupt controller.
        # decode the interrupt
        .macro  hal_intc_decode dreg,state
        lwz     \dreg,CYGARC_PPCREG_VECTOR(\state)         # retrieve vector number,
        srwi.   \dreg, \dreg, (CYGHWR_HAL_POWERPC_VECTOR_ALIGNMENT)
        # Check interrupt type, external and watchdog are handle differently
        cmpwi   \dreg, CYGNUM_HAL_VECTOR_DECREMENTER
        beq     3f
#ifdef CYGSEM_HAL_QUICC3_WATCHDOG_ENABLE
        cmpwi   \dreg, CYGNUM_HAL_VECTOR_WATCHDOG
        beq     4f
#endif
#ifdef CYGPKG_HAL_SMP_SUPPORT
        # Get CPU id
        mfspr   \dreg, CYGARC_REG_PIR
        cmplwi  cr7, \dreg, 1
        bne     cr7, 1f
        lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_PER_CPU_OFFSET)>>16    # load register base
        lwz     \dreg,CYGARC_REG_IMM_IACK_CPU1(\dreg)      # if external, get IACK vector for CPU1
        bl      2f
#endif
1:
        lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_PER_CPU_OFFSET)>>16    # load register base
        lwz     \dreg,CYGARC_REG_IMM_IACK_CPU0(\dreg)      # if external, get IACK vector for CPU0
2:
        slwi    \dreg,\dreg,16                             # shift vector value by 16
        srwi    \dreg,\dreg,16                             # shift vector value by 16
        bl      5f
3:      lwi     \dreg,CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER # decrementer interrupt, load vector value (decrementer re-mapped to 104)
        bl      5f
4:      lwi     \dreg,CYGNUM_HAL_INTERRUPT_WATCHDOG_TIMER  # decrementer interrupt, load vector value (watchdog timer re-mapped to 105)
        bl      5f
5:      stw     \dreg,CYGARC_PPCREG_VECTOR(\state)         # update vector in state frame.
        slwi    \dreg,\dreg,2                              # convert vector number to index (for indexing hal_interrupt_handlers table) index = vector * 4 because hal_interrupt_handlers is an array of 32 bits type
        .endm                              
        
#define CYGPKG_HAL_POWERPC_INTC_DEFINED
#endif // CYGPKG_HAL_POWERPC_INTC_DEFINED
#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_VARIANT_INC
# end of variant.inc
