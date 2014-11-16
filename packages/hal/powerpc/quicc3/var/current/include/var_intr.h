//==========================================================================
//
//      var_intr.h
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
// Contributors: 
// Date:         2009-11-01
// Purpose:      
// Description:  Variant HAL interrupt for Freescale Quicc III target
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_VAR_INTR_H
#define CYGONCE_VAR_INTR_H

#include <pkgconf/hal.h>
#include <cyg/hal/plf_intr.h>
#include <cyg/hal/quicc3.h>             // Memory map
#include <cyg/infra/cyg_type.h>         // types
#include <cyg/hal/hal_io.h>             // io macros
#include <cyg/infra/cyg_ass.h>          // CYG_FAIL

#include <cyg/infra/diag.h>

#include <cyg/hal/var_regs.h>
#include <cyg/hal/ppc_regs.h>

#ifdef CYGPKG_HAL_SMP_SUPPORT
#include <cyg/hal/hal_smp.h>
#endif

#define CYGNUM_HAL_VECTOR_PERFORMANCE_MON   26

#define CYGNUM_HAL_VSR_MAX                   CYGNUM_HAL_VECTOR_PERFORMANCE_MON

// These are the values used when passed out to an
// external exception handler using cyg_hal_deliver_exception()
#define CYGNUM_HAL_EXCEPTION_PERFORMANCE_MON \
     CYGNUM_HAL_VECTOR_PERFORMANCE_MON

#define CYGNUM_HAL_EXCEPTION_MAX             CYGNUM_HAL_VECTOR_PERFORMANCE_MON

// Defined for the time being until ppc_stub.c is fixed
#define CYGNUM_HAL_NO_VECTOR_TRACE

// ISR
#ifndef CYGHWR_HAL_INTERRUPT_DEFINED

#define CYGNUM_HAL_INTERRUPT_L2_CACHE            0
#define CYGNUM_HAL_INTERRUPT_ECM                 1
#define CYGNUM_HAL_INTERRUPT_DDR_CTRL            2
#define CYGNUM_HAL_INTERRUPT_LBC_CTRL            3
#define CYGNUM_HAL_INTERRUPT_DMA_CH1             4
#define CYGNUM_HAL_INTERRUPT_DMA_CH2             5
#define CYGNUM_HAL_INTERRUPT_DMA_CH3             6
#define CYGNUM_HAL_INTERRUPT_DMA_CH4             7
#define CYGNUM_HAL_INTERRUPT_PCI_CH3             8
#define CYGNUM_HAL_INTERRUPT_PCI_CH2             9
#define CYGNUM_HAL_INTERRUPT_PCI_CH1             10
#define CYGNUM_HAL_INTERRUPT_UNUSED              12
#define CYGNUM_HAL_INTERRUPT_TSEC1_Tx            13
#define CYGNUM_HAL_INTERRUPT_TSEC1_Rx            14
#define CYGNUM_HAL_INTERRUPT_TSEC3_Tx            15
#define CYGNUM_HAL_INTERRUPT_TSEC3_Rx            16
#define CYGNUM_HAL_INTERRUPT_TSEC3_Err           17
#define CYGNUM_HAL_INTERRUPT_TSEC1_Err           18
#define CYGNUM_HAL_INTERRUPT_TSEC2_Tx            19
#define CYGNUM_HAL_INTERRUPT_TSEC2_Rx            20
#define CYGNUM_HAL_INTERRUPT_TSEC4_TX            21
#define CYGNUM_HAL_INTERRUPT_TSEC4_Rx            22
#define CYGNUM_HAL_INTERRUPT_TSEC4_Err           23
#define CYGNUM_HAL_INTERRUPT_TSEC2_Err           24
#define CYGNUM_HAL_INTERRUPT_FEC                 25
#define CYGNUM_HAL_INTERRUPT_UART                26
#define CYGNUM_HAL_INTERRUPT_UART1               26
#define CYGNUM_HAL_INTERRUPT_UART2               26
#define CYGNUM_HAL_INTERRUPT_I2C                 27
#define CYGNUM_HAL_INTERRUPT_MON                 28
#define CYGNUM_HAL_INTERRUPT_SEC1                29
#define CYGNUM_HAL_INTERRUPT_GPIO                31
#define CYGNUM_HAL_INTERRUPT_SRIO_ERR            32
#define CYGNUM_HAL_INTERRUPT_SRIO_OD             33
#define CYGNUM_HAL_INTERRUPT_SRIO_ID             34
#define CYGNUM_HAL_INTERRUPT_SRIO_OMU1           37
#define CYGNUM_HAL_INTERRUPT_SRIO_IMU1           38
#define CYGNUM_HAL_INTERRUPT_SRIO_OMU2           39
#define CYGNUM_HAL_INTERRUPT_SRIO_IMU2           40
#define CYGNUM_HAL_INTERRUPT_PME                 41
#define CYGNUM_HAL_INTERRUPT_SEC2                42
#define CYGNUM_HAL_INTERRUPT_TLU1                45
#define CYGNUM_HAL_INTERRUPT_PME_CH1             48
#define CYGNUM_HAL_INTERRUPT_PME_CH2             49 
#define CYGNUM_HAL_INTERRUPT_PME_CH3             50
#define CYGNUM_HAL_INTERRUPT_PME_CH4             51
#define CYGNUM_HAL_INTERRUPT_IEEE_1588_TSEC1     52
#define CYGNUM_HAL_INTERRUPT_IEEE_1588_TSEC2     53
#define CYGNUM_HAL_INTERRUPT_IEEE_1588_TSEC3     54
#define CYGNUM_HAL_INTERRUPT_IEEE_1588_TSEC4     55
#define CYGNUM_HAL_INTERRUPT_TLU2                59
#define CYGNUM_HAL_INTERRUPT_DMA2_CH1            60
#define CYGNUM_HAL_INTERRUPT_DMA2_CH2            61
#define CYGNUM_HAL_INTERRUPT_DMA2_CH3            62
#define CYGNUM_HAL_INTERRUPT_DMA2_CH4            63

