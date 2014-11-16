/*==========================================================================
//
//      mb_generic_misc.c
//
//      Microblaze generic platform HAL functions
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
// Date:         2011-02-12
// Description:  
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/hal.h>
#include <pkgconf/hal_mb.h>
#include <pkgconf/hal_mb_generic.h>
#ifdef CYGPKG_KERNEL
#include <pkgconf/kernel.h>
#endif

#include <cyg/infra/diag.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_arch.h>           // HAL header
#include <cyg/hal/hal_intr.h>           // HAL header
#include <cyg/hal/hal_if.h>             // HAL header

cyg_uint32 _period;

//==========================================================================

void hal_variant_init( void )
{

#ifdef CYGPKG_HAL_MICROBLAZE_IPC
# ifdef CYGPKG_HAL_MICROBLAZE_IPC_BLOCK_XPS_INTC
    // Enable interrupts, can only be disable by restart
    HAL_WRITE_UINT32( CYGHWR_HAL_MB_XPS_INTC_BASE + CYGHWR_HAL_MB_XPS_INTC_MER, \
            ( CYGHWR_HAL_MB_XPS_INTC_MER_HIE | CYGHWR_HAL_MB_XPS_INTC_MER_ME ) );
# endif
#endif

#ifdef CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT
   hal_if_init();
#endif

}


#ifdef CYGPKG_PROFILE_GPROF
//--------------------------------------------------------------------------
//
// Profiling support - uses a separate high-speed timer
//

# include <cyg/hal/hal_arch.h>
# include <cyg/hal/hal_intr.h>
# include <cyg/profile/profile.h>
# include <cyg/hal/drv_api.h> // CYG_ISR_HANDLED

static cyg_uint32 profile_period = 0;

# if defined(CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_XPS_TIMER)
# define CYGHWR_HAL_MB_PROFILING_TIMER_BASE   (CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BASE + (0x10 * CYGNUM_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_ID))
# define CYGHWR_HAL_MB_PROFILING_TIMER_TCSR_INIT        \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_UP   |       \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_EN   |       \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_ENI  |       \
              CYGHWR_HAL_MB_XPS_TIMER_TCSR_ARHT
# endif


# if defined(CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_XPS_FIT) || \
         defined(CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_XPS_TIMER)

// Profiling timer ISR
static cyg_uint32
profile_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data, HAL_SavedRegisters *regs)
{

#if defined(CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_XPS_TIMER)
   HAL_WRITE_UINT32(
             CYGHWR_HAL_MB_PROFILING_TIMER_BASE+CYGHWR_HAL_MB_XPS_TIMER_TCSR,
             CYGHWR_HAL_MB_PROFILING_TIMER_TCSR_INIT |
             CYGHWR_HAL_MB_XPS_TIMER_TCSR_TOIN );
#endif

   HAL_INTERRUPT_ACKNOWLEDGE( CYGNUM_HAL_MICROBLAZE_PROFILING_TIMER_IRQNUM );

   // R14 contains the PC value when interrupt occurred
   __profile_hit( regs->d[14] );

   return CYG_ISR_HANDLED;
}

int
hal_enable_profile_timer( int resolution )
{
   // Run periodic timer interrupt for profile
   // The resolution is specified in us

#if defined(CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_XPS_FIT)
   // In case of FIT, the period is decided when synthetizing the IP block so
   // you must ensure the resolution you wish to use match the HW.
   // Nothing to do
#elif defined(CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_BLOCK_XPS_TIMER)
   {
      cyg_uint32 profile_period =
         (CYGPKG_HAL_MICROBLAZE_PROFILING_TIMER_FREQ / 1000000) * resolution;

      HAL_WRITE_UINT32( CYGHWR_HAL_MB_PROFILING_TIMER_BASE+
                                      CYGHWR_HAL_MB_XPS_TIMER_TLR, profile_period );

      HAL_WRITE_UINT32( CYGHWR_HAL_MB_PROFILING_TIMER_BASE+
            CYGHWR_HAL_MB_XPS_TIMER_TCSR, CYGHWR_HAL_MB_XPS_TIMER_TCSR_LOAD );

      HAL_WRITE_UINT32( CYGHWR_HAL_MB_PROFILING_TIMER_BASE+
            CYGHWR_HAL_MB_XPS_TIMER_TCSR, CYGHWR_HAL_MB_PROFILING_TIMER_TCSR_INIT );
   }
#endif

   // Attach ISR.
   HAL_INTERRUPT_ATTACH( CYGNUM_HAL_MICROBLAZE_PROFILING_TIMER_IRQNUM, &profile_isr, 0, 0 );
   HAL_INTERRUPT_UNMASK( CYGNUM_HAL_MICROBLAZE_PROFILING_TIMER_IRQNUM );

   return resolution;
}

# endif

#endif

//==========================================================================
// EOF mb_generic_misc.c
