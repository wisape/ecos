#ifndef CYGONCE_HAL_ARCH_H
#define CYGONCE_HAL_ARCH_H
/*==========================================================================
//
//      hal_arch.h
//
//      Microblaze architecture abstractions
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
// Original(s):  nickg for Cortex-M
// Date:         2011-02-03
// Description:  Define architecture abstractions
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/system.h>
#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/var_arch.h>

//==========================================================================
// CPU save state
//
//
// Register   0:    Zero register                 - Dedicated
// Register   1:    Stack Pointer                 - Dedicated
// Register   2:    RO small data                 - Dedicated
// Registers  3-4:  Return values                 - Volatile
// Registers  5-10: Parameter passing             - Volatile
// Registers 11-12: Temporary values              - Volatile
// Register  13:    RW small data                 - Dedicated
// Register  14:    Return address for interrupt  - Dedicated
// Register  15:    Return address for subroutine - Dedicated
// Register  16:    Return address for trap       - Dedicated
// Register  17:    Return address for exceptions - Dedicated
// Register  18:    Reserved                      - Dedicated
// Registers 19:    General purpose               - Non-volatile
// Registers 20:    General purpose               - Dedicated or Non-volatile
// Registers 21-31: General purpose               - Non-volatile
//

typedef struct 
{
    // These are common to all saved states
    cyg_uint32   d[32];                 // Data Registers (R0 not used)

    // These are saved for exceptions and interrupts, but may also
    // be saved in a context switch if thread-aware debugging is enabled.
    cyg_uint32   msr;                   // Machine State Register
    cyg_uint32   pc;                    // Program Counter

    // Link register used when returning from interrupt or exception
    cyg_uint32   lr;

    // This marks the limit of state saved during a context switch and
    // is used to calculate necessary stack allocation for context switches.
    // It would probably be better to have a union instead...
    cyg_uint32   context_size[0];

    // These are only saved for exceptions and interrupts
    cyg_uint32   vector;                // Vector number

} HAL_SavedRegisters;


//-----------------------------------------------------------------------------
// eABI, cTODO: what is the microblaze ABI value? keep 52 now

#define CYGARC_MB_STACK_FRAME_SIZE      52      // size of a stack frame
// Compare to PPC, only the Link Register is saved, not the stack pointer

//==========================================================================
// Thread context initialization

#define HAL_THREAD_INIT_CONTEXT( _sparg_, _thread_, _entry_, _id_ )           \
    CYG_MACRO_START                                                           \
    register CYG_WORD _sp_ = (((CYG_WORD)_sparg_) &~15)                       \
                                 - CYGARC_MB_STACK_FRAME_SIZE;                \
    register HAL_SavedRegisters *_regs_;                                      \
    int _i_;                                                                  \
    ((CYG_WORD *)_sp_)[0] = 0;                 /* Zero old LR for EABI */     \
    _regs_ = (HAL_SavedRegisters *)((_sp_) - sizeof(HAL_SavedRegisters));     \
    for( _i_ = 1; _i_ < 32; _i_++ ) (_regs_)->d[_i_] = (_id_)|_i_;            \
    (_regs_)->d[01] = (CYG_WORD)(_sp_);        /* SP = top of stack      */   \
    (_regs_)->d[05] = (CYG_WORD)(_thread_);    /* R5 = arg1 = thread ptr */   \
    (_regs_)->d[15] = (CYG_WORD)(_entry_);     /* R15 = LR = _entry_ */       \
    (_regs_)->pc = (CYG_WORD)(_entry_);        /* set PC for thread dbg  */   \
    (_regs_)->msr = CYGARC_REG_MSR_IE;         /* MSR = enable irqs      */   \
    _sparg_ = (CYG_ADDRESS)_regs_;                                            \
    CYG_MACRO_END

//==========================================================================
// Context switch macros.
// The arguments are pointers to locations where the stack pointer
// of the current thread is to be stored, and from where the SP of the
// next thread is to be fetched.

__externC void hal_thread_switch_context( CYG_ADDRESS to, CYG_ADDRESS from );
__externC void hal_thread_load_context( CYG_ADDRESS to ) __attribute__ ((noreturn));

#define HAL_THREAD_SWITCH_CONTEXT(__fspptr,__tspptr)                    \
        hal_thread_switch_context((CYG_ADDRESS)__tspptr,                \
                                  (CYG_ADDRESS)__fspptr);