#endif // CYGHWR_HAL_INTERRUPT_DEFINED

// Continue with external interrupt
#define CYGNUM_HAL_INTERRUPT_IRQ0                64
#define CYGNUM_HAL_INTERRUPT_IRQ1                65
#define CYGNUM_HAL_INTERRUPT_IRQ2                66
#define CYGNUM_HAL_INTERRUPT_IRQ3                67
#define CYGNUM_HAL_INTERRUPT_IRQ4                68
#define CYGNUM_HAL_INTERRUPT_IRQ5                69
#define CYGNUM_HAL_INTERRUPT_IRQ6                70
#define CYGNUM_HAL_INTERRUPT_IRQ7                71
#define CYGNUM_HAL_INTERRUPT_IRQ8                72
#define CYGNUM_HAL_INTERRUPT_IRQ9                73
#define CYGNUM_HAL_INTERRUPT_IRQ10               74
#define CYGNUM_HAL_INTERRUPT_IRQ11               75
// Message interrupt
#define CYGNUM_HAL_INTERRUPT_MSG0                76
#define CYGNUM_HAL_INTERRUPT_MSG1                77
#define CYGNUM_HAL_INTERRUPT_MSG2                78
#define CYGNUM_HAL_INTERRUPT_MSG3                79
#define CYGNUM_HAL_INTERRUPT_MSG4                80
#define CYGNUM_HAL_INTERRUPT_MSG5                81
#define CYGNUM_HAL_INTERRUPT_MSG6                82
#define CYGNUM_HAL_INTERRUPT_MSG7                83
// Shared interrupt
#define CYGNUM_HAL_INTERRUPT_SHA0                84
#define CYGNUM_HAL_INTERRUPT_SHA1                85
#define CYGNUM_HAL_INTERRUPT_SHA2                86
#define CYGNUM_HAL_INTERRUPT_SHA3                87
#define CYGNUM_HAL_INTERRUPT_SHA4                88
#define CYGNUM_HAL_INTERRUPT_SHA5                89
#define CYGNUM_HAL_INTERRUPT_SHA6                90
#define CYGNUM_HAL_INTERRUPT_SHA7                91
// Timer group A
#define CYGNUM_HAL_INTERRUPT_TGA0                92
#define CYGNUM_HAL_INTERRUPT_TGA1                93
#define CYGNUM_HAL_INTERRUPT_TGA2                94
#define CYGNUM_HAL_INTERRUPT_TGA3                95
// Timer group B
#define CYGNUM_HAL_INTERRUPT_TGB0                96
#define CYGNUM_HAL_INTERRUPT_TGB1                97
#define CYGNUM_HAL_INTERRUPT_TGB2                98
#define CYGNUM_HAL_INTERRUPT_TGB3                99
// Interprocessor interrupt
#define CYGNUM_HAL_INTERRUPT_IPI0                100
#define CYGNUM_HAL_INTERRUPT_IPI1                101
#define CYGNUM_HAL_INTERRUPT_IPI2                102
#define CYGNUM_HAL_INTERRUPT_IPI3                103

