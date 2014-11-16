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

#ifndef CYGONCE_VAR_INTR_H
#define CYGONCE_VAR_INTR_H

#include <pkgconf/hal.h>
#include <cyg/hal/plf_intr.h>
#include <cyg/hal/qoriq.h>             // Memory map
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
#define CYGNUM_HAL_INTERRUPT_ERROR               0
#define CYGNUM_HAL_INTERRUPT_WRS                 1
#define CYGNUM_HAL_INTERRUPT_UNUSED_2            2
#define CYGNUM_HAL_INTERRUPT_UNUSED_3            3
#define CYGNUM_HAL_INTERRUPT_UNUSED_4            4
#define CYGNUM_HAL_INTERRUPT_UNUSED_5            5
#define CYGNUM_HAL_INTERRUPT_UNUSED_6            6
#define CYGNUM_HAL_INTERRUPT_UNUSED_7            7
#define CYGNUM_HAL_INTERRUPT_PAMU_VIOL           8
#define CYGNUM_HAL_INTERRUPT_PAMU_ELBC           9
#define CYGNUM_HAL_INTERRUPT_UNUSED_10           10
#define CYGNUM_HAL_INTERRUPT_UNUSED_11           11
#define CYGNUM_HAL_INTERRUPT_DMA1_CH1            12
#define CYGNUM_HAL_INTERRUPT_DMA1_CH2            13
#define CYGNUM_HAL_INTERRUPT_DMA1_CH3            14
#define CYGNUM_HAL_INTERRUPT_DMA1_CH4            15
#define CYGNUM_HAL_INTERRUPT_DMA2_CH1            16
#define CYGNUM_HAL_INTERRUPT_DMA2_CH2            17
#define CYGNUM_HAL_INTERRUPT_DMA2_CH3            18
#define CYGNUM_HAL_INTERRUPT_DMA2_CH4            19
#define CYGNUM_HAL_INTERRUPT_UART1               20
#define CYGNUM_HAL_INTERRUPT_UART2               21
#define CYGNUM_HAL_INTERRUPT_I2C1                22
#define CYGNUM_HAL_INTERRUPT_I2C2                23
#define CYGNUM_HAL_INTERRUPT_PCI1_INTA           24
#define CYGNUM_HAL_INTERRUPT_PCI2_INTA           25
#define CYGNUM_HAL_INTERRUPT_PCI3_INTA           26
#define CYGNUM_HAL_INTERRUPT_UNUSED_27           27
#define CYGNUM_HAL_INTERRUPT_USB1                28
#define CYGNUM_HAL_INTERRUPT_USB2                29
#define CYGNUM_HAL_INTERRUPT_UNUSED_30           30
#define CYGNUM_HAL_INTERRUPT_UNUSED_31           31
#define CYGNUM_HAL_INTERRUPT_ESHDC               32
#define CYGNUM_HAL_INTERRUPT_UNUSED_33           33
#define CYGNUM_HAL_INTERRUPT_UNUSED_34           34
#define CYGNUM_HAL_INTERRUPT_UNUSED_35           35
#define CYGNUM_HAL_INTERRUPT_MON                 36
#define CYGNUM_HAL_INTERRUPT_ESPI                37
#define CYGNUM_HAL_INTERRUPT_UNUSED_38           38
#define CYGNUM_HAL_INTERRUPT_GPIO                39
#define CYGNUM_HAL_INTERRUPT_SRIO_OD             40
#define CYGNUM_HAL_INTERRUPT_SRIO_ID             41
#define CYGNUM_HAL_INTERRUPT_UNUSED_42           42
#define CYGNUM_HAL_INTERRUPT_UNUSED_43           43
#define CYGNUM_HAL_INTERRUPT_SRIO_OMU1           44
#define CYGNUM_HAL_INTERRUPT_SRIO_IMU1           45
#define CYGNUM_HAL_INTERRUPT_SRIO_OMU2           46
#define CYGNUM_HAL_INTERRUPT_SRIO_IMU2           47
#define CYGNUM_HAL_INTERRUPT_UNUSED_48           48
#define CYGNUM_HAL_INTERRUPT_UNUSED_49           49
#define CYGNUM_HAL_INTERRUPT_UNUSED_50           50
#define CYGNUM_HAL_INTERRUPT_UNUSED_51           51
#define CYGNUM_HAL_INTERRUPT_UNUSED_52           52
#define CYGNUM_HAL_INTERRUPT_UNUSED_53           53
#define CYGNUM_HAL_INTERRUPT_UNUSED_54           54
#define CYGNUM_HAL_INTERRUPT_UNUSED_55           55
#define CYGNUM_HAL_INTERRUPT_UNUSED_56           56
#define CYGNUM_HAL_INTERRUPT_UNUSED_57           57
#define CYGNUM_HAL_INTERRUPT_UNUSED_58           58
#define CYGNUM_HAL_INTERRUPT_UNUSED_59           59
#define CYGNUM_HAL_INTERRUPT_UNUSED_60           60
#define CYGNUM_HAL_INTERRUPT_UNUSED_61           61
#define CYGNUM_HAL_INTERRUPT_UNUSED_62           62
#define CYGNUM_HAL_INTERRUPT_UNUSED_63           63
#define CYGNUM_HAL_INTERRUPT_UNUSED_64           64
#define CYGNUM_HAL_INTERRUPT_UNUSED_65           65
#define CYGNUM_HAL_INTERRUPT_UNUSED_66           66
#define CYGNUM_HAL_INTERRUPT_UNUSED_67           67
#define CYGNUM_HAL_INTERRUPT_EPU1                68
#define CYGNUM_HAL_INTERRUPT_EPU2                69
#define CYGNUM_HAL_INTERRUPT_UNUSED_70           70
#define CYGNUM_HAL_INTERRUPT_UNUSED_71           71
#define CYGNUM_HAL_INTERRUPT_SEC_Q0              72
#define CYGNUM_HAL_INTERRUPT_SEC_Q1              73
#define CYGNUM_HAL_INTERRUPT_SEC_Q2              74
#define CYGNUM_HAL_INTERRUPT_SEC_Q3              75
#define CYGNUM_HAL_INTERRUPT_SEC_ERR             76
#define CYGNUM_HAL_INTERRUPT_SYS_MON             77
#define CYGNUM_HAL_INTERRUPT_UNUSED_78           78
#define CYGNUM_HAL_INTERRUPT_UNUSED_79           79
#define CYGNUM_HAL_INTERRUPT_FMAN1               80
#define CYGNUM_HAL_INTERRUPT_FMAN2               81
#define CYGNUM_HAL_INTERRUPT_UNUSED_82           82
#define CYGNUM_HAL_INTERRUPT_UNUSED_83           83
#define CYGNUM_HAL_INTERRUPT_MDIO1               84
#define CYGNUM_HAL_INTERRUPT_MDIO2               85
#define CYGNUM_HAL_INTERRUPT_UNUSED_86           86
#define CYGNUM_HAL_INTERRUPT_UNUSED_87           87
#define CYGNUM_HAL_INTERRUPT_QMAN0               88
#define CYGNUM_HAL_INTERRUPT_BMAN0               89
#define CYGNUM_HAL_INTERRUPT_QMAN1               90
#define CYGNUM_HAL_INTERRUPT_BMAN1               91
#define CYGNUM_HAL_INTERRUPT_QMAN2               92
#define CYGNUM_HAL_INTERRUPT_BMAN2               93
#define CYGNUM_HAL_INTERRUPT_QMAN3               94
#define CYGNUM_HAL_INTERRUPT_BMAN3               95
#define CYGNUM_HAL_INTERRUPT_QMAN4               96
#define CYGNUM_HAL_INTERRUPT_BMAN4               97
#define CYGNUM_HAL_INTERRUPT_QMAN5               98
#define CYGNUM_HAL_INTERRUPT_BMAN5               99
#define CYGNUM_HAL_INTERRUPT_QMAN6               100
#define CYGNUM_HAL_INTERRUPT_BMAN6               101
#define CYGNUM_HAL_INTERRUPT_QMAN7               102
#define CYGNUM_HAL_INTERRUPT_BMAN7               103
#define CYGNUM_HAL_INTERRUPT_QMAN8               104
#define CYGNUM_HAL_INTERRUPT_BMAN8               105
#define CYGNUM_HAL_INTERRUPT_QMAN9               106
#define CYGNUM_HAL_INTERRUPT_BMAN9               107
#define CYGNUM_HAL_INTERRUPT_UNUSED_108          108
#define CYGNUM_HAL_INTERRUPT_UNUSED_109          109
#define CYGNUM_HAL_INTERRUPT_UNUSED_110          110
#define CYGNUM_HAL_INTERRUPT_UNUSED_111          111

