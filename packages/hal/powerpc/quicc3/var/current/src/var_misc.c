//==========================================================================
//
//      var_misc.c
//
//      HAL implementation miscellaneous functions
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 
// 2008, 2009, 2010 Free Software Foundation, Inc.
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
// Contributors: 
// Date:         2009-11-01
// Purpose:      
// Description:  Miscellaneous functions for Freescale Quicc III variant
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <cyg/hal/hal_io.h>
#include <cyg/infra/cyg_ass.h>          // CYG_ASSERT
#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <cyg/hal/ppc_regs.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_mem.h>
#include <cyg/hal/quicc3.h>
#include <cyg/hal/var_mem.h>
#include <cyg/hal/plf_mem.h>
#include <cyg/hal/hal_intr.h>

//--------------------------------------------------------------------------
// Helper functions
static cyg_uint32 e500_tlb_size(cyg_uint32 a, cyg_uint32 b)
{
  cyg_uint32 i;
  cyg_uint64 ret=1;

  for(i=0;i<b;i++)
    ret*=a;

  return ret;
}


//--------------------------------------------------------------------------
void hal_variant_init(void)
{

}


//--------------------------------------------------------------------------
// Variant specific idle thread action.
bool
hal_variant_idle_thread_action( cyg_uint32 count )
{

    hal_delay_us( 100 );

    // Let architecture idle thread action run
    return true;
}

//---------------------------------------------------------------------------
// Use MMU resources to map memory regions.
// Takes and returns an int used to ID the MMU resource to use. This ID
// is increased as resources are used and should be used for subsequent
// invocations.
int
cyg_hal_map_memory (int id,CYG_ADDRESS virt, CYG_ADDRESS phys,
                    cyg_int32 size, cyg_uint8 flags)
{
    cyg_uint32 mas0 = 0, mas1 = 0, mas2 = 0, mas3 = 0;
    cyg_uint32 tlb_size;
    cyg_uint32 blk_size = CYGARC_TLB_MIN_KB_SIZE;
    cyg_uint32 max_blk_size;
    cyg_uint32 bl = 0;
    cyg_uint32 ts = 0;

    // Get TLB maximum size
    CYGARC_MFSPR(TLB1CFG, tlb_size);
    tlb_size = (tlb_size >> 16) & 0x0000000F;

    // Compute TLB size in KBytes
    max_blk_size = e500_tlb_size(CYGARC_TLB_MIN_KB_SIZE, tlb_size);

    if (flags & CYGARC_MEMDESC_CI)
        mas2 |= CYGARC_MAS2_I;
    if (flags & CYGARC_MEMDESC_GUARDED)
        mas2 |= CYGARC_MAS2_G;
    if (flags & CYGARC_MEMDESC_WRITE_THROUGH)
        mas2 |= CYGARC_MAS2_W;
    if (flags & CYGARC_MEMDESC_MEMORY_COHERENCE)
        mas2 |= CYGARC_MAS2_M;
    if(flags & CYGARC_MEMDESC_TS)
        ts = CYGARC_MAS1_TS;

    mas3 = CYGARC_MAS3_SX | CYGARC_MAS3_SW | CYGARC_MAS3_SR;

    // This implementation is somehow wrong since it should actually manipulate 64 bits
    // variable to be able to hangle 36 bits interface. Assume it's ok for the time being.

    while (id < CYGARC_TLB1_MAX && size > 0)
    { 
        bl = 1;
        blk_size = CYGARC_TLB_MIN_KB_SIZE;
        while (blk_size < max_blk_size && (blk_size * CYGARC_CYG_MEM_1KB) < size)
        {
            blk_size *= 4;
            bl++;
        }
        mas0 = CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(id);
        mas1 = CYGARC_MAS1_V | CYGARC_MAS1_IPROT | ts | CYGARC_MAS1_TSIZE(bl);
        mas2 |= (CYGARC_MAS2_EPN_MASK & virt);
        mas3 |= (CYGARC_MAS3_RPN_MASK & phys);

        CYGARC_MTSPR (MAS0, mas0);
        CYGARC_MTSPR (MAS1, mas1);
        CYGARC_MTSPR (MAS2, mas2);
        CYGARC_MTSPR (MAS3, mas3);
        CYGARC_MTSPR (MAS6, 0);
        CYGARC_MTSPR (MAS7, 0);

        asm volatile("isync;msync;tlbwe;isync");

        size -= (blk_size * CYGARC_CYG_MEM_1KB);
        virt += (blk_size * CYGARC_CYG_MEM_1KB);
        phys += (blk_size * CYGARC_CYG_MEM_1KB);        
        id++;
    }

    return id;
}


// Invalidate e500 TLBs
static 
void hal_invalidate_tlb(cyg_uint8 tlb)
{
  if (tlb == 0)
    CYGARC_MTSPR(MMUCSR0, 0x4);
  if (tlb == 1)
   CYGARC_MTSPR(MMUCSR0, 0x2);
}


