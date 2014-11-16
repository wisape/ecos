##=============================================================================
##
##      context.S
##
##      Microblaze context switch code
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
## Author(s):    ccoutand
## Original(s):  nickg for PowerPC
## Date:         1998-04-27
## Purpose:      Microblaze context switch code
## Description:  This file contains implementations of the thread context
##               switch routines. It also contains the longjmp() and setjmp()
##               routines.
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/hal.h>

#include <cyg/hal/arch.inc>
#include <cyg/hal/mb_offsets.inc>


#------------------------------------------------------------------------------
# Configure to use either a minimal or maximal thread state
#ifdef CYGDBG_HAL_COMMON_CONTEXT_SAVE_MINIMUM

        .macro  save_special reg
        .endm

        .macro  load_special reg
        .endm

        .macro save_general reg
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

        .macro load_general reg
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

#else

        .macro  save_special reg
         // cTODO
        .endm

        .macro  load_special reg
         // cTODO
        .endm

        .macro save_general reg
           swi   CYGARC_REG(4),  \reg, (CYGARC_MBREG_REGS+4*4)
           swi   CYGARC_REG(7),  \reg, (CYGARC_MBREG_REGS+7*4)
           swi   CYGARC_REG(8),  \reg, (CYGARC_MBREG_REGS+8*4)
           swi   CYGARC_REG(9),  \reg, (CYGARC_MBREG_REGS+9*4)
           swi   CYGARC_REG(10), \reg, (CYGARC_MBREG_REGS+10*4)
           swi   CYGARC_REG(11), \reg, (CYGARC_MBREG_REGS+11*4)
           swi   CYGARC_REG(12), \reg, (CYGARC_MBREG_REGS+12*4)
           swi   CYGARC_REG(14), \reg, (CYGARC_MBREG_REGS+14*4)
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

        .macro load_general reg
           lwi   CYGARC_REG(4),  \reg, (CYGARC_MBREG_REGS+4*4)
           lwi   CYGARC_REG(7),  \reg, (CYGARC_MBREG_REGS+7*4)
           lwi   CYGARC_REG(8),  \reg, (CYGARC_MBREG_REGS+8*4)
           lwi   CYGARC_REG(9),  \reg, (CYGARC_MBREG_REGS+9*4)
           lwi   CYGARC_REG(10), \reg, (CYGARC_MBREG_REGS+10*4)
           lwi   CYGARC_REG(11), \reg, (CYGARC_MBREG_REGS+11*4)
           lwi   CYGARC_REG(12), \reg, (CYGARC_MBREG_REGS+12*4)
           lwi   CYGARC_REG(14), \reg, (CYGARC_MBREG_REGS+14*4)
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

#endif

#------------------------------------------------------------------------------
# hal_thread_switch_context
# Switch thread contexts
# R5 = address of sp of next thread to execute
# R6 = address of sp save location of current thread

FUNC_START(hal_thread_switch_context)
        addk  r3, sp, r0                                      # R3 = saved stack pointer
        addik sp, sp, -CYGARC_MB_CONTEXT_SIZE                 # space for state

        save_general sp

        swi   CYGARC_REG(3),  sp, (CYGARC_MBREG_REGS+1*4)     # R3 = real SP, save in R1 slot
        swi   CYGARC_REG(2),  sp, (CYGARC_MBREG_REGS+2*4)
        swi   CYGARC_REG(13), sp, (CYGARC_MBREG_REGS+13*4)

        swi   CYGARC_REG(15), sp, (CYGARC_MBREG_REGS+15*4)    # R15 = LR

#ifdef CYGDBG_HAL_DEBUG_GDB_THREAD_SUPPORT
        # Make the thread context look like an exception context if thread-
        # aware debugging is required. This state does not need restoring.
        swi  lr, sp, CYGARC_MBREG_PC                          # pc of caller
#endif

        mfs  r4, rmsr
        swi  r4, sp, CYGARC_MBREG_MSR                         # save msr

        save_special sp                                       # save special regs

        swi  sp, r6, 0                                        # save SP into save location

        # Now load the destination thread by dropping through
        # to hal_thread_load_context

#------------------------------------------------------------------------------
# hal_thread_load_context
# Load thread context
# R5 = address of sp of next thread to execute
# Note that this function is also the second half of hal_thread_switch_context
# and is simply dropped into from it.