// Continue with external interrupt
#define CYGNUM_HAL_INTERRUPT_IRQ0                112
#define CYGNUM_HAL_INTERRUPT_IRQ1                113
#define CYGNUM_HAL_INTERRUPT_IRQ2                114
#define CYGNUM_HAL_INTERRUPT_IRQ3                115
#define CYGNUM_HAL_INTERRUPT_IRQ4                116
#define CYGNUM_HAL_INTERRUPT_IRQ5                117
#define CYGNUM_HAL_INTERRUPT_IRQ6                118
#define CYGNUM_HAL_INTERRUPT_IRQ7                119
#define CYGNUM_HAL_INTERRUPT_IRQ8                120
#define CYGNUM_HAL_INTERRUPT_IRQ9                121
#define CYGNUM_HAL_INTERRUPT_IRQ10               122
#define CYGNUM_HAL_INTERRUPT_IRQ11               123
// Message interrupt
#define CYGNUM_HAL_INTERRUPT_MSGA0               124
#define CYGNUM_HAL_INTERRUPT_MSGA1               125
#define CYGNUM_HAL_INTERRUPT_MSGA2               126
#define CYGNUM_HAL_INTERRUPT_MSGA3               127
#define CYGNUM_HAL_INTERRUPT_MSGB0               128
#define CYGNUM_HAL_INTERRUPT_MSGB1               129
#define CYGNUM_HAL_INTERRUPT_MSGB2               130
#define CYGNUM_HAL_INTERRUPT_MSGB3               131
// Shared Message Signaled Interrupts
#define CYGNUM_HAL_INTERRUPT_MSIA0               132
#define CYGNUM_HAL_INTERRUPT_MSIA1               133
#define CYGNUM_HAL_INTERRUPT_MSIA2               134
#define CYGNUM_HAL_INTERRUPT_MSIA3               135
#define CYGNUM_HAL_INTERRUPT_MSIA4               136
#define CYGNUM_HAL_INTERRUPT_MSIA5               137
#define CYGNUM_HAL_INTERRUPT_MSIA6               138
#define CYGNUM_HAL_INTERRUPT_MSIA7               139
#define CYGNUM_HAL_INTERRUPT_MSIB0               140
#define CYGNUM_HAL_INTERRUPT_MSIB1               141
#define CYGNUM_HAL_INTERRUPT_MSIB2               142
#define CYGNUM_HAL_INTERRUPT_MSIB3               143
#define CYGNUM_HAL_INTERRUPT_MSIB4               144
#define CYGNUM_HAL_INTERRUPT_MSIB5               145
#define CYGNUM_HAL_INTERRUPT_MSIB6               146
#define CYGNUM_HAL_INTERRUPT_MSIB7               147
#define CYGNUM_HAL_INTERRUPT_MSIC0               148
#define CYGNUM_HAL_INTERRUPT_MSIC1               149
#define CYGNUM_HAL_INTERRUPT_MSIC2               150
#define CYGNUM_HAL_INTERRUPT_MSIC3               151
#define CYGNUM_HAL_INTERRUPT_MSIC4               152
#define CYGNUM_HAL_INTERRUPT_MSIC5               153
#define CYGNUM_HAL_INTERRUPT_MSIC6               154
#define CYGNUM_HAL_INTERRUPT_MSIC7               155
// Timer group A
#define CYGNUM_HAL_INTERRUPT_TGA0                156
#define CYGNUM_HAL_INTERRUPT_TGA1                157
#define CYGNUM_HAL_INTERRUPT_TGA2                158
#define CYGNUM_HAL_INTERRUPT_TGA3                159
// Timer group B
#define CYGNUM_HAL_INTERRUPT_TGB0                160
#define CYGNUM_HAL_INTERRUPT_TGB1                161
#define CYGNUM_HAL_INTERRUPT_TGB2                162
#define CYGNUM_HAL_INTERRUPT_TGB3                163
// Interprocessor interrupt
#define CYGNUM_HAL_INTERRUPT_IPI0                164
#define CYGNUM_HAL_INTERRUPT_IPI1                165
#define CYGNUM_HAL_INTERRUPT_IPI2                166
#define CYGNUM_HAL_INTERRUPT_IPI3                167