// Disable e500 TLBs
static 
void hal_disable_tlb(cyg_uint8 tlb, cyg_uint8 id)
{
  cyg_uint32 mas0 = 0, mas1 = 0, mas2 = 0, mas3 = 0;

  if(tlb == 1)
  {
    mas0 = CYGARC_MAS0_TLBSEL_TLB1 | CYGARC_MAS0_ESEL(id);
  }
  else
  {
    mas0 = CYGARC_MAS0_TLBSEL_TLB0 | CYGARC_MAS0_ESEL(id);
  }
  mas1 = 0;
  mas2 = 0;
  mas3 = 0;
  
  CYGARC_MTSPR(MAS0, mas0);
  CYGARC_MTSPR(MAS1, mas1);
  CYGARC_MTSPR(MAS2, mas2);
  CYGARC_MTSPR(MAS3, mas3);

  asm volatile("isync; msync; tlbwe; isync");

}


// Initialize MMU. This is the initialization done from vector.s. Do not delete
// the TLB entries that contains the RAM and FLASH entries
externC void
cyg_hal_clear_MMU (void)
{
  int i;
  
  // Invalidate TLB0
  hal_invalidate_tlb(0);

  // Reset TLB0 entries
  for(i=0; i<CYGARC_TLB0_MAX; i++)
    hal_disable_tlb(0, i);
  
  // Reset TLB1 entries
  for(i=0; i<CYGARC_TLB1_APP_MAX; i++)
    hal_disable_tlb(1, i);

}


// L2 cache is enable here
externC void
hal_enable_L2_cache (void)
{
// If no platform specific requirement, the full L2 Cache is enable
#ifndef CYGPLF_ENABLE_L2_CACHE
  volatile l2cache_mm *l2cache = \
        (volatile l2cache_mm *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_L2CACHE);
  volatile cyg_uint32 l2ctl;

  l2cache->l2ctl = 0;
  
  asm volatile("sync; isync;"); 

  // Disable parity
  l2cache->l2errdis = 0x0000001d;
  
  l2cache->l2ctl = \
        CYGARC_REG_L2CTL_E | CYGARC_REG_L2CTL_I;
  
  // Force load operation 
  l2ctl = l2cache->l2ctl;
  
  asm volatile("sync; isync;");
#else
  CYGPLF_ENABLE_L2_CACHE();
#endif
}


// CPU core reset
externC void
cyg_hal_core_reset (int id)
{
  volatile cyg_uint32 *pir =   (volatile cyg_uint32 *) \
      (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIR);

  *pir = (1 << id);
}


// CPU master reset
externC void
cyg_hal_cpu_hwreset (void)
{
#ifdef CYGSEM_HAL_POWERPC_QUICC3_HRESET_REQ_ENABLE
  volatile cyg_uint32 *rstcr =   (volatile cyg_uint32 *) \
     (CYGARC_IMM_BASE + CYGARC_REG_IMM_RSTCR);
  cyg_uint32 istate;
   
  HAL_DISABLE_INTERRUPTS(istate);
  
  *rstcr = CYGARC_REG_IMM_RSTCR_HRESET_REQ;

   hal_delay_us(10000);
   
#else
   // TODO
  diag_printf("CPU reset not implemented for QUICC3");
#endif
}


#ifdef CYGSEM_HAL_QUICC3_WATCHDOG_ENABLE

// Reset watchdog
void
cyg_hal_reset_watchdog(void)
{
  cyg_uint32 tsr_reg;

  tsr_reg = CYGARC_REG_TSR_WIS | CYGARC_REG_TSR_ENW;
  asm volatile ( \
          "mtspr %1, %0;" \
          : \
          : "r" (tsr_reg), "I" (CYGARC_REG_TSR));

}

// Enable watchdog
void
cyg_hal_enable_watchdog(cyg_uint16 period)
{
  cyg_uint32 tcr_reg;
  cyg_uint32 istate;

  HAL_DISABLE_INTERRUPTS( istate );
  asm volatile ( \
          "mfspr %0, %1;" \
          : "=r" (tcr_reg) \
          : "I" (CYGARC_REG_TCR));
  // Enable watchdog, fix behavior : restart the board
  // FIXME, make timer value configurable
  tcr_reg |= ( 0x20100000 | CYGARC_REG_TCR_WIE );
  asm volatile ( \
          "mtspr %1, %0;" \
          : \
          : "r" (tcr_reg), "I" (CYGARC_REG_TCR));
  HAL_RESTORE_INTERRUPTS( istate );
}

// Disable watchdog
void
cyg_hal_disable_watchdog(void)
{
  cyg_uint32 tcr_reg;
  cyg_uint32 istate;

  HAL_DISABLE_INTERRUPTS( istate );
  asm volatile ( \
          "mfspr %0, %1;" \
          : "=r" (tcr_reg) \
          : "I" (CYGARC_REG_TCR));
  tcr_reg &= ~CYGARC_REG_TCR_WIE;
  asm volatile ( \
          "mtspr %1, %0;" \
          : \
          : "r" (tcr_reg), "I" (CYGARC_REG_TCR));
  HAL_RESTORE_INTERRUPTS( istate );
}

#endif

//--------------------------------------------------------------------------
// End of var_misc.c
