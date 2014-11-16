//==========================================================================
//
//      hal_misc.c
//
//      HAL miscellaneous functions
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2007, 2008 Free Software Foundation, Inc.
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
// Author(s):    nickg, jskov
// Contributors: nickg, jskov,
//               jlarmour, gthomas
// Date:         1999-02-20
// Purpose:      HAL miscellaneous functions
// Description:  This file contains miscellaneous functions provided by the
//               HAL.
//
//####DESCRIPTIONEND####
//
//===========================================================================

#include <pkgconf/hal.h>

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <cyg/hal/ppc_regs.h>           // SPR definitions

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros
#include <cyg/infra/diag.h>             // diag_printf

#include <cyg/hal/hal_arch.h>           // HAL header
#include <cyg/hal/hal_cache.h>          // HAL cache
#if defined(CYGFUN_HAL_COMMON_KERNEL_SUPPORT) && \
    defined(CYGPKG_HAL_EXCEPTIONS)
# include <cyg/hal/hal_intr.h>           // HAL interrupts/exceptions
#endif
#include <cyg/hal/hal_mem.h>            // HAL memory handling

#if defined(CYGPKG_PROFILE_GPROF)
#  include <cyg/profile/profile.h>
#endif

//---------------------------------------------------------------------------
// Functions used during initialization.

#ifdef CYGSEM_HAL_STOP_CONSTRUCTORS_ON_FLAG
cyg_bool cyg_hal_stop_constructors;
#endif

typedef void (*pfunc) (void);
extern pfunc __CTOR_LIST__[];
extern pfunc __CTOR_END__[];

void
cyg_hal_invoke_constructors (void)
{
#ifdef CYGSEM_HAL_STOP_CONSTRUCTORS_ON_FLAG
    static pfunc *p = &__CTOR_END__[-1];
    
    cyg_hal_stop_constructors = 0;
    for (; p >= __CTOR_LIST__; p--) {
        (*p) ();
        if (cyg_hal_stop_constructors) {
            p--;
            break;
        }
    }
#else
    pfunc *p;

    for (p = &__CTOR_END__[-1]; p >= __CTOR_LIST__; p--)
        (*p) ();
#endif
}

// Override any __eabi the compiler might generate. We don't want
// constructors to be called twice.
void __eabi (void) {}

//---------------------------------------------------------------------------
// First level C exception handler.

externC void __handle_exception (void);

externC HAL_SavedRegisters *_hal_registers;

#ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS
externC void* volatile __mem_fault_handler;
#endif