#define CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER     168  // redefined DECREMENTER interrupt since 0 is used for L2 cache external IRQ
#define CYGNUM_HAL_INTERRUPT_WATCHDOG_TIMER      169  // Define a watchdog interrupt number
#define CYGNUM_HAL_INTERRUPT_SPURIOUS            170  // Added to detect spurious interrupt, SVR must be configured accordingly

#define CYGNUM_HAL_INTERRUPT_RTC                 CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER
#define CYGNUM_HAL_ISR_MAX                       CYGNUM_HAL_INTERRUPT_SPURIOUS

// Error interrupt code
#define CYGNUM_HAL_INTERRUPT_ERR_FMAN2           0
#define CYGNUM_HAL_INTERRUPT_ERR_FMAN1           1
#define CYGNUM_HAL_INTERRUPT_ERR_BMAN            2
#define CYGNUM_HAL_INTERRUPT_ERR_QMAN            3
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_4        4
#define CYGNUM_HAL_INTERRUPT_ERR_PME             5
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_6        6
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_7        7
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_8        8
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_9        9
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_10      10
#define CYGNUM_HAL_INTERRUPT_ERR_SRIO_RMU       11
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_12      12
#define CYGNUM_HAL_INTERRUPT_ERR_PCI1           13
#define CYGNUM_HAL_INTERRUPT_ERR_PCI2           14
#define CYGNUM_HAL_INTERRUPT_ERR_PCI3           15
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_16      16
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_17      17
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_18      18
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_19      19
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_20      20
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_21      21
#define CYGNUM_HAL_INTERRUPT_ERR_MEM_CTR2       22
#define CYGNUM_HAL_INTERRUPT_ERR_MEM_CTR1       23
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_24      24
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_25      25
#define CYGNUM_HAL_INTERRUPT_ERR_CPC2           26
#define CYGNUM_HAL_INTERRUPT_ERR_CPC1           27
#define CYGNUM_HAL_INTERRUPT_ERR_UNUSED_28      28
#define CYGNUM_HAL_INTERRUPT_ERR_RAM_ECC        29
#define CYGNUM_HAL_INTERRUPT_ERR_PAMU_HW        30
#define CYGNUM_HAL_INTERRUPT_ERR_CCF            31

