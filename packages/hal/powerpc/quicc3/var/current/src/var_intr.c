//==========================================================================
//
//      var_intr.c
//
//      Variant HAL interrupt and clock support
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
// Contributors: Edgar Grimberg
// Date:         2009-11-01
// Purpose:      
// Description:  Variant HAL interrupt for Freescale Quicc III target
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/var_regs.h>
#include <cyg/hal/var_intr.h>
#include <cyg/hal/var_type.h>

static  void hal_map_IRQ_init(void);
static  void hal_IRQ_init_mask(void);

pic_info gpic_info;

extern  void
hal_variant_IRQ_init(void)
{
  volatile cyg_uint32 *gcr =   (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_GCR);
  volatile cyg_uint32 *svr=    (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_SVR);
  volatile cyg_uint32 *mer0= (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_MER0);
  volatile cyg_uint32 *mer1= (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_MER1);
  volatile cyg_uint32 *msir= (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_MSIR0);
  volatile cyg_uint32 *cptr0= (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + CYGARC_REG_IMM_CTPR0);
  volatile cyg_uint32 *cptr1= (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + CYGARC_REG_IMM_CTPR1);
 
  volatile cyg_uint32 read = 0;
  cyg_uint32 i = 0;

  hal_map_IRQ_init();

  // Reset PIC, all pending and in-service interrupts are cleared
  *gcr = CYGARC_REG_IMM_GCR_RST;

  // Wait until PIC initialization is completed
  while(*gcr & CYGARC_REG_IMM_GCR_RST);

  // Configure spurious interrupt. At the moment this is a protection because
  // the default_interrupt handler has no sanity check on interrupt vector
  // number and 0xFFFF as default spurious interrupt number is way out of
  // questions
  *svr = CYGNUM_HAL_INTERRUPT_SPURIOUS;

  // Clear register
  *cptr0  = 0;
  *cptr1  = 0;
  
   // Select mixed mode or pass-through
#ifdef CYGSEM_HAL_QUICC3_PIC_MIXED_MODE
  *gcr = CYGARC_REG_IMM_GCR_M;
#endif

   // Initialize IRQ
   hal_IRQ_init_mask();
  
   // Read pending shared message interrupt
   for(i=0; i < CYGARC_REG_PIC_SHA_IRQ_NUM ; i++)
   {
     read = *msir;
     msir += 4;
   }
 
   *mer0 = 0x0000000F;
   *mer1 = 0x0000000F;

}

// Setup the interrupt controller map structure
static  void
hal_map_IRQ_init(void)
{
  int i, j;
  
  // Internal interrupt
  for(i = 0; i < CYGARC_REG_PIC_INT_IRQ_NUM ; i++)
  {
    gpic_info.irq[i].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_INT_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[i].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_INT_OFFSET_VPR + i * OFFSET_32_BYTES);
  }
  
  // External interrupt
  for(i = 0; i < CYGARC_REG_PIC_EXT_IRQ_NUM ; i++)
  {
    j = i + CYGARC_REG_PIC_INT_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_EXT_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_EXT_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Message interrupt
  for(i = 0; i < CYGARC_REG_PIC_MSG_IRQ_NUM ; i++)
  {
    j = i + CYGARC_REG_PIC_INT_IRQ_NUM + CYGARC_REG_PIC_EXT_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_MSG_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_MSG_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Shared interrupt
  for(i = 0; i < CYGARC_REG_PIC_SHA_IRQ_NUM ; i++)
  {
    j = i + CYGARC_REG_PIC_INT_IRQ_NUM + CYGARC_REG_PIC_EXT_IRQ_NUM +\
         CYGARC_REG_PIC_MSG_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_SHA_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_IRQ_SRC_SHA_OFFSET_VPR + i * OFFSET_32_BYTES);
  }
  
  // Timer A interrupt
  for(i = 0; i < CYGARC_REG_PIC_GTA_IRQ_NUM; i++)
  {
    j = i + CYGARC_REG_PIC_INT_IRQ_NUM + CYGARC_REG_PIC_EXT_IRQ_NUM +\
         CYGARC_REG_PIC_MSG_IRQ_NUM + CYGARC_REG_PIC_SHA_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_GTDRA0 + i * OFFSET_64_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_GTVPRA0 + i * OFFSET_64_BYTES);
  }  

  // Timer B interrupt
  for(i = 0; i < CYGARC_REG_PIC_GTB_IRQ_NUM; i++)
  {
    j = i + CYGARC_REG_PIC_INT_IRQ_NUM + CYGARC_REG_PIC_EXT_IRQ_NUM +\
        CYGARC_REG_PIC_MSG_IRQ_NUM + CYGARC_REG_PIC_SHA_IRQ_NUM +\
        CYGARC_REG_PIC_GTA_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_GTDRB0 + i * OFFSET_64_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_GTVPRB0 + i * OFFSET_64_BYTES);
  }    

  // Inter-processor interrupt
  for(i = 0; i < CYGARC_REG_PIC_IPI_IRQ_NUM; i++)
  {
    gpic_info.irq_ipi[i].idr[0] = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_IPIDR0_CPU0 + i * OFFSET_16_BYTES);
