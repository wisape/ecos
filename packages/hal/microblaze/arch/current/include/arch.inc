#ifndef _CYGONCE_HAL_MICROBLAZE_ARCH_INC_
#define _CYGONCE_HAL_MICROBLAZE_ARCH_INC_
##=============================================================================
##
##    arch.inc
##
##    Microblaze architecture assembler header file
##
##=============================================================================
## ####ECOSGPLCOPYRIGHTBEGIN####                                            
## -------------------------------------------                              
## This file is part of eCos, the Embedded Configurable Operating System.   
## Copyright (C) 2011 Free Software Foundation, Inc.
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
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s):   ccoutand
## Original:    nickg for PowerPC architecture
## Date:        2011-02-07
## Purpose:     Microblaze definitions.
## Description: This file contains various definitions and macros that are
##              useful for writing assembly code for the Microblaze
##              It also includes the variant assembly header file.
## Usage:
##      #include <cyg/hal/arch.inc>
##      ...
##
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <cyg/hal/variant.inc>
#include <cyg/hal/mb_offsets.inc>
#include <cyg/hal/hal_io.h>


##-----------------------------------------------------------------------------
## NOP
##

    .macro hal_nop
        or   r0, r0, r0
    .endm

#------------------------------------------------------------------------------
# Some useful coding macros

#define FUNC_START(name)        \
    .type name, @function;      \
    .globl name;                \
name:

#define FUNC_END(name)


##-----------------------------------------------------------------------------
## Push CPU registers on stack
##

#define sp                       r1
#define lr                       r15

#define CYGARC_REG(_v)           r##_v
#define CYGARC_REG_SP            CYGARC_REG(1)
#define CYGARC_REG_LR            CYGARC_REG(15)

     # cTODO : ASM not supporting .set ?

    .macro hal_regs_save reg
       lwi   CYGARC_REG(2),  \reg, (CYGARC_MBREG_REGS+2*4)
       # R3 and R4 saved separately
       swi   CYGARC_REG(5),  \reg, (CYGARC_MBREG_REGS+5*4)
       swi   CYGARC_REG(6),  \reg, (CYGARC_MBREG_REGS+6*4)
       swi   CYGARC_REG(7),  \reg, (CYGARC_MBREG_REGS+7*4)
       swi   CYGARC_REG(8),  \reg, (CYGARC_MBREG_REGS+8*4)
       swi   CYGARC_REG(9),  \reg, (CYGARC_MBREG_REGS+9*4)
       swi   CYGARC_REG(10), \reg, (CYGARC_MBREG_REGS+10*4)
       swi   CYGARC_REG(11), \reg, (CYGARC_MBREG_REGS+11*4)
       swi   CYGARC_REG(12), \reg, (CYGARC_MBREG_REGS+12*4)
       swi   CYGARC_REG(13), \reg, (CYGARC_MBREG_REGS+13*4)
       swi   CYGARC_REG(14), \reg, (CYGARC_MBREG_REGS+14*4)
       swi   CYGARC_REG(15), \reg, (CYGARC_MBREG_REGS+15*4)
       swi   CYGARC_REG(16), \reg, (CYGARC_MBREG_REGS+16*4)
       swi   CYGARC_REG(17), \reg, (CYGARC_MBREG_REGS+17*4)
       swi   CYGARC_REG(18), \reg, (CYGARC_MBREG_REGS+18*4)
       swi   CYGARC_REG(19), \reg, (CYGARC_MBREG_REGS+19*4)
       swi   CYGARC_REG(20), \reg, (CYGARC_MBREG_REGS+20*4)
       swi   CYGARC_REG(21), \reg, (CYGARC_MBREG_REGS+21*4)
       swi   CYGARC_REG(22), \reg, (CYGARC_MBREG_REGS+22*4)
       swi   CYGARC_REG(23), \reg, (CYGARC_MBREG_REGS+23*4)
       swi   CYGARC_REG(24), \reg, (CYGARC_MBREG_REGS+24*4)
       swi   CYGARC_REG(25), \reg, (CYGARC_MBREG_REGS+25*4)
       swi   CYGARC_REG(26), \reg, (CYGARC_MBREG_REGS+26*4)
       swi   CYGARC_REG(27), \reg, (CYGARC_MBREG_REGS+27*4)
       swi   CYGARC_REG(28), \reg, (CYGARC_MBREG_REGS+28*4)
       swi   CYGARC_REG(29), \reg, (CYGARC_MBREG_REGS+29*4)
       swi   CYGARC_REG(30), \reg, (CYGARC_MBREG_REGS+30*4)
       swi   CYGARC_REG(31), \reg, (CYGARC_MBREG_REGS+31*4)
    .endm

    .macro hal_regs_load reg
       lwi   CYGARC_REG(2),  \reg, (CYGARC_MBREG_REGS+2*4)
       # R3 restored outside
       lwi   CYGARC_REG(4),  \reg, (CYGARC_MBREG_REGS+4*4)
       lwi   CYGARC_REG(5),  \reg, (CYGARC_MBREG_REGS+5*4)
       lwi   CYGARC_REG(6),  \reg, (CYGARC_MBREG_REGS+6*4)
       lwi   CYGARC_REG(7),  \reg, (CYGARC_MBREG_REGS+7*4)
       lwi   CYGARC_REG(8),  \reg, (CYGARC_MBREG_REGS+8*4)
       lwi   CYGARC_REG(9),  \reg, (CYGARC_MBREG_REGS+9*4)
       lwi   CYGARC_REG(10), \reg, (CYGARC_MBREG_REGS+10*4)
       lwi   CYGARC_REG(11), \reg, (CYGARC_MBREG_REGS+11*4)
       lwi   CYGARC_REG(12), \reg, (CYGARC_MBREG_REGS+12*4)
       lwi   CYGARC_REG(13), \reg, (CYGARC_MBREG_REGS+13*4)
       lwi   CYGARC_REG(15), \reg, (CYGARC_MBREG_REGS+15*4)
       lwi   CYGARC_REG(16), \reg, (CYGARC_MBREG_REGS+16*4)
       lwi   CYGARC_REG(17), \reg, (CYGARC_MBREG_REGS+17*4)
       lwi   CYGARC_REG(18), \reg, (CYGARC_MBREG_REGS+18*4)
       lwi   CYGARC_REG(19), \reg, (CYGARC_MBREG_REGS+19*4)
       lwi   CYGARC_REG(20), \reg, (CYGARC_MBREG_REGS+20*4)
       lwi   CYGARC_REG(21), \reg, (CYGARC_MBREG_REGS+21*4)
       lwi   CYGARC_REG(22), \reg, (CYGARC_MBREG_REGS+22*4)
       lwi   CYGARC_REG(23), \reg, (CYGARC_MBREG_REGS+23*4)
       lwi   CYGARC_REG(24), \reg, (CYGARC_MBREG_REGS+24*4)
       lwi   CYGARC_REG(25), \reg, (CYGARC_MBREG_REGS+25*4)
       lwi   CYGARC_REG(26), \reg, (CYGARC_MBREG_REGS+26*4)
       lwi   CYGARC_REG(27), \reg, (CYGARC_MBREG_REGS+27*4)
       lwi   CYGARC_REG(28), \reg, (CYGARC_MBREG_REGS+28*4)
       lwi   CYGARC_REG(29), \reg, (CYGARC_MBREG_REGS+29*4)
       lwi   CYGARC_REG(30), \reg, (CYGARC_MBREG_REGS+30*4)
       lwi   CYGARC_REG(31), \reg, (CYGARC_MBREG_REGS+31*4)
    .endm