#include <cyg/hal/var_type.h>

//-----------------------------------------------------------------------------
// Symbols used by assembly code
#define CYGARC_VARIANT_DEFS                                     \
    DEFINE(CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER, CYGNUM_HAL_INTERRUPT_MAP_DECREMENTER);

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
    HAL_IPIC_VPR(vector) |= CYGARC_REG_IMM_PIC_IPIVPR_MSK;
  }
#ifdef CYGPKG_HAL_SMP_SUPPORT    
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    HAL_IPIC_IPI_VPR(vector) |= CYGARC_REG_IMM_PIC_IPIVPR_MSK;
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
    HAL_IPIC_VPR(vector) &= ~CYGARC_REG_IMM_PIC_IPIVPR_MSK;
  }
#ifdef CYGPKG_HAL_SMP_SUPPORT      
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    HAL_IPIC_IPI_VPR(vector) &= ~CYGARC_REG_IMM_PIC_IPIVPR_MSK;
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
    if(HAL_IPIC_VPR(vector) & CYGARC_REG_IMM_PIC_IPIVPR_MSK)
      return 1;
  }
#ifdef CYGPKG_HAL_SMP_SUPPORT
  else if(vector <= CYGNUM_HAL_INTERRUPT_IPI3)
  {
    if(HAL_IPIC_IPI_VPR(vector) & CYGARC_REG_IMM_PIC_IPIVPR_MSK)
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
         reg &= ~CYGARC_REG_IMM_PIC_EXT_IRQ_SENSE_LEVEL;
       }
       else
       {
         reg |= CYGARC_REG_IMM_PIC_EXT_IRQ_SENSE_LEVEL;
       }
       if (!up) {
         reg &= ~CYGARC_REG_IMM_PIC_EXT_IRQ_HIGH_OR_UP;
       }
       else
       {
         reg |= CYGARC_REG_IMM_PIC_EXT_IRQ_HIGH_OR_UP;
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
    masking = *vpr & CYGARC_REG_IMM_PIC_IPIVPR_MSK;
   
    // mask interrupt
    *vpr |= CYGARC_REG_IMM_PIC_IPIVPR_MSK;
    while((*vpr & CYGARC_REG_PIC_VECPRI_MASK) != CYGARC_REG_PIC_VECPRI_MASK);
 
    // change priority
    *vpr |=  (level << 16);

    // set back mask bit
    if(!masking)
      *vpr &= ~CYGARC_REG_IMM_PIC_IPIVPR_MSK;
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

#ifdef CYGSEM_HAL_POWERPC_QORIQ_RTC_AUTORELOAD
# define CYGARC_DEC_TCR_ARE_BIT CYGARC_REG_TCR_ARE
#else
# define CYGARC_DEC_TCR_ARE_BIT 0
#endif

#ifdef CYGPKG_REDBOOT
# define CYGARC_DEC_TCR_BITS ((CYGARC_DEC_TCR_ARE_BIT) >> 16)
#else
# define CYGARC_DEC_TCR_BITS ((CYGARC_DEC_TCR_ARE_BIT | CYGARC_REG_TCR_DIE) >> 16)
#endif

#ifdef CYGSEM_HAL_POWERPC_QORIQ_RTC_AUTORELOAD

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

#else

#define HAL_CLOCK_RESET( _vector_, _period_ )   \
    CYG_MACRO_START                             \
    cyg_uint32 tmp;                             \
    asm volatile (                              \
        "lis   %0,%3;"                          \
        "mtspr %2,%0;"                          \
        "mtdec %1;"                             \
        : "=r" (tmp)                            \
        : "r"(_period_), "I" (CYGARC_REG_TSR), "I" (CYGARC_REG_TSR_DIS >> 16) \
        );                                      \
    CYG_MACRO_END

#endif

#define HAL_CLOCK_INITIALIZE( _period_ )        \
    CYG_MACRO_START                             \
    cyg_uint32 tmp;                             \
    asm volatile (                              \
        "mtspr %2,%1;"                          \
        "mfspr %0,%3;"                          \
        "oris  %0, %0, %4;"                     \
        "mtspr %3,%0;"                          \
        "mtdec %1;"                             \
        : "=r" (tmp)                            \
        : "r"(_period_), "I" (CYGARC_REG_DECAR), "I" (CYGARC_REG_TCR), "I" (CYGARC_DEC_TCR_BITS) \
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
