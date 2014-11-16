//=============================================================================
//
//      plf_misc.c
//
//      HAL implementation miscellaneous functions; per platform
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2012 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   ccoutand
// Contributors:
// Date:        
// Purpose:     Miscellaneous functions for the COMX-P4080 platform from Emerson
// Description: 
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <cyg/hal/hal_mem.h>            // HAL memory definitions
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/qoriq.h>              // For IMM structures
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_intr.h>

#ifdef CYGPKG_HAL_SMP_SUPPORT
# include <cyg/hal/hal_smp.h>
#endif

//
// Cause the platform to reset
//
externC void cyg_hal_cpu_hwreset(void);
void
comx_p4080_reset(void)
{
  cyg_hal_cpu_hwreset();
}


//
// Timer support
//

/* ----------------------          GTA0         ----------------------------*/

void hal_clock_initialize_gta0(cyg_uint32 period)
{
  CYG_ASSERT(period < 0x80000000, "Invalid clock period");

  // Disable counter
  HAL_WRITE_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC+CYGARC_REG_IMM_PIC_GTBCRA0, CYGARC_REG_IMM_PIC_GTCCR_CI);

  // Load the period and enable interrupt
  HAL_WRITE_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC+CYGARC_REG_IMM_PIC_GTBCRA0, period);

}


void hal_clock_reset_gta0(cyg_uint32 vector, cyg_uint32 period)
{
  hal_clock_initialize_gta0(period);
}


void hal_clock_read_gta0(cyg_uint32 *pvalue)
{
  CYG_ADDRESS timer = CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC;
  cyg_uint32 val;

  HAL_READ_UINT32(timer+CYGARC_REG_IMM_PIC_GTCCRA0, val);
  *pvalue = val;
}


/* ----------------------          GTB0         ----------------------------*/

void hal_clock_initialize_gtb0(cyg_uint32 period)
{
  CYG_ASSERT(period < 0x80000000, "Invalid clock period");

  // Disable counter
  HAL_WRITE_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC+CYGARC_REG_IMM_PIC_GTBCRB0, CYGARC_REG_IMM_PIC_GTCCR_CI);

  // Load the period and enable interrupt
  HAL_WRITE_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PIC+CYGARC_REG_IMM_PIC_GTBCRB0, period);

}


void hal_clock_reset_gtb0(cyg_uint32 vector, cyg_uint32 period)
{
  hal_clock_initialize_gtb0(period);
}


void hal_clock_read_gtb0(cyg_uint32 *pvalue)
{
  CYG_ADDRESS timer = CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC;
  cyg_uint32 val;

  HAL_READ_UINT32(timer+CYGARC_REG_IMM_PIC_GTCCRB0, val);
  *pvalue = val;
}


#ifdef CYGPKG_PROFILE_GPROF
//--------------------------------------------------------------------------
//
// Profiling support - uses a separate high-speed timer
//

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/profile/profile.h>

# ifdef CYGSEM_PROFILE_PER_CORE
static HAL_SPINLOCK_TYPE    profile_lock = HAL_SPINLOCK_INIT_CLEAR;
# endif

// Can't rely on Cyg_Interrupt class being defined.
#define Cyg_InterruptHANDLED 1

static int  profile_period  = 0;

// Profiling timer ISR
static cyg_uint32
profile_isr_core0(CYG_ADDRWORD vector, CYG_ADDRWORD data, HAL_SavedRegisters *regs)
{
# ifdef CYGSEM_PROFILE_PER_CORE
  cyg_uint32 istate;
# endif

  hal_clock_reset_gta0(CYGNUM_HAL_INTERRUPT_TGA0, profile_period);
  HAL_INTERRUPT_ACKNOWLEDGE(CYGNUM_HAL_INTERRUPT_TGA0);

# ifdef CYGSEM_PROFILE_PER_CORE
  HAL_DISABLE_INTERRUPTS( istate );
  HAL_SPINLOCK_SPIN( profile_lock );
# endif

  __profile_hit(regs->pc);

# ifdef CYGSEM_PROFILE_PER_CORE
  HAL_SPINLOCK_CLEAR( profile_lock );
  HAL_RESTORE_INTERRUPTS( istate  );
# endif

  return Cyg_InterruptHANDLED;
}

# ifdef CYGSEM_PROFILE_PER_CORE
// Profiling timer ISR
static cyg_uint32
profile_isr_core1(CYG_ADDRWORD vector, CYG_ADDRWORD data, HAL_SavedRegisters *regs)
{
  cyg_uint32 istate;

  hal_clock_reset_gtb0(CYGNUM_HAL_INTERRUPT_TGB0, profile_period);
  HAL_INTERRUPT_ACKNOWLEDGE(CYGNUM_HAL_INTERRUPT_TGB0);

  HAL_DISABLE_INTERRUPTS( istate );
  HAL_SPINLOCK_SPIN( profile_lock );

  __profile_hit(regs->pc);

  HAL_SPINLOCK_CLEAR( profile_lock );
  HAL_RESTORE_INTERRUPTS( istate  );

  return Cyg_InterruptHANDLED;
}
# endif

int
hal_enable_profile_timer(int resolution)
{
  // Run periodic timer interrupt for profile
  // The resolution is specified in us

  // calculate # of ticks for the resolution which is given in us.
  profile_period = (CYGHWR_HAL_POWERPC_CPU_SPEED * resolution) / 8000000;

  // Attach ISR.
  HAL_INTERRUPT_ATTACH(CYGNUM_HAL_INTERRUPT_TGA0, &profile_isr_core0, 0x1111, 0);
  HAL_INTERRUPT_UNMASK(CYGNUM_HAL_INTERRUPT_TGA0);

# ifdef CYGSEM_PROFILE_PER_CORE
  // Attach ISR for core1
  HAL_INTERRUPT_ATTACH(CYGNUM_HAL_INTERRUPT_TGB0, &profile_isr_core1, 0x1111, 0);
  HAL_INTERRUPT_UNMASK(CYGNUM_HAL_INTERRUPT_TGB0);
  HAL_INTERRUPT_SET_CPU(CYGNUM_HAL_INTERRUPT_TGB0, 1);

  // Set period and enable timer interrupts for core 1
  hal_clock_initialize_gtb0(profile_period);
#endif

  // Set period and enable timer interrupts for core 0
  hal_clock_initialize_gta0(profile_period);

  return resolution;
}
#endif


/**************************** Optional HAL version ***************************/
#ifdef CYGHWR_HAL_VERSION
#define QUOTE_T(x) #x
#define XQUOTE_T(x) QUOTE_T(x)
static char version_string[] =
    XQUOTE_T(CYGHWR_HAL_VERSION) " ("__DATE__" - "__TIME__ ")" ;

extern char * get_hal_version()
{
  return &version_string[0];
}
#endif

// EOF plf_misc.c