#define CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER     104  // redefined DECREMENTER interrupt since 0 is used for L2 cache external IRQ
#define CYGNUM_HAL_INTERRUPT_WATCHDOG_TIMER      105  // Define a watchdog interrupt number
#define CYGNUM_HAL_INTERRUPT_SPURIOUS            106  // Added to detect spurious interrupt, SVR must be configured accordingly

#define CYGNUM_HAL_INTERRUPT_RTC                 CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER
#define CYGNUM_HAL_ISR_MAX                       CYGNUM_HAL_INTERRUPT_SPURIOUS

#include <cyg/hal/var_type.h>

//-----------------------------------------------------------------------------
// Symbols used by assembly code
#define CYGARC_VARIANT_DEFS                                                             \
    DEFINE(CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER, CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER); \
    DEFINE(CYGNUM_HAL_INTERRUPT_WATCHDOG_TIMER, CYGNUM_HAL_INTERRUPT_WATCHDOG_TIMER);

//--------------------------------------------------------------------------
// Interrupt controller access
extern pic_info gpic_info;

#define HAL_IPIC_VPR( _vector_ ) (*gpic_info.irq[_vector_].vpr)
#define HAL_IPIC_IDR( _vector_ ) (*gpic_info.irq[_vector_].idr)

#define HAL_IPIC_IPI_VPR( _vector_ ) (*gpic_info.irq_ipi[_vector_ - CYGNUM_HAL_INTERRUPT_IPI0].vpr)
#define HAL_IPIC_IPI_IDR( _cpu_, _vector_ ) (*gpic_info.irq_ipi[_vector_ - CYGNUM_HAL_INTERRUPT_IPI0].idr[_cpu_])

#define HAL_IPIC_EOI( _cpu_ ) (*gpic_info.irq_mp[_cpu_].eoi)

#ifndef CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

static __inline__ void
cyg_hal_interrupt_mask ( cyg_uint32 vector )
{
   
  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                  "Invalid vector");

  if(vector <= CYGNUM_HAL_INTERRUPT_TGB3)
  {
    HAL_IPIC_VPR(vector) |= CYGARC_REG_IMM_IPIVPR_MSK;
  }
#ifdef CYGPKG_HAL_SMP_SUPPORT    
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    HAL_IPIC_IPI_VPR(vector) |= CYGARC_REG_IMM_IPIVPR_MSK;
  } 
#endif  
  
}


static __inline__ void
cyg_hal_interrupt_unmask ( cyg_uint32 vector )
{

  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                    "Invalid vector");
  
  if(vector <= CYGNUM_HAL_INTERRUPT_TGB3)
  {
    HAL_IPIC_VPR(vector) &= ~CYGARC_REG_IMM_IPIVPR_MSK;
  }
#ifdef CYGPKG_HAL_SMP_SUPPORT      
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    HAL_IPIC_IPI_VPR(vector) &= ~CYGARC_REG_IMM_IPIVPR_MSK;
  }
#endif  
  
}

#ifdef DEBUG_IRQ
externC char
cyg_hal_interrupt_status( cyg_uint32 vector )
{

  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                     "Invalid vector");

  if(vector <= CYGNUM_HAL_INTERRUPT_TGB3)
  {
    if(HAL_IPIC_VPR(vector) & CYGARC_REG_IMM_IPIVPR_MSK)
      return 1;
  }
