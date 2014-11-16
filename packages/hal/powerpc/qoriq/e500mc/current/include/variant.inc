# ==========================================================================
#
#      variant.inc
#
#      Freescale QorIQ family assembler header file
#
##==========================================================================
## ####ECOSGPLCOPYRIGHTBEGIN####                                            
## -------------------------------------------                              
## This file is part of eCos, the Embedded Configurable Operating System.   
## Copyright (C) 2012 Free Software Foundation, Inc.                        
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
# Date:         2012-07-20
# Purpose:      
# Description:  Variant package for Freescale QorIQ architecture
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
## QorIQ exception vector
##
      .macro hal_extra_vectors
      exception_vector        performance_monitor
      exception_vector        processor_doorbell
      exception_vector        processor_doorbell_critical
      exception_vector        guest_processor_doorbell
      exception_vector        guest_processor_doorbell_critical
      exception_vector        hypervisor_sys_call
      exception_vector        hypervisor_privilege
      .endm

      //define the reset words
      .macro hal_reserved_vector_00000
      b _start
      .endm

#define CYG_HAL_RESERVED_VECTOR_00000 hal_reserved_vector_00000
##-----------------------------------------------------------------------------


##-----------------------------------------------------------------------------
## QorIQ CPU initialization
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
## QorIQ CPU initialization
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
      mfspr r3, CYGARC_REG_DBSR
      mtspr CYGARC_REG_DBSR, r3
      isync

      ## Enable Machine check.
      ## Loads and stores to storage that are marked as cache inhibited and
      ## guarded are ordered.
      msync
      isync
      lis   r3,HID0_EMCP@h
      ori   r3,r3,HID0_CIGLSO@l
      mtspr HID0,r3
      isync

      ## Invalidate and enable branch prediction
      li    r3,0x201
      mtspr CYGARC_REG_BUCSR,r3
      isync

#ifdef CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_A003999
      ## Running Floating Point instructions requires special initialization
      mfspr   r3,977
      oris    r3,r3,0x0100
      mtspr   977,r3
#endif
      .endm

##-----------------------------------------------------------------------------
## Reconfiguring the CoreNet Coherency Fabric for GEN errata A-004510
## and A-004511, Rev. E.
## Only the SPR write is implemented in SW. Instead of implementing the
## rest of the workaround in SW, The DCSR and CCSR writes are embedded
## in the PBL image.

#ifdef CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_A004510_A004511
      .macro hal_cpu_erratum_A004510_A004511
      msync
      isync
      mfspr  r3,976
      li     r4,0x48
      rlwimi r3,r4,0,0x1f8
      mtspr  976,r3
      isync
      .endm
#endif

##-----------------------------------------------------------------------------
## QorIQ CPU IVOR init
##    
      .macro hal_var_ivor_init
      ## Common BOOK_E IVOR registers are set in vector.S
      lwi     r4,0x0000FFF0
      hal_ivor_init   IVOR35, performance_monitor
      hal_ivor_init   IVOR36, processor_doorbell
      hal_ivor_init   IVOR37, processor_doorbell_critical
      hal_ivor_init   IVOR38, guest_processor_doorbell
      hal_ivor_init   IVOR39, guest_processor_doorbell_critical
      hal_ivor_init   IVOR40, hypervisor_sys_call
      hal_ivor_init   IVOR41, hypervisor_privilege
      .endm

##-----------------------------------------------------------------------------
## QorIQ CPU L1 / L2 cache
##
      .macro hal_cpu_invalidate_caches
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

      # Invalidate L2 cache, wait operation completed
      lis r3,L2CSR0_L2FI@H
      ori r3,r3,L2CSR0_L2LFC
      mtspr L2CSR0,r3
      isync
      msync

1:
      mfspr r4,L2CSR0
      and.  r5,r3,r4
      bne   1b
      .endm

##-----------------------------------------------------------------------------
## QorIQ CPU L3 cache
##
      .macro hal_cpu_invalidate_cpc_caches
      # Configure L3 as cache (CPC1)
      mbar
      isync

      # Disable L3 cache (CPC1)
      lwi r3, (CYGARC_IMM_BASE + CYGARC_REG_IMM_CPC1)
      li  r4, 0
      stw r4, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      lwz r5, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      mbar
      isync

      # Configure L3 as cache (CPC1) 
      lwi r4, 0
      stw r4, (CYGARC_REG_IMM_CPC_CPCSRCR0)(r3)
      lwz r5, (CYGARC_REG_IMM_CPC_CPCSRCR0)(r3)
      mbar
      isync

      # Invalidate L3 cache (CPC1)
      lis r4, CPCCSR0_CPCFI@H
      ori r4, r4, CPCCSR0_CPCLFC@L
      stw r4, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      lwz r5, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      mbar
      isync

1:
      lwz   r5, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      and.  r6,r4,r5
      bne   1b

      # Do the same with other L3 caches
#ifdef CYGARC_REG_IMM_CPC2
      # Configure L3 as cache (CPC2)
      mbar
      isync

      # Disable L3 cache (CPC2)
      lwi r3, (CYGARC_IMM_BASE + CYGARC_REG_IMM_CPC2)
      li  r4, 0
      stw r4, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      lwz r5, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      mbar
      isync

      # Configure L3 as cache (CPC2) 
      lwi r4, 0
      stw r4, (CYGARC_REG_IMM_CPC_CPCSRCR0)(r3)
      lwz r5, (CYGARC_REG_IMM_CPC_CPCSRCR0)(r3)
      mbar
      isync

      # Invalidate L3 cache (CPC2)
      lis r4, CPCCSR0_CPCFI@H
      ori r4, r4, CPCCSR0_CPCLFC@L
      stw r4, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      lwz r5, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      mbar
      isync