#define HAL_THREAD_LOAD_CONTEXT(__tspptr)                               \
        hal_thread_load_context( (CYG_ADDRESS)__tspptr );

//==========================================================================
// Exception handling function
// This function is defined by the kernel according to this prototype. It is
// invoked from the HAL to deal with any CPU exceptions that the HAL does
// not want to deal with itself. It usually invokes the kernel's exception
// delivery mechanism.

externC void cyg_hal_deliver_exception( CYG_WORD code, CYG_ADDRWORD data );

//--------------------------------------------------------------------------
// Bit manipulation macros

#define HAL_LSBIT_INDEX(__index, __mask)                                \
{                                                                       \
    register cyg_uint32 __bit = (__mask);                               \
    register int __count = (__mask) ? 32 : -1;                          \
    while( __bit ) {                                                    \
      __bit = __bit << 1;                                               \
      __count--;                                                        \
    }                                                                   \
    (__index) = __count;                                                \
}

#define HAL_MSBIT_INDEX(__index, __mask)                                \
{                                                                       \
    register cyg_uint32 __bit = (__mask);                               \
    register int __count = -1;                                          \
    while( __bit ) {                                                    \
      __bit = __bit >> 1;                                               \
      __count++;                                                        \
    }                                                                   \
    (__index) = __count;                                                \
}


//==========================================================================
// Execution reorder barrier.
// When optimizing the compiler can reorder code. In multithreaded systems
// where the order of actions is vital, this can sometimes cause problems.
// This macro may be inserted into places where reordering should not happen.

#define HAL_REORDER_BARRIER() asm volatile ( "" : : : "memory" )

//==========================================================================
// Breakpoint support
// HAL_BREAKPOINT() is a code sequence that will cause a breakpoint to happen
// if executed.
// HAL_BREAKINST is the value of the breakpoint instruction and 
// HAL_BREAKINST_SIZE is its size in bytes.


# define HAL_BREAKINST_SIZE      4
# define HAL_BREAKINST_TYPE      cyg_uint32
# define HAL_BREAKINST           0xba0c0018         // BRKI

# define HAL_BREAKPOINT(_label_)                        \
__asm__ volatile (" .globl  " #_label_ ";"              \
              #_label_":"                               \
              "brki r16, 0x18;"                         \
    );


//==========================================================================
// GDB support

// Register layout expected by GDB
typedef struct
{
  cyg_uint32  gpr[32];
  cyg_uint32  pc;
  cyg_uint32  msr;
} HAL_MB_GDB_Registers;


// Translate a stack pointer as saved by the thread context macros
// into a pointer to a HAL_SavedRegisters structure. On the Cortex-M
// these are equivalent.

#define HAL_THREAD_GET_SAVED_REGISTERS(__stack, __regs) \
    CYG_MACRO_START                                     \
    (__regs)    = (HAL_SavedRegisters*)(__stack);       \
    CYG_MACRO_END


__externC void hal_get_gdb_registers( HAL_MB_GDB_Registers *gdbreg, HAL_SavedRegisters *regs );
__externC void hal_set_gdb_registers( HAL_MB_GDB_Registers *gdbreg, HAL_SavedRegisters *regs );

#define HAL_GET_GDB_REGISTERS(__regval, __regs) hal_get_gdb_registers( (HAL_MB_GDB_Registers *)(__regval), (HAL_SavedRegisters *)(__regs) )
#define HAL_SET_GDB_REGISTERS(__regs, __regval) hal_set_gdb_registers( (HAL_MB_GDB_Registers *)(__regval), (HAL_SavedRegisters *)(__regs) )

//==========================================================================
// HAL setjmp


//-----------------------------------------------------------------------------
// HAL setjmp

typedef struct {
    cyg_uint32 sp;
    cyg_uint32 r2;
    cyg_uint32 r13;
    cyg_uint32 r19;
    cyg_uint32 r20;
    cyg_uint32 r21;
    cyg_uint32 r22;
    cyg_uint32 r23;
    cyg_uint32 r24;
    cyg_uint32 r25;
    cyg_uint32 r26;
    cyg_uint32 r27;
    cyg_uint32 r28;
    cyg_uint32 r29;
    cyg_uint32 r30;
    cyg_uint32 r31;
    cyg_uint32 lr;
} hal_jmp_buf_t;