void
cyg_hal_exception_handler(HAL_SavedRegisters *regs)
{
#ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS

    // If we caught an exception inside the stubs, see if we were expecting it
    // and if so jump to the saved address
    if (__mem_fault_handler) {
        regs->pc = (CYG_ADDRWORD)__mem_fault_handler;
        return; // Caught an exception inside stubs        
    }

    // Set the pointer to the registers of the current exception
    // context. At entry the GDB stub will expand the
    // HAL_SavedRegisters structure into a (bigger) register array.
    _hal_registers = regs;

    __handle_exception();

#ifdef CYGPKG_HAL_QUICC
    {
        // This is unpleasant: it appears that if we interrupt the board
        // using ^C coming in on the QUICC's SMC1, by planting a breakpoint
        // at the interrupt return address, the decrementer interrupt is
        // not taken when the bp exception returns AND WORSE no other
        // interrupt is possible until the decrementer fires again.  This
        // does not apply to simple "incoming character" interrupts; it
        // seems it has to be combined with an immediate trap on RTI for
        // this to occur.
        // 
        // The solution is to test for decrementer underflow after the
        // (any) exception, and maybe reinitialize the decrementer.  If the
        // decrementer interrupt gets taken, that causes decrementer reinit
        // too, and no harm is done.

        cyg_uint32 result;
        asm volatile(
            "mfdec  %0;"
            : "=r"(result)
            );

        if ( CYGNUM_HAL_RTC_PERIOD < result ) {
            // then we missed a tick, but the exception masked it
            // reset the decrementer here
            asm volatile(
                "mtdec  %0;"
                : : "r"(CYGNUM_HAL_RTC_PERIOD)
                );
        }
    }
#endif

#elif defined(CYGFUN_HAL_COMMON_KERNEL_SUPPORT) && \
      defined(CYGPKG_HAL_EXCEPTIONS)
    int vector = regs->vector>>CYGHWR_HAL_POWERPC_VECTOR_ALIGNMENT;

    // We should decode the vector and pass a more appropriate
    // value as the second argument. For now we simply pass a
    // pointer to the saved registers. We should also divert
    // breakpoint and other debug vectors into the debug stubs.

#ifndef CYGHWR_HAL_POWERPC_BOOK_E
    if (vector==CYGNUM_HAL_VECTOR_PROGRAM) {
        int srr1;
        CYGARC_MFSPR(CYGARC_REG_SRR1, srr1); // get srr1

        switch ((srr1 >> 17) & 0xf) {
        case 1:
            vector = CYGNUM_HAL_EXCEPTION_TRAP;
            break;
        case 2:
            vector = CYGNUM_HAL_EXCEPTION_PRIVILEGED_INSTRUCTION;
            break;
        case 4:
            vector = CYGNUM_HAL_EXCEPTION_ILLEGAL_INSTRUCTION;
            break;
        case 8:
            vector = CYGNUM_HAL_EXCEPTION_FPU;
            break;
        default:
            CYG_FAIL("Unknown PROGRAM exception!!");
        }
    }
#endif
    
    cyg_hal_deliver_exception( vector, (CYG_ADDRWORD)regs );

#else

    CYG_FAIL("Exception!!!");
    
#endif    
    
    return;
}

//---------------------------------------------------------------------------
// Default ISRs

#ifndef CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT
externC cyg_uint32
hal_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    diag_printf("Interrupt: %d\n", vector);

    CYG_FAIL("Spurious Interrupt!!!");
    return 0;
}
#else
externC cyg_uint32
hal_arch_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    return 0;
}
#endif

// The decrementer default ISR has to do nothing. The reason is that
// decrementer interrupts cannot be disabled - if a kernel configuration
// does not use the RTC, but does use external interrupts, the decrementer
// underflow could cause a CYG_FAIL (as above) even though the user did
// not expect any decrementer interrupts to happen.
externC cyg_uint32
hal_default_decrementer_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    return 0;
}

//---------------------------------------------------------------------------
// Idle thread action

externC bool hal_variant_idle_thread_action(cyg_uint32);

void
hal_idle_thread_action( cyg_uint32 count )
{
    // Execute variant idle thread action, while allowing it to control
    // whether to run any of the architecture action code.
    if (!hal_variant_idle_thread_action(count))
        return;

#if 0
    do {
        register cyg_uint32 dec;

        asm volatile(
            "mfdec  %0;"
            : "=r"(dec)
            );
        diag_printf( "Decrementer %08x\n", dec);
    } while (0);
#endif
}

//---------------------------------------------------------------------------
// Use MMU resources to map memory regions.  
// This relies that the platform HAL providing an
//          externC cyg_memdesc_t cyg_hal_mem_map[];
// as detailed in hal_cache.h, and the variant HAL providing the
// MMU mapping/clear functions.
externC void
hal_MMU_init (void)
{
    int id = 0;
    int i  = 0;

    cyg_hal_clear_MMU ();

    while (cyg_hal_mem_map[i].size) {
        id = cyg_hal_map_memory (id, 
                                 cyg_hal_mem_map[i].virtual_addr,
                                 cyg_hal_mem_map[i].physical_addr,
                                 cyg_hal_mem_map[i].size,
                                 cyg_hal_mem_map[i].flags);
        i++;
    }
}

//---------------------------------------------------------------------------
// Initial cache enabling
// Specific behavior for each platform configured via plf_cache.h

