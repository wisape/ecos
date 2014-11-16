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
// Copyright (C) 2012 Free Software Foundation, Inc.
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
// Date:         2012-07-20
// Purpose:      
// Description:  Variant HAL interrupt for Freescale QorIQ target
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
  volatile cyg_uint32 read;
  volatile cyg_uint32 *msir;

  cyg_uint32 i = 0, j = 0;

  hal_map_IRQ_init();

  // Reset PIC, all pending and in-service interrupts are cleared
  *gpic_info.gcr = CYGARC_REG_IMM_PIC_GCR_RST;

  // Wait until PIC initialization is completed
  while(*gpic_info.gcr & CYGARC_REG_IMM_PIC_GCR_RST);

  // Configure spurious interrupt
  *gpic_info.gcr = CYGNUM_HAL_INTERRUPT_SPURIOUS;

  // Clear register
   for(i = 0; i < CYGPKG_HAL_SMP_CPU_MAX; i++ ) {
    *gpic_info.irq_mp[i].cptr  = 0;
   }

   // Select mixed mode or pass-through
#ifdef CYGSEM_HAL_POWERPC_QORIQ_E500MC_PIC_MIXED_MODE
  *gpic_info.gcr = CYGARC_REG_IMM_PIC_GCR_M;
#endif

   // Initialize IRQ
   hal_IRQ_init_mask();

   // Read pending shared message interrupt
   for(j=0; j < CYGHWR_HAL_QORIQ_PIC_MSI_BANK_NUM; j++ ){
     msir = gpic_info.irq_msi[j].base;
     for(i=0; i < gpic_info.irq_msi[j].num ; i++)
     {
       read = *msir;
       msir += 4;
     }
   }

   *gpic_info.mera = 0x0000000F;
   *gpic_info.merb = 0x0000000F;

}

// Setup the interrupt controller map structure
static  void
hal_map_IRQ_init(void)
{
  int i, j;

  gpic_info.gcr  =   (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_GCR);
  gpic_info.svr  =    (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_SVR);
  gpic_info.mera = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSGRA + \
     CYGARC_REG_IMM_PIC_MER);
  gpic_info.merb = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSGRB + \
     CYGARC_REG_IMM_PIC_MER);
  gpic_info.msra = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSGRA + \
     CYGARC_REG_IMM_PIC_MSR);
  gpic_info.msrb = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSGRB + \
     CYGARC_REG_IMM_PIC_MSR);

  gpic_info.irq_msi[0].base = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSIRA);
  gpic_info.irq_msi[1].base = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSIRB);
  gpic_info.irq_msi[2].base = (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_MSIRC);

  gpic_info.irq_msi[0].num = CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM;
  gpic_info.irq_msi[1].num = CYGHWR_HAL_QORIQ_PIC_MSIB_IRQ_NUM;
  gpic_info.irq_msi[2].num = CYGHWR_HAL_QORIQ_PIC_MSIC_IRQ_NUM;

  // Internal interrupt
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM ; i++)
  {
    gpic_info.irq[i].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_INT_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[i].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_INT_OFFSET_VPR + i * OFFSET_32_BYTES);
    gpic_info.irq[i].ilr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_INT_OFFSET_ILR + i * OFFSET_32_BYTES);
  }
  
  // External interrupt
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM ; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_EXT_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_EXT_OFFSET_VPR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].ilr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_EXT_OFFSET_ILR + i * OFFSET_32_BYTES);
  }

  // Message interrupt [Bank A]
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM ; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSGA_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSGA_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Message interrupt [Bank B]
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM ; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSGB_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSGB_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Shared message signaled interrupt [Bank A]
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM ; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSIA_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSIA_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Shared message signaled interrupt [Bank B]
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_MSIB_IRQ_NUM ; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM +   \
            CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSIB_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSIB_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Shared message signaled interrupt [Bank C]
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_MSIC_IRQ_NUM ; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM +   \
            CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSIB_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSIC_OFFSET_IDR + i * OFFSET_32_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET + \
         CYGARC_REG_IMM_PIC_IRQ_SRC_MSIC_OFFSET_VPR + i * OFFSET_32_BYTES);
  }

  // Timer A interrupt
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_GTA_IRQ_NUM; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM +   \
            CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSIB_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSIC_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_PIC_GTDRA0 + i * OFFSET_64_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_PIC_GTVPRA0 + i * OFFSET_64_BYTES);
  }  

  // Timer B interrupt
  for(i = 0; i < CYGHWR_HAL_QORIQ_PIC_GTB_IRQ_NUM; i++)
  {
    j = i + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM +   \
            CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_MSIB_IRQ_NUM + \
            CYGHWR_HAL_QORIQ_PIC_MSIC_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_GTA_IRQ_NUM;
    gpic_info.irq[j].idr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_PIC_GTDRB0 + i * OFFSET_64_BYTES);
    gpic_info.irq[j].vpr = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
         CYGARC_REG_IMM_PIC_GTVPRB0 + i * OFFSET_64_BYTES);
  }

  // Per CPU extra registers && Inter-processor interrupt
  for(i = 0; i < CYGPKG_HAL_SMP_CPU_MAX; i++ ) {
    for(j = 0; j < CYGHWR_HAL_QORIQ_PIC_IPI_IRQ_NUM; j++)
    {
        gpic_info.irq_ipi[j].idr[i] = (volatile cyg_uint32 *) \
            (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_MP_CPU_OFFSET + \
             CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET * i + \
             CYGARC_REG_IMM_PIC_IPIDR0 + j * OFFSET_16_BYTES);

        gpic_info.irq_ipi[j].vpr = (volatile cyg_uint32 *) \
            (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + \
             CYGARC_REG_IMM_PIC_IPIVPR0 + j * OFFSET_16_BYTES);
    }

    gpic_info.irq_mp[i].eoi = (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_MP_CPU_OFFSET + \
         CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET * i + CYGARC_REG_IMM_PIC_IEO);

    gpic_info.irq_mp[i].whoami= (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_MP_CPU_OFFSET + \
         CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET * i + CYGARC_REG_IMM_PIC_WHOAMI);

   gpic_info.irq_mp[i].iack= (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_MP_CPU_OFFSET + \
         CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET * i + CYGARC_REG_IMM_PIC_IACK);

   gpic_info.irq_mp[i].cptr= (volatile cyg_uint32 *) \
        (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_MP_CPU_OFFSET + \
         CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET * i + CYGARC_REG_IMM_PIC_CTPR);
  }

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
    diag_printf("%p \n", (volatile cyg_uint32 *)  gpic_info.irq[i].idr);
    diag_printf("%p \n", (volatile cyg_uint32 *)  gpic_info.irq[i].vpr);
    if ( i < ( CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM + CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM) )
      diag_printf("%p \n", (volatile cyg_uint32 *)  gpic_info.irq[i].ilr);
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