#define CYGARC_JMP_BUF_SIZE      (sizeof(hal_jmp_buf_t) / sizeof(cyg_uint32))

typedef cyg_uint32 hal_jmp_buf[CYGARC_JMP_BUF_SIZE];

__externC int hal_setjmp(hal_jmp_buf env);
__externC void hal_longjmp(hal_jmp_buf env, int val);


//--------------------------------------------------------------------------
// Idle thread code.
// This macro is called in the idle thread loop, and gives the HAL the
// chance to insert code. Typical idle thread behaviour might be to halt the
// processor. Here we only supply a default fallback if the variant/platform
// doesn't define anything.

#ifndef HAL_IDLE_THREAD_ACTION
#define HAL_IDLE_THREAD_ACTION(_count_) CYG_EMPTY_STATEMENT
#endif

//==========================================================================
// Minimal and sensible stack sizes: the intention is that applications
// will use these to provide a stack size in the first instance prior to
// proper analysis.  Idle thread stack should be this big.

//    THESE ARE NOT INTENDED TO BE MICROMETRICALLY ACCURATE FIGURES.
//           THEY ARE HOWEVER ENOUGH TO START PROGRAMMING.
// YOU MUST MAKE YOUR STACKS LARGER IF YOU HAVE LARGE "AUTO" VARIABLES!

// This is not a config option because it should not be adjusted except
// under "enough rope" sort of disclaimers.

// A minimal, optimized stack frame - space for return link plus four
// arguments or local variables.
#define CYGNUM_HAL_STACK_FRAME_SIZE (4 * 20)

// Stack needed for a context switch
#define CYGNUM_HAL_STACK_CONTEXT_SIZE \
    (34*4 /* offsetof(HAL_SavedRegisters, context_size) */)

// Interrupt + call to ISR, interrupt_end() and the DSR
#define CYGNUM_HAL_STACK_INTERRUPT_SIZE \
    ((36*4 /* sizeof(HAL_SavedRegisters) */) + 2 * CYGNUM_HAL_STACK_FRAME_SIZE)

// We have lots of registers so no particular amount is added in for
// typical local variable usage.

// We define a minimum stack size as the minimum any thread could ever
// legitimately get away with. We can throw asserts if users ask for less
// than this. Allow enough for three interrupt sources - clock, serial and
// one other

#ifdef CYGFUN_KERNEL_THREADS_STACK_CHECKING
#define CYGNUM_HAL_STACK_CHECKING_OVERHEAD (2*CYGNUM_KERNEL_THREADS_STACK_CHECK_DATA_SIZE)
#else
#define CYGNUM_HAL_STACK_CHECKING_OVERHEAD 0
#endif

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK

// An interrupt stack which is large enough for all possible interrupt
// conditions (and only used for that purpose) exists.  "User" stacks
// can therefore be much smaller

# define CYGNUM_HAL_STACK_SIZE_MINIMUM \
         (16*CYGNUM_HAL_STACK_FRAME_SIZE + \
          2*CYGNUM_HAL_STACK_INTERRUPT_SIZE + \
          CYGNUM_HAL_STACK_CHECKING_OVERHEAD)

#else

// No separate interrupt stack exists.  Make sure all threads contain
// a stack sufficiently large
# define CYGNUM_HAL_STACK_SIZE_MINIMUM                  \
        (((2+3)*CYGNUM_HAL_STACK_INTERRUPT_SIZE) +      \
         (16*CYGNUM_HAL_STACK_FRAME_SIZE) +             \
         CYGNUM_HAL_STACK_CHECKING_OVERHEAD)
#endif

// Now make a reasonable choice for a typical thread size. Pluck figures
// from thin air and say 30 call frames with an average of 16 words of
// automatic variables per call frame
#define CYGNUM_HAL_STACK_SIZE_TYPICAL                \
        (CYGNUM_HAL_STACK_SIZE_MINIMUM +             \
         (30 * (CYGNUM_HAL_STACK_FRAME_SIZE+(16*4))) + \
         CYGNUM_HAL_STACK_CHECKING_OVERHEAD)

//==========================================================================
// Macros for switching context between two eCos instances (jump from
// code in ROM to code in RAM or vice versa).

#define CYGARC_HAL_SAVE_GP()
#define CYGARC_HAL_RESTORE_GP()

//==========================================================================
#endif //CYGONCE_HAL_ARCH_H

