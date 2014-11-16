/*==========================================================================
//
//      hal_misc.c
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
// Original(s):  nickg for Cortex-M
// Date:         2011-02-09
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/hal.h>
#include <pkgconf/hal_mb.h>
#ifdef CYGPKG_KERNEL
# include <pkgconf/kernel.h>
#endif

#include <cyg/infra/diag.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros

#include <cyg/hal/hal_arch.h>          // HAL header
#include <cyg/hal/hal_intr.h>          // HAL header
#include <cyg/hal/hal_cache.h>         // HAL header

#include <cyg/hal/drv_api.h>


#if defined(CYGPKG_KERNEL_INSTRUMENT) &&        \
    defined(CYGDBG_KERNEL_INSTRUMENT_INTR)
# include <cyg/kernel/instrmnt.h>
#endif

#ifdef CYGSEM_HAL_MICROBLAZE_TRACE
void hal_mb_show_pvr( void );
# define hal_db diag_printf
#else
# define hal_db
#endif

//==========================================================================

typedef cyg_uint32 cyg_isr(cyg_uint32 vector, CYG_ADDRWORD data);

//==========================================================================
// External references

// VSRs in vectors.S
__externC void  hal_default_exception_vsr(void);
__externC void  hal_default_interrupt_vsr(void);

// HAL and eCos functions
__externC void  hal_system_init(void);
__externC void  hal_variant_init(void);
__externC void  hal_platform_init(void);
__externC void  hal_ctrlc_isr_init(void);
__externC void  initialize_stub(void);
__externC void  cyg_hal_invoke_constructors(void);
__externC void  cyg_start(void);

// DATA and BSS locations
__externC cyg_uint32 __ram_data_start;
__externC cyg_uint32 __ram_data_end;
__externC cyg_uint32 __rom_data_start;
__externC cyg_uint32 __bss_start;
__externC cyg_uint32 __bss_end;

// Scheduler lock
__externC volatile cyg_uint32 cyg_scheduler_sched_lock;

#ifdef CYGHWR_HAL_MICROBLAZE_SUPPORT_EXCEPTION

externC void __handle_exception (void);
externC HAL_SavedRegisters *_hal_registers;
# ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS
externC void* volatile __mem_fault_handler;
# endif

#endif // CYGHWR_HAL_MICROBLAZE_SUPPORT_EXCEPTION

// Local functions
static void hal_enable_caches(void);

//==========================================================================
// Interrupt tables

volatile CYG_ADDRESS  hal_interrupt_handlers[CYGNUM_HAL_ISR_COUNT];
volatile CYG_ADDRWORD hal_interrupt_data[CYGNUM_HAL_ISR_COUNT];
volatile CYG_ADDRESS  hal_interrupt_objects[CYGNUM_HAL_ISR_COUNT];


//==========================================================================
// Main entry point
//
// Enter here from reset via slot 1 of VSR table. The stack pointer is
// already set to the value in VSR slot 0, usually the top of internal
// SRAM.

void
hal_reset_vsr(void)
{
    // Call system init routine. This should do the minimum necessary
    // for the rest of the initialization to complete. For example set
    // up GPIO, the SRAM, power management etc. This routine is
    // usually supplied by the platform HAL. Calls to
    // hal_variant_init() and hal_platform_init() later will perform
    // the main initialization.

    hal_system_init();

    // Initialize vector table
    {

#if !defined(CYG_HAL_STARTUP_RAM)
        register int i;

        // Only install the exception vectors for non-RAM startup. For
        // RAM startup we want these to continue to point to the original
        // VSRs, which will belong to RedBoot or GDB stubs.

        for( i = CYGNUM_HAL_VSR_MIN; i <= CYGNUM_HAL_VSR_MAX; i++ )
            hal_vsr_table[i] = (CYG_ADDRESS)hal_default_exception_vsr;

#endif

        // Initialize interrupt handler for all startup type
        hal_vsr_table[CYGNUM_HAL_VECTOR_EXTERNAL] = (CYG_ADDRESS)hal_default_interrupt_vsr;

    }

#if defined(CYG_HAL_STARTUP_ROM)
    // Relocate data from ROM to RAM
    {
        register cyg_uint32 *p,
                       *q;
        for (p = &__ram_data_start, q = &__rom_data_start;
             p < &__ram_data_end; p++, q++)
            *p = *q;
    }
#endif

    // Clear BSS
    {
        register cyg_uint32 *p;
        for (p = &__bss_start; p < &__bss_end; p++)
            *p = 0;
    }

    // Initialized ISR table with default entry
    {
        register int i;

        for( i = 0; i < CYGNUM_HAL_ISR_COUNT; i++ )
        {
           hal_interrupt_handlers[i] = (CYG_ADDRESS)hal_default_isr;
        }
    }

    // Call variant and platform init routines
    hal_variant_init();

    hal_platform_init();

    // Enable Data and Instruction caches
    hal_enable_caches();

    // Start up the system clock
    HAL_CLOCK_INITIALIZE( CYGNUM_HAL_RTC_PERIOD );

#ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS

    initialize_stub();

#endif

#if defined(CYGDBG_HAL_DEBUG_GDB_CTRLC_SUPPORT) || \
    defined(CYGDBG_HAL_DEBUG_GDB_BREAK_SUPPORT)

    hal_ctrlc_isr_init();

#endif

    // Run through static constructors
    cyg_hal_invoke_constructors();

#ifdef CYGSEM_HAL_MICROBLAZE_TRACE
    hal_mb_show_pvr( );
#endif

    // Finally call into application
    cyg_start();

    for (;;);
}

//==========================================================================
// C++ support - run initial constructors

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


//==========================================================================
// GDB support
//
// These functions translate between HAL saved contexts and GDB
// register dumps.

__externC void
hal_get_gdb_registers(HAL_MB_GDB_Registers * gdbreg,
                      HAL_SavedRegisters * regs)
{

}

__externC void
hal_set_gdb_registers(HAL_MB_GDB_Registers * gdbreg,
                      HAL_SavedRegisters * regs)
{


}


#ifdef CYGHWR_HAL_MICROBLAZE_SUPPORT_EXCEPTION

//==========================================================================
// Handle Exceptions
//
// Exceptions are passed here from the initial VSR. We pass the
// exception on to GDB stubs or the kernel as appropriate.

void hal_deliver_exception( HAL_SavedRegisters *regs )
{
    cyg_uint32 vector = regs->vector;

    // Adjust the return address according to the type
    // of exception
    switch( vector ){
    case CYGNUM_HAL_VECTOR_USER_EXCEPTION:
       regs->lr = regs->d[15]; // recommended value
       break;
    case CYGNUM_HAL_VECTOR_SOFT_HARD_BREAK:
       regs->lr = regs->d[16];
       break;
    case CYGNUM_HAL_VECTOR_HARD_FAULT:
       regs->lr = regs->d[17];
       break;
    default:
       CYG_FAIL("Unknown PROGRAM exception!!");
    }

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
#endif

#if defined(CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS)

    _hal_registers = regs;
    __handle_exception();

#elif defined(CYGPKG_KERNEL_EXCEPTIONS)
    if ( vector==CYGNUM_HAL_VECTOR_HARD_FAULT ) {
        cyg_uint32 esr_v;
        CYGARC_MFS( resr, esr_v );
        vector = esr_v & 0x1f;
    }
    cyg_hal_deliver_exception( vector, (CYG_ADDRWORD)regs );

#else

    CYG_FAIL("Exception!!!");

#endif
}

#endif // CYGHWR_HAL_MICROBLAZE_SUPPORT_EXCEPTION


#ifdef CYGSEM_HAL_MICROBLAZE_TRACE

void
hal_mb_show_pvr( void )
{
   cyg_uint32 msr;
   cyg_uint32 pvr;
   cyg_uint8  ver[0x13][7] = \
             { "------\0", "v5.00a\0", "v5.00b\0", "v5.00c\0", \
               "v6.00a\0", "v6.00b\0", "v7.00a\0", \
               "v7.00b\0", "v7.10a\0", "v7.10b\0", \
               "v7.10c\0", "v7.10d\0", "v7.20a\0", \
               "v7.20b\0", "v7.20c\0", "v7.20d\0", \
               "v7.30a\0", "v7.30b\0", "v8.00a\0" };
   cyg_uint8 idx = 0;

   CYGARC_MFS( rmsr, msr );

   if ( msr & CYGARC_REG_MSR_PVR ) {
      CYGARC_MFS( rpvr0, pvr );
      hal_db("\n\rMicroblaze features:\n\r");

      if( pvr & CYGARC_REG_PVR0_BS ) {
          hal_db("- Barrel shifter\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_DIV ) {
          hal_db("- Divider\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_MUL ) {
          hal_db("- Hardware multiplier\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_FPU ) {
          hal_db("- FPU\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_EXC ) {
          hal_db("- Exceptions\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_ICU ) {
          hal_db("- Instruction cache\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_DCU ) {
          hal_db("- Data cache\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_MMU ) {
          hal_db("- MMU\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_BTC ) {
          hal_db("- Branch target cache\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_ENDI ) {
          hal_db("- Little-endian\n\r");
      } else {
          hal_db("- Big-endian\n\r");
      }
      if( pvr & CYGARC_REG_PVR0_FT ) {
          hal_db("- Fault tolerant\n\r");
      }
      idx = CYGARC_REG_PVR0_MBV(pvr);
      if( idx < 0x13 )
          hal_db("- Version %6s \n\r", (char *) &ver[ idx ] );
      else
          hal_db("- Version 0x%d \n\r", idx );
   } else {
      hal_db("Microblaze PVR not available\n\r");
   }
}

#endif


//==========================================================================
// Interrupt masking and configuration

#ifndef HAL_VAR_INTERRUPT_MASK
#define HAL_VAR_INTERRUPT_MASK( __vector ) CYG_EMPTY_STATEMENT
#define HAL_VAR_INTERRUPT_UNMASK( __vector ) CYG_EMPTY_STATEMENT
#define HAL_VAR_INTERRUPT_SET_LEVEL( __vector, __level ) CYG_EMPTY_STATEMENT
#define HAL_VAR_INTERRUPT_ACKNOWLEDGE( __vector ) CYG_EMPTY_STATEMENT
#define HAL_VAR_INTERRUPT_CONFIGURE( __vector, __level, __up ) CYG_EMPTY_STATEMENT
#endif

__externC void hal_interrupt_mask( cyg_uint32 vector )
{
    HAL_VAR_INTERRUPT_MASK( vector );
}

//--------------------------------------------------------------------------

__externC void hal_interrupt_unmask( cyg_uint32 vector )
{
    HAL_VAR_INTERRUPT_UNMASK( vector );
}

//--------------------------------------------------------------------------

__externC void hal_interrupt_acknowledge( cyg_uint32 vector )
{
    HAL_VAR_INTERRUPT_ACKNOWLEDGE( vector );
}

//--------------------------------------------------------------------------

__externC void hal_interrupt_configure( cyg_uint32 vector, cyg_uint32 level, cyg_uint32 up )
{
    HAL_VAR_INTERRUPT_CONFIGURE( vector, level, up );
}

//--------------------------------------------------------------------------

__externC void hal_interrupt_set_level( cyg_uint32 vector, cyg_uint32 level )
{
    HAL_VAR_INTERRUPT_SET_LEVEL( vector, level );
}


//==========================================================================
// Architecture default ISR

__externC cyg_uint32
hal_arch_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
    CYG_TRACE1(true, "Interrupt: %d", vector);

    CYG_FAIL("Spurious Interrupt!!!");

    return 0;
}


//---------------------------------------------------------------------------
// Initial cache enabling
// Specific behavior for each platform configured via plf_cache.h

static void
hal_enable_caches(void)
{
    // When reaching here, the caches are always disable for ROM, ROMRAM or
    // JTAG startup type, there is nothing in cache, it has not been used
    // yet, we can invalidate all the cache lines.

    // For RAM startup type, the cache is setup by RedBoot or other
    // ROM / ROMRAM boot code so we can't invalidate the data cache as
    // it might not be synchronized with the memory. It's up to the
    // boot code to leave the cache in a correct state
#if !(defined(CYG_HAL_STARTUP_RAM))
    // Invalidate caches
    HAL_DCACHE_INVALIDATE_ALL();
    HAL_ICACHE_INVALIDATE_ALL();
#endif

#ifdef CYGSEM_HAL_ENABLE_ICACHE_ON_STARTUP
    HAL_ICACHE_ENABLE();
#endif

#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    HAL_DCACHE_ENABLE();
#endif

}


//==========================================================================
// Microsecond delay
//
// The system RTC is set up to tick at 1MHz so all we need to do here
// is count ticks.

__externC void
hal_delay_us(cyg_int32 us)
{
    cyg_uint32      t0,
                    t1;

    HAL_CLOCK_READ(&t0);
    while (us > 0) {
        HAL_CLOCK_READ(&t1);
        if (t1 < t0)
            us -= (t1 + CYGNUM_HAL_RTC_PERIOD - t0);
        else
            us -= t1 - t0;
        t0 = t1;
    }
}

//---------------------------------------------------------------------------
// GPROF mcount implementation based on i368 code
// hal_mcount(  CYG_ADDRWORD caller, CYG_ADDRWORD callee ) is called from
// the _mcount routine in vector.s. The _mcount assembly routine takes care
// of saving / restoring the stack

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

    this_cpu = HAL_SMP_CPU_THIS();
    if ( mcount_cpu != this_cpu ) {
        HAL_SPINLOCK_SPIN( mcount_lock );
        mcount_cpu  = this_cpu;
        __profile_mcount(caller, callee);
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

# endif // SMP

#endif // CYGPKG_PROFILE_GPROF

//==========================================================================
// EOF hal_misc.c