##-----------------------------------------------------------------------------
## CPU specific macros. These provide a common assembler interface to
## operations that may have CPU specific implementations on different
## variants of the architecture. msrset rd, imm

     # Enable interrupts
     .macro hal_cpu_int_enable
        msrset   r3, CYGARC_REG_MSR_IE
    .endm

     # Disable interrupts
    .macro hal_cpu_int_disable
        msrclr   r3, CYGARC_REG_MSR_IE
    .endm

     # Merge the interrupt enable state of the status register in
     # \sr with the current sr.
    .macro  hal_cpu_int_merge sr, wr
        mfs    r11, rmsr
        addik  \wr, r0,  CYGARC_REG_MSR_IE
        and    \wr, \wr, \sr
        or     r11, r11, \wr
        mts    rmsr, r11
    .endm

#------------------------------------------------------------------------------
# SMP stack support
## 

#ifdef CYGPKG_HAL_SMP_SUPPORT

    .macro hal_smp_init
    .endm

    .macro hal_smp_cpu id
    .endm

    .macro  hal_init_istack reg
    .endm

    .macro  hal_load_istack reg
    .endm

    .macro  hal_load_istack_base reg
    .endm

#else // CYGPKG_HAL_SMP_SUPPORT

    # Load interrupt stack
    .macro hal_init_istack reg
       addi  \reg, r0, hal_startup_stack
    .endm

    # Load interrupt stack
    .macro hal_load_istack reg
       addi  \reg, r0, hal_startup_stack
    .endm

    # Load interrupt stack base
    .macro  hal_load_istack_base reg
       addi  \reg, r0, (hal_startup_stack-CYGNUM_HAL_MICROBLAZE_COMMON_INTERRUPTS_STACK_SIZE)
    .endm

    # SMP init
    .macro  hal_smp_init
    .endm

    # Get CPU id
    .macro    hal_smp_cpu reg
       addi  \reg, r0, 0
    .endm

#endif // CYGPKG_HAL_SMP_SUPPORT


#------------------------------------------------------------------------------
# Handle vector
#

    .macro  exception_vsr_lookup name
       .globl vsr_lookup\name
       .align 8
vsr_lookup\name:
        addik   sp, sp, -CYGARC_MB_EXCEPTION_DECREMENT
        swi     r3, sp, (CYGARC_MBREG_REGS+3*4)  # Stash R3
        swi     r4, sp, (CYGARC_MBREG_REGS+4*4)  # Stash R4
        addik   r3, r0, vsr_lookup\name
        srl     r3, r3                           # Shift by 8 bits (should be link to the alignement)
        srl     r3, r3
        srl     r3, r3
        srl     r3, r3
        srl     r3, r3
        srl     r3, r3
        addik   r4, r3, hal_vsr_table            # Make a stop at 6 bits shift to address VSR table
        srl     r3, r3
        srl     r3, r3
        lwi     r4, r4, 0
        bra     r4
        nop
    .endm

    .macro  exception_vector name
        .globl exception_vector\name
exception_vector\name:
        bri vsr_lookup\name
        hal_nop
    .endm

    .macro  exception_reset name
        .globl exception_reset\name
exception_reset\name:
#ifdef CYG_HAL_STARTUP_ROMRAM
       brai ((_start-CYGMEM_REGION_ram)+CYGMEM_REGION_rom)
#else
       brai _start
#endif
    .endm


#------------------------------------------------------------------------------
# end of arch.inc

#endif // _CYGONCE_HAL_MICROBLAZE_ARCH_INC_