1:
      lwz   r5, (CYGARC_REG_IMM_CPC_CPCCSR0)(r3)
      and.  r6,r4,r5
      bne   1b
#endif
      .endm

##-----------------------------------------------------------------------------
## QorIQ monitor initialization

#ifndef CYGPKG_HAL_PPC_MON_DEFINED

#if     defined(CYG_HAL_STARTUP_ROM) ||                 \
        defined(CYG_HAL_STARTUP_ROMRAM) ||              \
        (       defined(CYG_HAL_STARTUP_RAM) &&         \
                !defined(CYGSEM_HAL_USE_ROM_MONITOR))

        .macro  hal_mon_init

#ifdef CYGSEM_HAL_POWERPC_COPY_VECTORS

        hal_var_ivor_init

        isync

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
#ifdef CYGSEM_HAL_QORIQ_WATCHDOG_ENABLE
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
//#ifdef CYGSEM_HAL_QORIQ_WATCHDOG_ENABLE
//        lwi     r3,cyg_hal_default_critical_interrupt_vsr
//        stw     r3,CYGNUM_HAL_VECTOR_WATCHDOG*4(r4)
//#endif
        .endm


#else

        .macro  hal_mon_init
        .endm

#endif


#define CYGPKG_HAL_PPC_MON_DEFINED

#endif // CYGPKG_HAL_PPC_MON_DEFINED


##-----------------------------------------------------------------------------
## QorIQ exception state handling
        .macro hal_variant_save regs
        .endm

        .macro hal_variant_load regs
        .endm

##-----------------------------------------------------------------------------
## Indicate that the ISR tables are defined in variant.S

#define CYG_HAL_PPC_ISR_TABLES_DEFINED

##-----------------------------------------------------------------------------
## QorIQ interrupt handling.

#ifndef CYGPKG_HAL_POWERPC_INTC_DEFINED

## First level decoding of QoriIQ PIC interrupt controller.
        # decode the interrupt
        .macro  hal_intc_decode dreg,state
          lwz     \dreg, CYGARC_PPCREG_VECTOR(\state)         # retrieve vector number,
          srwi.   \dreg, \dreg, (CYGHWR_HAL_POWERPC_VECTOR_ALIGNMENT)
          # Check interrupt type, external and watchdog are handle differently
          cmpwi   \dreg, CYGNUM_HAL_VECTOR_DECREMENTER
          beq     10f
#ifdef CYGPKG_HAL_SMP_SUPPORT
          # Get CPU id
          mfspr   \dreg, CYGARC_REG_PIR
          cmplwi  cr7, \dreg, 1
          bne     cr7, 1f
          cmplwi  cr7, \dreg, 2
          bne     cr7, 2f
# if CYGPKG_HAL_SMP_CPU_MAX > 2
          cmplwi  cr7, \dreg, 3
          bne     cr7, 3f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 3
          cmplwi  cr7, \dreg, 4
          bne     cr7, 4f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 4
          cmplwi  cr7, \dreg, 5
          bne     cr7, 5f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 5
          cmplwi  cr7, \dreg, 6
          bne     cr7, 6f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 6
          cmplwi  cr7, \dreg, 7
          bne     cr7, 7f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 7
          cmplwi  cr7, \dreg, 8
          bne     cr7, 8f
# endif
2:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU1(\dreg)      # if external, get IACK vector for CPU1
          bl      9f
# if CYGPKG_HAL_SMP_CPU_MAX > 2
3:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU2(\dreg)      # if external, get IACK vector for CPU2
          bl      9f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 3
4:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU3(\dreg)      # if external, get IACK vector for CPU3
          bl      9f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 4
5:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU4(\dreg)      # if external, get IACK vector for CPU4
          bl      9f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 5
6:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU5(\dreg)      # if external, get IACK vector for CPU5
          bl      9f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 6
7:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU6(\dreg)      # if external, get IACK vector for CPU6
          bl      9f
# endif
# if CYGPKG_HAL_SMP_CPU_MAX > 7
8:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU7(\dreg)      # if external, get IACK vector for CPU7
          bl      9f
# endif
#endif
1:
          lis     \dreg,(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC_MP_CPU_OFFSET+CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET)>>16    # load register base
          lwz     \dreg,CYGARC_REG_IMM_PIC_IACK_CPU0(\dreg)      # if external, get IACK vector for CPU0
9:
          slwi    \dreg,\dreg,16                             # shift vector value by 16
          srwi    \dreg,\dreg,16                             # shift vector value by 16
          bl      11f
10:       lwi     \dreg,CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER # decrementer interrupt, load vector value (decrementer re-mapped to 104)
          bl      11f
11:       stw     \dreg,CYGARC_PPCREG_VECTOR(\state)         # update vector in state frame.
          slwi    \dreg,\dreg,2                              # convert vector number to index (for indexing hal_interrupt_handlers table) index = vector * 4 because hal_interrupt_handlers is an array of 32 bits type
        .endm

#define CYGPKG_HAL_POWERPC_INTC_DEFINED
#endif // CYGPKG_HAL_POWERPC_INTC_DEFINED
#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_VARIANT_INC
# end of variant.inc