#ifdef CYGPKG_HAL_SMP_SUPPORT
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    if(HAL_IPIC_IPI_VPR(vector) & CYGARC_REG_IMM_IPIVPR_MSK)
      return 1;
  }
#endif

  return 0;

}
#endif


#ifdef CYGPKG_HAL_SMP_SUPPORT

static __inline__ void
hal_interrupt_set_cpu( CYG_WORD32 vector, HAL_SMP_CPU_TYPE cpu )
{
  cyg_uint32 cpu_vector = 1;

  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                     "Invalid vector");

  if(vector <= CYGNUM_HAL_INTERRUPT_TGB3)
  {
     // Clear destination CPU field
     HAL_IPIC_IDR(vector) &= ~CYGARC_REG_PIC_DEST_CPU_MASK;
     cpu_vector = cpu_vector << cpu;
     // Set destination CPU field
     HAL_IPIC_IDR(vector) |= cpu_vector;
  }
  
}


static __inline__ void
hal_interrupt_get_cpu( CYG_WORD32 vector, HAL_SMP_CPU_TYPE *cpu )
{
  cyg_uint32 cpu_vector = 0;
  *cpu = 0;
  
  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                     "Invalid vector");

  if(vector <= CYGNUM_HAL_INTERRUPT_TGB3)
  {
    cpu_vector = ((HAL_IPIC_IDR(vector) & CYGARC_REG_PIC_DEST_CPU_MASK)) >> 1;
    // Shift and count bit position
    while(cpu_vector)
    {
      cpu_vector = cpu_vector >> 1;
      *cpu += 1;
    }
  }

}


static __inline__ void
hal_interrupt_ipi_send( CYG_WORD32 vector, HAL_SMP_CPU_TYPE cpu )
{
  cyg_uint32 cpu_vector = 1;
  
  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                     "Invalid vector");
  
  if((vector <= CYGNUM_HAL_INTERRUPT_IPI3) && \
     (vector >= CYGNUM_HAL_INTERRUPT_IPI0))
  {
     cpu_vector = cpu_vector << cpu; 
     // Set destination CPU field
     HAL_IPIC_IPI_IDR(HAL_SMP_CPU_THIS(), vector) = cpu_vector;
  }
   
}
#endif


static __inline__ void
cyg_hal_interrupt_end ( cyg_uint32 vector )
{
#ifdef CYGPKG_HAL_SMP_SUPPORT  
  HAL_SMP_CPU_TYPE me = HAL_SMP_CPU_THIS();
#else
  cyg_uint32 me = 0;
#endif

  // Do not write EOI for decrementer or spurious interrupt
  if(vector < CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER)
  {
    HAL_IPIC_EOI(me) = 0x0;
  }
  
}


// should the reading of EOI register move to vector.s to avoid users messing up the PIC?
static __inline__ void
cyg_hal_interrupt_acknowledge ( cyg_uint32 vector )
{
  cyg_hal_interrupt_end(vector);
}


static __inline__ void
cyg_hal_interrupt_configure ( cyg_uint32 vector,
                              cyg_bool level,
                              cyg_bool up )
{
    cyg_uint32 reg = HAL_IPIC_VPR(vector);

    CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                      "Invalid vector");

    switch (vector) {
    case CYGNUM_HAL_INTERRUPT_IRQ0 ... CYGNUM_HAL_INTERRUPT_IRQ11:
       if (!level) {
         reg &= ~CYGARC_REG_IMM_EXT_IRQ_SENSE_LEVEL;
       }
       else
       {
         reg |= CYGARC_REG_IMM_EXT_IRQ_SENSE_LEVEL;
       }
       if (!up) {
         reg &= ~CYGARC_REG_IMM_EXT_IRQ_HIGH_OR_UP;
       }
       else
       {
         reg |= CYGARC_REG_IMM_EXT_IRQ_HIGH_OR_UP;
       }
       HAL_IPIC_VPR(vector) = reg;
       break;

    default:
       CYG_FAIL("Interrupt # not implemented in cyg_hal_interrupt_configure !!!");
       break;
    }

}