externC void
hal_enable_caches(void)
{
#ifndef HAL_CACHE_UNIFIED
        
#if !(defined(CYG_HAL_STARTUP_RAM) || defined(CYG_HAL_STARTUP_JTAG))
    // Invalidate caches
    HAL_DCACHE_INVALIDATE_ALL();
    HAL_ICACHE_INVALIDATE_ALL();
#endif

#ifdef CYGSEM_HAL_ENABLE_ICACHE_ON_STARTUP
#ifdef HAL_ICACHE_UNLOCK_ALL
    HAL_ICACHE_UNLOCK_ALL();
#endif
    HAL_ICACHE_ENABLE();
#endif

#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
#ifdef HAL_DCACHE_UNLOCK_ALL
    HAL_DCACHE_UNLOCK_ALL();
#endif
    HAL_DCACHE_ENABLE();
#ifdef HAL_DCACHE_WRITE_MODE
#ifdef CYGSEM_HAL_DCACHE_STARTUP_MODE_COPYBACK
    HAL_DCACHE_WRITE_MODE(HAL_DCACHE_WRITEBACK_MODE);
#else
    HAL_DCACHE_WRITE_MODE(HAL_DCACHE_WRITETHRU_MODE);
#endif
#endif
#endif

#else // HAL_CACHE_UNIFIED


#if !(defined(CYG_HAL_STARTUP_RAM) || defined(CYG_HAL_STARTUP_JTAG))
    // Invalidate cache
    HAL_UCACHE_INVALIDATE_ALL();
#endif

    
#if defined(CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP)

    
    HAL_UCACHE_ENABLE();    

#endif
    
#endif // HAL_CACHE_UNIFIED
    
}

//---------------------------------------------------------------------------
//A jump via a null pointer causes the CPU to end up here.
externC void
hal_null_call(void)
{
    
    CYG_FAIL("Call via NULL-pointer!");
    for(;;);
}

//---------------------------------------------------------------------------
// GPROF mcount implementation based on i368 code
// hal_mcount(  CYG_ADDRWORD caller, CYG_ADDRWORD callee ) is called from the
// _mcount routine in vector.s. The _mcount assembly routine takes care of 
// saving / restoring the stack

#ifdef CYGPKG_PROFILE_GPROF

# ifdef CYGPKG_HAL_SMP_SUPPORT
static HAL_SMP_CPU_TYPE     mcount_cpu  = HAL_SMP_CPU_NONE;
static HAL_SPINLOCK_TYPE    mcount_lock = HAL_SPINLOCK_INIT_CLEAR;

externC void
hal_mcount( CYG_ADDRWORD caller, CYG_ADDRWORD callee )
{
    int istate;
    HAL_SMP_CPU_TYPE this_cpu;
    
    HAL_DISABLE_INTERRUPTS( istate );

    // This cpu is now not going to run any other code. So, did it
    // already own the spinlock?
    this_cpu = HAL_SMP_CPU_THIS();
    if ( mcount_cpu != this_cpu ) {
        // Nope, so this cannot be a nested call to mcount()
        HAL_SPINLOCK_SPIN( mcount_lock );
        // And no other cpu is executing inside mcount() either
        mcount_cpu  = this_cpu;
        // A possibly-recursive call is now safe.
        __profile_mcount(caller,
                         callee);
        // All done.
        mcount_cpu = HAL_SMP_CPU_NONE;
        HAL_SPINLOCK_CLEAR( mcount_lock);
    }
    
    HAL_RESTORE_INTERRUPTS( istate  );
}

# else   // ! SMP

static int mcount_nested;

externC void
hal_mcount( CYG_ADDRWORD caller, CYG_ADDRWORD callee )
{
    int istate;
    HAL_DISABLE_INTERRUPTS( istate );
    if (! mcount_nested) {
        mcount_nested   = 1;
        __profile_mcount(caller,
                         callee);
        mcount_nested   = 0;
    }
    HAL_RESTORE_INTERRUPTS( istate );
}

#endif // SMP

#endif // GPROF

//---------------------------------------------------------------------------
// End of hal_misc.c