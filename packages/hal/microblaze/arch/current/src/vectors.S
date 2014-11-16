/*==========================================================================
//
//      vectors.S
//
//      Microblaze exception vectors
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
// Original(s):  nickg for Cortex-M and PowerPC
// Date:         2011-01-18
// Description:  This file defines the code placed into the exception
//               vectors. It also contains the first level default VSRs
//               that save and restore state for both exceptions and
//               interrupts.
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/hal/hal_io.h>

#ifdef CYGPKG_KERNEL
# include <pkgconf/kernel.h>
#endif

#include <cyg/hal/arch.inc>
#include <cyg/hal/mb_offsets.inc>

        .extern hal_vsr_table

#ifdef CYGHWR_HAL_MICROBLAZE_NEED_VECTORS

//==========================================================================
// Initial exception vector table
//
        .section       ".vectors","ax"

rom_vectors:
        // Exception vectors can only hold 2 instructions, therefore, it is not
        // possible to jump to VSR routine from here. Instead, the processor
        // jumps to the intermediate VSR lookup routines
        exception_reset        _reset
        exception_vector       _user_exception
        exception_vector       _interrupt
        exception_vector       _break
        exception_vector       _exception
#ifndef CYGHWR_HAL_MICROBLAZE_MINIMUM_VECTORS
        exception_vector       _reserved_05
        exception_vector       _reserved_06
        exception_vector       _reserved_07
        exception_vector       _reserved_08
        exception_vector       _reserved_09
#endif

        // Keep VSR lookup routine alignement as defined (.align 8)
        // VSR lookup routines take care of moving the stack pointer, stash
        // R3 and R4. When leaving the routine, R3 contains the vector
        // number. The VSR handler can continue stashing the registers
        exception_vsr_lookup   _user_exception  // @ 0x100
        exception_vsr_lookup   _interrupt       // @ 0x200
        exception_vsr_lookup   _break           // @ 0x300
        exception_vsr_lookup   _exception       // @ 0x400
#ifndef CYGHWR_HAL_MICROBLAZE_MINIMUM_VECTORS
        exception_vsr_lookup   _reserved_05     // @ 0x500
        exception_vsr_lookup   _reserved_06     // @ 0x600
        exception_vsr_lookup   _reserved_07     // @ 0x700
        exception_vsr_lookup   _reserved_08     // @ 0x800
        exception_vsr_lookup   _reserved_09     // @ 0x900
#endif

rom_vectors_end:

#endif // CYGHWR_HAL_MICROBLAZE_NEED_VECTORS


//===========================================================================
// Real startup code. We jump here from the various reset vectors to set up
// the world.
//
        .text
        .align 2
FUNC_START(_start)
        # Start with CPU specific initialisation
        hal_cpu_init

        # Set up small data anchor
        addik   r2, r0, __SDATA2_START__
        addik   r13, r0, __SDATA_START__

        # Allow exceptions when supported by the hardware
#ifdef CYGHWR_HAL_MICROBLAZE_SUPPORT_EXCEPTION
        msrset r11, CYGARC_REG_MSR_EE
#endif

        # Disable caches at startup. If starting from RAM, keep the
        # cache as setup by the ROM or ROMRAM boot code.
#if !(defined(CYG_HAL_STARTUP_RAM))
        mfs     r11, rmsr
        andi    r11, r11, ~(CYGARC_REG_MSR_DCE | CYGARC_REG_MSR_ICE);
        mts     rmsr, r11
#endif

        hal_plf_startup_init

        hal_load_istack sp

        addik   r11, r0, hal_reset_vsr
        brald   lr,  r11

        hal_nop
FUNC_END(_start)


//==========================================================================
// Default exception VSR
//
        .align 2
FUNC_START(hal_default_exception_vsr)
#ifdef CYGHWR_HAL_MICROBLAZE_SUPPORT_EXCEPTION
        # We come here with all register containing their
        # pre-exception values except:
        # R3    = Vector Number
        # R3 and R4 are stashed on the exception stack

        # Save vector
        swi     r3, sp, CYGARC_MBREG_VECTOR

        # Save r5 to r31
        hal_regs_save sp

        # Save MSR and PC
        mfs     r3, rmsr
        swi     r3, sp, CYGARC_MBREG_MSR
        mfs     r3, rpc
        swi     r3, sp, CYGARC_MBREG_PC

        addik   r3, sp, CYGARC_MB_EXCEPTION_DECREMENT
        swi     r3, sp, CYGARC_MBREG_REGS+1*4

        # Save variant registers
        hal_variant_save sp

        # The entire CPU state is now stashed on the stack,
        # increment the scheduler lock and call the ISR
        # for this vector.

        # Pass SP as argument to restore_state
        addik   r5, sp, 0

        addik   sp, sp, -CYGARC_MB_STACK_FRAME_SIZE   # create new stack frame
        swi     r0, sp, 0                             # LR = 0

        addik   lr, r0, restore_state                 # get return link
        addik   lr, lr, -8                            # step back 2 instructions

        .extern hal_deliver_exception
        braid hal_deliver_exception
        hal_nop

        # Continue to restore state
#else
        # Loop forever, should never end up here
        addik   r11, r0, hal_default_exception_vsr
        bra     r11
#endif
FUNC_END(hal_default_exception_vsr)

//==========================================================================
// Default interrupt VSR
//
        .align 2
FUNC_START(hal_default_interrupt_vsr)
        # We come here with all register containing their
        # pre-exception values except:
        # R3    = Vector Number
        # R3 and R4 are stashed on the exception stack

        # Save vector
        swi     r3, sp, CYGARC_MBREG_VECTOR

        # Save r5 to r31
        hal_regs_save sp

        # Save MSR, PC and return address
        mfs     r3, rmsr
        swi     r3, sp, CYGARC_MBREG_MSR
        mfs     r3, rpc
        swi     r3, sp, CYGARC_MBREG_PC
        swi    r14, sp, CYGARC_MBREG_LR

        addik   r3, sp, CYGARC_MB_EXCEPTION_DECREMENT
        swi     r3, sp, CYGARC_MBREG_REGS+1*4         # and save it in state

        # Save variant registers
        hal_variant_save sp

        # The entire CPU state is now stashed on the stack,
        # increment the scheduler lock and call the ISR
        # for this vector.

#ifdef CYGFUN_HAL_COMMON_KERNEL_SUPPORT
        .extern cyg_scheduler_sched_lock
        lwi     r3, r0, cyg_scheduler_sched_lock
        addik   r3, r3, 1
        swi     r3, r0, cyg_scheduler_sched_lock
#endif

        # Move sp to r19
        addik   r19, sp, 0

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK
        hal_load_istack      r3                       # stack top
        hal_load_istack_base r4                       # stack base
        rsub    r5, r4, sp                            # sp - base
        blti    r5, 1f                                # if < 0 - not on istack
        rsub    r5, r1, r3                            # top - sp
        bgti    r5, 2f                                # if > 0 - already on istack

1:      hal_load_istack sp                            # switch to istack

2:      swi     r19, sp, -4                           # save old SP on stack
        addik   sp,  sp, -4
#endif

        addik   sp, sp, -CYGARC_MB_STACK_FRAME_SIZE   # create new stack frame
        swi     r0, sp, 0                             # LR = 0

#if defined(CYGPKG_KERNEL_INSTRUMENT) && defined(CYGDBG_KERNEL_INSTRUMENT_INTR)

        addik   r5, r5,  0x0301                       # r3 = type = INTR,RAISE
        lwi     r6, r19, CYGARC_MBREG_VECTOR          # arg1 = vector address
        addik   r7, r0,  0                            # arg2 = 0
        addik   r8, r0, cyg_instrument
        brald   lr, r8                                # call instrument function
        hal_nop
#endif

        # Don't touch r19 accross macro
        hal_intc_decode r20,r19                       # get table index

#if defined(CYGDBG_HAL_DEBUG_GDB_CTRLC_SUPPORT) \
    || defined(CYGDBG_HAL_DEBUG_GDB_BREAK_SUPPORT)
        # If we are supporting Ctrl-C interrupts from GDB, we must squirrel
        # away a pointer to the save interrupt state here so that we can
        # plant a breakpoint at some later time.

        .extern hal_saved_interrupt_state
        lwi     r3,  r0, cyg_scheduler_sched_lock
        swi     r19, r3, 0
#endif

        lwi     r5,  r19, CYGARC_MBREG_VECTOR         # retrieve decoded vector #

        lwi     r8,  r20, hal_interrupt_handlers      # get interrupt handler table and load routine pointer

        lwi     r6,  r20, hal_interrupt_data          # get interrupt data table and load data pointer
                                                      # R6 = data argument
        addik   r7,  r19, 0                           # R7 = saved registers

        brald   lr, r8                                # put isr address in ctr

        hal_nop

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK

        # If we are returning from the last nested interrupt, move back
        # to the thread stack. interrupt_end() must be called on the
        # thread stack since it potentially causes a context switch.
        # Since we have arranged for the top of stack location to
        # contain the sp we need to go back to here, just pop it off
        # and put it in SP.

        lwi     sp, sp,  CYGARC_MB_STACK_FRAME_SIZE

        addik   sp, sp, -CYGARC_MB_STACK_FRAME_SIZE   # create new stack frame
        swi     r0, sp, 0                             # LR = 0

#endif

        # On return r3 bit 1 will indicate whether a DSR is
        # to be posted. Pass this together with a pointer to
        # the interrupt object we have just used to the
        # interrupt tidy up routine. r3 must be moved to r5
        addik   r5, r3, 0

        # Note that r19 to r31 are defined to be preserved across
        # calls by the calling convention, so r20 and r21 still contain
        # the register dump and the vector number respectively.
        lwi     r6, r20, hal_interrupt_objects        # get interrupt object table and load object pointer

        addik   r7, r19, 0                            # arg3 = saved register dump

        .extern interrupt_end
        addik   r8, r0, interrupt_end
        brald   lr, r8                                # call into C to finish off

        hal_nop

restore_state:

        # All done, restore CPU state and continue

        # retrieve CPU state pointer
        addik   sp, sp, CYGARC_MB_STACK_FRAME_SIZE

        # Restore variant registers
        hal_variant_load sp

        hal_regs_load sp

        hal_cpu_int_disable

        # Restore MSR and PC / r3 again and SP
        lwi     r3,   sp, CYGARC_MBREG_MSR
        mts     rmsr, r3
        lwi     r3,   sp, CYGARC_MBREG_PC
        mts     rpc,  r3
        lwi     r14,  sp, CYGARC_MBREG_LR

        lwi     r3,   sp, CYGARC_MBREG_REGS+3*4
        lwi     sp,   sp, CYGARC_MBREG_REGS+1*4

        # Return from interrupt set IE back in MSR
        rtid r14,0                                    # and return

        hal_nop
FUNC_END(hal_default_interrupt_vsr)


//==========================================================================
// Execute pending DSRs on the interrupt stack with interrupts enabled.
// Note: this can only be called from code running on a thread stack

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK
    .extern cyg_interrupt_call_pending_DSRs

FUNC_START(hal_interrupt_stack_call_pending_DSRs)
        # Change to interrupt stack and save state, no need make stack space
        # for cyg_interrupt_call_pending_DSRs ?
        addik   r3,  sp, 0                            # R3 = SP

        hal_load_istack sp                            # SP = New stack
        addik   sp,  sp, -16

        swi     r0,  sp, 0                            # LR = 0
        swi     r3,  sp, 4                            # save old sp

        # Save MSR and PC
        mfs     r3,  rmsr
        swi     r3,  sp, 8                            # save old MSR
        swi     lr,  sp, 12                           # save old LR

        hal_cpu_int_enable

        # Call into kernel which will execute DSRs
        bralid  lr,  cyg_interrupt_call_pending_DSRs
        hal_nop

        lwi     lr,  sp, 12
        lwi     r5,  sp, 4                            # get SP from saved state

        lwi     r4,  sp, 8                            # restore interrupt setting
        hal_cpu_int_merge r3, r4

        addik   sp,  r5, 0                            # restore stack pointer
        rtsd    lr, 8                                 # Return to link register
        hal_nop
FUNC_END(hal_interrupt_stack_call_pending_DSRs)
#endif

//==========================================================================
// Profiling support _mcount
//
// GCC calls _mcount with brki r16, <_mcount> -> r16 contains the PC value
// Before calling eCos profiling function, save registers
// that may be modified across function call, r5 through r10.
//

#ifdef CYGPKG_PROFILE_GPROF
       .align  2
       .extern hal_mcount
FUNC_START(_mcount)
         addik sp, sp, -40
         # Leave offset 4 and 8 for hal_mcount to
         # possibly save the passing parameters
         # (R5 and R6)
         # Save registers
         swi   r5,  sp, 12
         swi   r6,  sp, 16
         swi   r7,  sp, 20
         swi   r8,  sp, 24
         swi   r9,  sp, 28
         swi   r10, sp, 32
         swi   r16, sp, 36
         # Store callee on stack and load it in r6
         addik r6, r15, 0
         # Load the caller in r5
         lwi   r5,  sp, 40
         # Call profiler
         bralid r15, hal_mcount
         hal_nop
         lwi   r5,  sp, 12
         lwi   r6,  sp, 16
         lwi   r7,  sp, 20
         lwi   r8,  sp, 24
         lwi   r9,  sp, 28
         lwi   r10, sp, 32
         lwi   r16, sp, 36
         addik sp,  sp, 40
         rtbd r16,  4
         hal_nop
FUNC_END(_mcount)
#endif

//==========================================================================
// end of vectors.S