static __inline__ void
cyg_hal_interrupt_set_level ( cyg_uint32 vector, cyg_uint32 level )
{
  volatile cyg_uint32 masking;  
  volatile cyg_uint32 *vpr;
  
  CYG_ASSERT(vector > CYGNUM_HAL_INTERRUPT_SPURIOUS, \
                     "Invalid vector");

  if(vector <= CYGNUM_HAL_INTERRUPT_TGB3)
  {
    vpr = gpic_info.irq[ vector ].vpr;
  }
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
   vpr = gpic_info.irq_ipi[ vector - CYGNUM_HAL_INTERRUPT_IPI0 ].vpr;
  }

  if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    // remember the mask bit
    masking = *vpr & CYGARC_REG_IMM_IPIVPR_MSK;
   
    // mask interrupt
    *vpr |= CYGARC_REG_IMM_IPIVPR_MSK;
    while((*vpr & CYGARC_REG_PIC_VECPRI_MASK) != CYGARC_REG_PIC_VECPRI_MASK);
 
    // change priority
    *vpr |=  (level << 16);

    // set back mask bit
    if(!masking)
      *vpr &= ~CYGARC_REG_IMM_IPIVPR_MSK;
  }

}


#define HAL_INTERRUPT_MASK( _vector_ )                    \
    CYG_MACRO_START                                       \
        cyg_hal_interrupt_mask ( (_vector_) );            \
    CYG_MACRO_END

#define HAL_INTERRUPT_UNMASK( _vector_ )                  \
    CYG_MACRO_START                                       \
        cyg_hal_interrupt_unmask ( (_vector_) );          \
    CYG_MACRO_END

#define HAL_INTERRUPT_ACKNOWLEDGE( _vector_ )             \
    CYG_MACRO_START                                       \
        cyg_hal_interrupt_acknowledge ( (_vector_) );     \
    CYG_MACRO_END

#define HAL_INTERRUPT_CONFIGURE( _vector_, _level_, _up_ )              \
    CYG_MACRO_START                                                     \
        cyg_hal_interrupt_configure ( (_vector_), (_level_), (_up_) );  \
    CYG_MACRO_END

#define HAL_INTERRUPT_SET_LEVEL( _vector_, _level_ )            \
    CYG_MACRO_START                                             \
        cyg_hal_interrupt_set_level ( (_vector_) , (_level_) ); \
    CYG_MACRO_END

#define HAL_INTERRUPT_END( _vector_)              \
    CYG_MACRO_START                               \
        cyg_hal_interrupt_end ( (_vector_) );     \
    CYG_MACRO_END
    
#define CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

#endif


#ifdef CYGPKG_HAL_SMP_SUPPORT
// Additional SMP interrupt configuration support.
__externC HAL_SPINLOCK_TYPE cyg_hal_ioapic_lock;

#define HAL_INTERRUPT_SET_CPU( _vector_, _cpu_ )                \
{                                                               \
    HAL_SPINLOCK_SPIN( cyg_hal_ioapic_lock );                   \
    hal_interrupt_set_cpu( _vector_, _cpu_ );       \
    HAL_SPINLOCK_CLEAR( cyg_hal_ioapic_lock );                  \
}

#define HAL_INTERRUPT_GET_CPU( _vector_, _cpu_ )                \
{                                                               \
    HAL_SMP_CPU_TYPE  *__cpu;                                    \
    HAL_SPINLOCK_SPIN( cyg_hal_ioapic_lock );                   \
    hal_interrupt_get_cpu( _vector_, __cpu );       \
    (_cpu_) = (*__cpu); \
    HAL_SPINLOCK_CLEAR( cyg_hal_ioapic_lock );                  \
}

#define HAL_INTERRUPT_IPI_SEND( _vector_, _cpu_ )                \
{                                                               \
    hal_interrupt_ipi_send( _vector_,  _cpu_ );       \
}


#define CYGNUM_HAL_SMP_CPU_INTERRUPT_VECTOR( _n_ ) ( CYGNUM_HAL_INTERRUPT_IPI0 + ( _n_ ))

#endif

//--------------------------------------------------------------------------
// Clock control

#define CYGHWR_HAL_CLOCK_DEFINED 1
// Note: variant or platform allowed to override these definitions

#ifdef CYGSEM_HAL_POWERPC_QUICC3_RTC_AUTORELOAD