FUNC_START(hal_thread_load_context)
        lwi  sp, r5, 0                                        # load state directly into SP

        load_special sp                                       # load special registers

        load_general sp

        lwi  r5, sp, CYGARC_MBREG_MSR                         # merge interrupt state
        hal_cpu_int_merge r5, r2

        lwi CYGARC_REG(2),  sp, (CYGARC_MBREG_REGS+2*4)
        lwi CYGARC_REG(13), sp, (CYGARC_MBREG_REGS+13*4)

        lwi CYGARC_REG(15), sp, (CYGARC_MBREG_REGS+15*4)      # load link register

        lwi  r5, sp, (CYGARC_MBREG_REGS+5*4)                  # load r5

        lwi  sp, sp, (CYGARC_MBREG_REGS+1*4)                  # finally restore true SP

        rtsd  lr, 8                                           # Jump to LR
        hal_nop
FUNC_END(hal_thread_load_context)


#------------------------------------------------------------------------------
# HAL longjmp, setjmp implementations
# hal_setjmp saves only to callee save registers 19-31, r1[sp],r2
# and lr into buffer supplied in r5[arg0]

FUNC_START(hal_setjmp)
        swi   CYGARC_REG(15), r5, CYGARC_JMPBUF_LR
        swi   CYGARC_REG(31), r5, CYGARC_JMPBUF_R31
        swi   CYGARC_REG(30), r5, CYGARC_JMPBUF_R30
        swi   CYGARC_REG(29), r5, CYGARC_JMPBUF_R29
        swi   CYGARC_REG(28), r5, CYGARC_JMPBUF_R28
        swi   CYGARC_REG(27), r5, CYGARC_JMPBUF_R27
        swi   CYGARC_REG(26), r5, CYGARC_JMPBUF_R26
        swi   CYGARC_REG(25), r5, CYGARC_JMPBUF_R25
        swi   CYGARC_REG(24), r5, CYGARC_JMPBUF_R24
        swi   CYGARC_REG(23), r5, CYGARC_JMPBUF_R23
        swi   CYGARC_REG(22), r5, CYGARC_JMPBUF_R22
        swi   CYGARC_REG(21), r5, CYGARC_JMPBUF_R21
        swi   CYGARC_REG(20), r5, CYGARC_JMPBUF_R20
        swi   CYGARC_REG(19), r5, CYGARC_JMPBUF_R19
        swi   CYGARC_REG(2),  r5, CYGARC_JMPBUF_R2    # TOC, optimize out?
        swi   CYGARC_REG(13), r5, CYGARC_JMPBUF_R13
        swi   CYGARC_REG(1),  r5, CYGARC_JMPBUF_SP
        addk  r3, r0, r0                              # Return 0
        rtsd  lr, 8                                   # Return to link register
        hal_nop
FUNC_END(hal_setjmp)


# hal_longjmp loads state from r5[arg0] and returns
# to the setjmp caller with r6[arg1] as return value

FUNC_START(hal_longjmp)
       lwi   CYGARC_REG(15), r5, CYGARC_JMPBUF_LR
       lwi   CYGARC_REG(31), r5, CYGARC_JMPBUF_R31
       lwi   CYGARC_REG(30), r5, CYGARC_JMPBUF_R30
       lwi   CYGARC_REG(29), r5, CYGARC_JMPBUF_R29
       lwi   CYGARC_REG(28), r5, CYGARC_JMPBUF_R28
       lwi   CYGARC_REG(27), r5, CYGARC_JMPBUF_R27
       lwi   CYGARC_REG(26), r5, CYGARC_JMPBUF_R26
       lwi   CYGARC_REG(25), r5, CYGARC_JMPBUF_R25
       lwi   CYGARC_REG(24), r5, CYGARC_JMPBUF_R24
       lwi   CYGARC_REG(23), r5, CYGARC_JMPBUF_R23
       lwi   CYGARC_REG(22), r5, CYGARC_JMPBUF_R22
       lwi   CYGARC_REG(21), r5, CYGARC_JMPBUF_R21
       lwi   CYGARC_REG(20), r5, CYGARC_JMPBUF_R20
       lwi   CYGARC_REG(19), r5, CYGARC_JMPBUF_R19
       lwi   CYGARC_REG(2),  r5, CYGARC_JMPBUF_R2
       lwi   CYGARC_REG(13), r5, CYGARC_JMPBUF_R13
       lwi   CYGARC_REG(1),  r5, CYGARC_JMPBUF_SP
       addk  r3, r6, r0                              # return r3[arg1]
       rtsd  lr, 8                                   # Return to link register
       hal_nop
FUNC_END(hal_longjmp)

#------------------------------------------------------------------------------
# end of context.S