#if CYGPKG_HAL_SMP_CPU_MAX > 1
    gpic_info.irq_ipi[i].idr[1] = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_IPIDR0_CPU1 + i * OFFSET_16_BYTES);
#endif
    gpic_info.irq_ipi[i].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_IPIVPR0 + i * OFFSET_16_BYTES);
  }    

  // Per CPU extra registers
  gpic_info.irq_mp[0].eoi = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_IEO_CPU0);

#if CYGPKG_HAL_SMP_CPU_MAX > 1
  gpic_info.irq_mp[1].eoi = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_IEO_CPU1);
#endif

  gpic_info.irq_mp[0].whoami= (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_WHOAMI_CPU0);

#if CYGPKG_HAL_SMP_CPU_MAX > 1
  gpic_info.irq_mp[1].whoami = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_WHOAMI_CPU1);
#endif

  gpic_info.irq_mp[0].iack= (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_IACK_CPU0);

#if CYGPKG_HAL_SMP_CPU_MAX > 1
  gpic_info.irq_mp[1].iack = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_PER_CPU_OFFSET + \
         CYGARC_REG_IMM_IACK_CPU1);
#endif
  
}


// Initialize the interrupts
static  void
hal_IRQ_init_mask(void)
{

  volatile cyg_uint32 *idr;
  volatile cyg_uint32 *vpr;
  cyg_uint32 i = 0;

  for(i = 0; i <= CYGNUM_HAL_INTERRUPT_IPI3; i++)
  {
    if(i < CYGNUM_HAL_INTERRUPT_IPI0)
    {
      idr = (volatile cyg_uint32 *)  gpic_info.irq[i].idr;
      vpr = (volatile cyg_uint32 *)  gpic_info.irq[i].vpr;
      // By default interrupts are delivered to core0
      *idr = CYGARC_REG_PIC_DEST_CPU0;
    }
    else   
    {
      vpr = (volatile cyg_uint32 *)  gpic_info.irq_ipi[i - CYGNUM_HAL_INTERRUPT_IPI0].vpr;  
    }
    // Mask interrupt
    *vpr |= CYGARC_REG_PIC_VECPRI_MASK;
    while((*vpr & CYGARC_REG_PIC_VECPRI_MASK) != CYGARC_REG_PIC_VECPRI_MASK);
    // Set default interrupt priority
    *vpr |=  (i | (CYGARC_REG_PIC_PRI_DEFAULT << 16));
  }

}


// Debug function
extern void
hal_IRQ_show_reg(void) {
  int i;
  
  for(i = 0; i < CYGNUM_HAL_INTERRUPT_IPI0; i++)
  {
    diag_printf("0X%p \n", (volatile cyg_uint32 *)  gpic_info.irq[i].idr);
    diag_printf("0X%p \n", (volatile cyg_uint32 *)  gpic_info.irq[i].vpr);
  }

}

#ifdef CYGVAR_HAL_DEFINED_HAL_DELAY_US

externC void
hal_delay_us(int us)
{
  cyg_uint32 val_u, val_l;
  volatile cyg_uint64 old_dec, new_dec;
  long long ticks;
  long long diff;
  unsigned long ticks_per_us;

  // Scale the desired number of microseconds to be a number of decrementer ticks
  ticks_per_us = ((unsigned long)1 * (CYGNUM_HAL_RTC_PERIOD )) / 10000;

  ticks = us * ticks_per_us;

  asm volatile("mftbl %0;" : "=r"(val_l) : );
  asm volatile("mftbu %0;" : "=r"(val_u) : );
  old_dec = (((cyg_uint64)val_u) << 32) | (cyg_uint64)(val_l);

  do
  {
    asm volatile("mftbl %0;" : "=r"(val_l) : );
    asm volatile("mftbu %0;" : "=r"(val_u) : );
    new_dec = (((cyg_uint64)val_u) << 32) | (cyg_uint64)(val_l);

    diff = (new_dec - old_dec);
  }while(diff < ticks);
}

#endif

// -------------------------------------------------------------------------
// EOF var_intr.c