#ifdef CYGPKG_REDBOOT
# define CYGARC_DEC_TCR_BITS ((CYGARC_REG_TCR_ARE) >> 16)
#else
# define CYGARC_DEC_TCR_BITS ((CYGARC_REG_TCR_ARE | CYGARC_REG_TCR_DIE) >> 16)
#endif

// this macro only acknowledge the decrementer interrupt
#define HAL_CLOCK_RESET( _vector_, _period_ )   \
    CYG_MACRO_START                             \
    cyg_uint32 tmp;                             \
    asm volatile (                              \
        "lis   %0,%2;"                          \
        "mtspr %1,%0;"                          \
        : "=r" (tmp)                            \
        : "I" (CYGARC_REG_TSR), "I" (CYGARC_REG_TSR_DIS >> 16) \
        );                                      \
    CYG_MACRO_END

#define HAL_CLOCK_INITIALIZE( _period_ )        \
    CYG_MACRO_START                             \
    cyg_uint32 tmp;                             \
    asm volatile (                              \
        "mtdec %1;"                             \
        "mtspr %2,%1;"                          \
        "mfspr %0,%3;"                          \
        "oris  %0, %0, %4;"                     \
        "mtspr %3,%0;"                          \
        : "=r" (tmp)                            \
        : "r"(_period_), "I" (CYGARC_REG_DECAR), "I" (CYGARC_REG_TCR), "I" (CYGARC_DEC_TCR_BITS) \
        );                                      \
    CYG_MACRO_END 

#else
#define HAL_CLOCK_INITIALIZE( _period_ )        \
    CYG_MACRO_START                             \
    asm volatile (                              \
        "mtdec %0;"                             \
        :                                       \
        : "r"(_period_)                         \
        );                                      \
    CYG_MACRO_END

#define HAL_CLOCK_RESET( _vector_, _period_ )   HAL_CLOCK_INITIALIZE( _period_ )
#endif

#define HAL_CLOCK_START( _period_ )        \
    CYG_MACRO_START                             \
    cyg_uint32 tmp;                             \
    asm volatile (                              \
        "mtdec %1;"                             \
        "mtspr %2,%1;"                          \
        "mfspr %0,%3;"                          \
        "oris  %0, %0, %4;"                     \
        "mtspr %3,%0;"                          \
        : "=r" (tmp)                            \
        : "r"(_period_), "I" (CYGARC_REG_DECAR), "I" (CYGARC_REG_TCR), "I" ((CYGARC_REG_TCR_ARE) >> 16) \
        );                                      \
    CYG_MACRO_END


#define HAL_CLOCK_READ( _pvalue_ )                              \
    CYG_MACRO_START                                             \
    register cyg_uint32 result;                                 \
    asm volatile(                                               \
        "mfdec  %0;"                                            \
        : "=r"(result)                                          \
        );                                                      \
    *(_pvalue_) = CYGNUM_KERNEL_COUNTERS_RTC_PERIOD-result;     \
    CYG_MACRO_END

#ifdef CYGVAR_KERNEL_COUNTERS_CLOCK_LATENCY
#define HAL_CLOCK_LATENCY( _pvalue_ )                           \
    CYG_MACRO_START                                             \
    register cyg_int32 result;                                  \
    asm volatile(                                               \
        "mfdec  %0;"                                            \
        : "=r"(result)                                          \
        );                                                      \
    /* Pending DEC interrupts cannot be discarded. If dec is */ \
    /* positive it''s because a DEC interrupt occured while  */ \
    /* eCos was getting ready to run. Just return 0 in that  */ \
    /* case.                                                 */ \
    if (result > 0)                                             \
        result = 0;                                             \
    *(_pvalue_) = -result;                                      \
    CYG_MACRO_END
#endif

#ifndef HAL_DELAY_US
externC void hal_delay_us(int);
#define HAL_DELAY_US(n) hal_delay_us(n)
#endif

// The vector used by the Real time clock
#ifndef CYGNUM_HAL_INTERRUPT_RTC
#define CYGNUM_HAL_INTERRUPT_RTC             CYGNUM_HAL_INTERRUPT_DECREMENTER
#endif // CYGNUM_HAL_INTERRUPT_RTC

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_VAR_INTR_H
// End of var_intr.h
