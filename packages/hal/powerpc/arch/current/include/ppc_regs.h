#ifndef CYGONCE_HAL_PPC_REGS_H
#define CYGONCE_HAL_PPC_REGS_H

//==========================================================================
//
//      ppc_regs.h
//
//      PowerPC CPU definitions
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2007 Free Software Foundation, Inc.
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
// Author(s):    jskov
// Contributors: jskov
// Date:         1999-02-19
// Purpose:      Provide PPC register definitions
// Description:  Provide PPC register definitions
//               The short difinitions (sans CYGARC_REG_) are exported only
//               if CYGARC_HAL_COMMON_EXPORT_CPU_MACROS is defined.
// Usage:
//               #include <cyg/hal/ppc_regs.h>
//               ...
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>

#include <cyg/hal/var_regs.h>

//--------------------------------------------------------------------------
// SPR access macros.
#define CYGARC_MTSPR(_spr_, _v_) \
    asm volatile ("mtspr %0, %1;" :: "I" (_spr_), "r" (_v_));
#define CYGARC_MFSPR(_spr_, _v_) \
    asm volatile ("mfspr %0, %1;" : "=r" (_v_) : "I" (_spr_));

//--------------------------------------------------------------------------
// TB access macros.
#define CYGARC_MTTB(_tbr_, _v_) \
    asm volatile ("mttb %0, %1;" :: "I" (_tbr_), "r" (_v_));
#define CYGARC_MFTB(_tbr_, _v_) \
    asm volatile ("mftb %0, %1;" : "=r" (_v_) : "I" (_tbr_));

//--------------------------------------------------------------------------
// PMR access macros.
#define CYGARC_MTPMR(_pmr_, _v_) \
    asm volatile ("mtpmr %0, %1;" :: "I" (_pmr_), "r" (_v_));
#define CYGARC_MFPMR(_pmr_, _v_) \
    asm volatile ("mfpmr %0, %1;" : "=r" (_v_) : "I" (_pmr_));

//--------------------------------------------------------------------------
// Generic PowerPC Family Definitions
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Some SPRs

#define CYGARC_REG_XER       1
#define CYGARC_REG_LR        8
#define CYGARC_REG_CTR       9
#define CYGARC_REG_DSISR    18
#define CYGARC_REG_DAR      19
#define CYGARC_REG_DEC      22
#define CYGARC_REG_SRR0     26
#define CYGARC_REG_SRR1     27
#define CYGARC_REG_SPRG0   272
#define CYGARC_REG_SPRG1   273
#define CYGARC_REG_SPRG2   274
#define CYGARC_REG_SPRG3   275
#define CYGARC_REG_PVR     287

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define DSISR      CYGARC_REG_DSISR
#define DAR        CYGARC_REG_DAR
#define DEC        CYGARC_REG_DEC
#define SRR0       CYGARC_REG_SRR0
#define SRR1       CYGARC_REG_SRR1
#define SPRG0      CYGARC_REG_SPRG0
#define SPRG1      CYGARC_REG_SPRG1
#define SPRG2      CYGARC_REG_SPRG2
#define SPRG3      CYGARC_REG_SPRG3
#define PVR        CYGARC_REG_PVR
#endif

//--------------------------------------------------------------------------
// MSR bits

#if !defined(CYGHWR_HAL_POWERPC_BOOK_E)

#define CYGARC_REG_MSR_LE       0x00000001   // little-endian mode enable
#define CYGARC_REG_MSR_RI       0x00000002   // recoverable exception
#define CYGARC_REG_MSR_DR       0x00000010   // data address translation
#define CYGARC_REG_MSR_IR       0x00000020   // instruction address translation
#define CYGARC_REG_MSR_IP       0x00000040   // exception prefix
#define CYGARC_REG_MSR_FE1      0x00000100   // floating-point exception mode 1
#define CYGARC_REG_MSR_BE       0x00000200   // branch trace enable
#define CYGARC_REG_MSR_SE       0x00000400   // single-step trace enable
#define CYGARC_REG_MSR_FE0      0x00000800   // floating-point exception mode 0
#define CYGARC_REG_MSR_ME       0x00001000   // machine check enable
#define CYGARC_REG_MSR_FP       0x00002000   // floating-point available
#define CYGARC_REG_MSR_PR       0x00004000   // privilege level
#define CYGARC_REG_MSR_EE       0x00008000   // external interrupt enable
#define CYGARC_REG_MSR_ILE      0x00010000   // exception little-endian mode
#define CYGARC_REG_MSR_POW      0x00040000   // power management enable

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define MSR_LE          CYGARC_REG_MSR_LE 
#define MSR_RI          CYGARC_REG_MSR_RI 
#define MSR_DR          CYGARC_REG_MSR_DR 
#define MSR_IR          CYGARC_REG_MSR_IR 
#define MSR_IP          CYGARC_REG_MSR_IP 
#define MSR_FE1         CYGARC_REG_MSR_FE1
#define MSR_BE          CYGARC_REG_MSR_BE 
#define MSR_SE          CYGARC_REG_MSR_SE 
#define MSR_FE0         CYGARC_REG_MSR_FE0
#define MSR_ME          CYGARC_REG_MSR_ME 
#define MSR_FP          CYGARC_REG_MSR_FP 
#define MSR_PR          CYGARC_REG_MSR_PR 
#define MSR_EE          CYGARC_REG_MSR_EE 
#define MSR_ILE         CYGARC_REG_MSR_ILE
#define MSR_POW         CYGARC_REG_MSR_POW
#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#else

// The MSR on BOOK E processors has some bits in common with the AIM
// architecture, but also has some differences.
#ifdef CYGHWR_HAL_POWERPC_E500_mc
#define CYGARC_REG_MSR_RI       0x00000002   // recoverable exception
#endif
#define CYGARC_REG_MSR_PMM      0x00000004   // performance monitor mark
#define CYGARC_REG_MSR_DS       0x00000010   // data address space
#define CYGARC_REG_MSR_IS       0x00000020   // instruction address space
#define CYGARC_REG_MSR_FE1      0x00000100   // floating-point exception mode 1
#define CYGARC_REG_MSR_DE       0x00000200   // debug interrupt enable
#define CYGARC_REG_MSR_UBLE     0x00000400   // User BTB lock enable (e
#define CYGARC_REG_MSR_FE0      0x00000800   // floating-point exception mode 0
#define CYGARC_REG_MSR_ME       0x00001000   // machine check enable
#define CYGARC_REG_MSR_FP       0x00002000   // floating-point available
#define CYGARC_REG_MSR_PR       0x00004000   // privilege level
#define CYGARC_REG_MSR_EE       0x00008000   // external interrupt enable
#define CYGARC_REG_MSR_CE       0x00020000   // critical interrupt enable
#define CYGARC_REG_MSR_WE       0x00040000   // wait state enable
#define CYGARC_REG_MSR_SPE      0x02000000   // SPE enable
#define CYGARC_REG_MSR_UCLE     0x04000000   // user cache lock enable
#ifdef CYGHWR_HAL_POWERPC_E500_mc
#define CYGARC_REG_MSR_GS       0x10000000   // guest state
#endif

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define MSR_PMM         CYGARC_REG_MSR_PMM
#define MSR_DS          CYGARC_REG_MSR_DS
#define MSR_IS          CYGARC_REG_MSR_IS 
#define MSR_FE1         CYGARC_REG_MSR_FE1
#define MSR_DE          CYGARC_REG_MSR_DE 
#define MSR_UBLE        CYGARC_REG_MSR_UBLE
#define MSR_FE0         CYGARC_REG_MSR_FE0
#define MSR_ME          CYGARC_REG_MSR_ME 
#define MSR_FP          CYGARC_REG_MSR_FP 
#define MSR_PR          CYGARC_REG_MSR_PR 
#define MSR_EE          CYGARC_REG_MSR_EE 
#define MSR_CE          CYGARC_REG_MSR_CE 
#define MSR_WE          CYGARC_REG_MSR_WE 
#define MSR_SPE         CYGARC_REG_MSR_SPE
#define MSR_UCLE        CYGARC_REG_MSR_UCLE

// The following bits are not defined by BOOK E processors. However
// they are referenced in the architecture HAL. By defining them as
// zero we neutralize their effect.
#define MSR_DR          0
#define MSR_IR          0
#ifdef CYGHWR_HAL_POWERPC_E500_mc
#define MSR_RI          CYGARC_REG_MSR_RI
#define MSR_GS          CYGARC_REG_MSR_GS
#else
#define MSR_RI          0
#endif

#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#endif

//--------------------------------------------------------------------------
// Time Base Registers
// READ and WRITE are different addresses on some variants!

#define CYGARC_REG_TBL_W   284
#define CYGARC_REG_TBU_W   285

#if !defined(CYGHWR_HAL_POWERPC_BOOK_E)
#define CYGARC_REG_TBL_R   268
#define CYGARC_REG_TBU_R   269
#else
#define CYGARC_REG_TBL_R   284
#define CYGARC_REG_TBU_R   285
#endif

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define TBL_W      CYGARC_REG_TBL_W
#define TBU_W      CYGARC_REG_TBU_W
#define TBL_R      CYGARC_REG_TBL_R
#define TBU_R      CYGARC_REG_TBU_R
#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#if defined(CYGHWR_HAL_POWERPC_BOOK_E)

#define CYGARC_REG_DECAR    54
#define CYGARC_REG_TCR     340
#define CYGARC_REG_TSR     336

#define CYGARC_REG_TCR_WP(__x)          ((((__x)&3)<<30)|(((__x)&0x3C)<<15))
#define CYGARC_REG_TCR_WRC_NONE         (0<<28)
#define CYGARC_REG_TCR_WRC_CHECKSTOP    (1<<28)
#define CYGARC_REG_TCR_WRC_RESET        (2<<28)
#define CYGARC_REG_TCR_WIE              (1<<27)
#define CYGARC_REG_TCR_DIE              (1<<26)
#define CYGARC_REG_TCR_FP(__x)          ((((__x)&3)<<24)|(((__x)&0x3C)<<11))
#define CYGARC_REG_TCR_FIE              (1<<23)
#define CYGARC_REG_TCR_ARE              (1<<22)


#define CYGARC_REG_TSR_ENW              (1<<31)
#define CYGARC_REG_TSR_WIS              (1<<30)
#define CYGARC_REG_TSR_WRS_NONE         (0<<28)
#define CYGARC_REG_TSR_WRS_CHECKSTOP    (1<<28)
#define CYGARC_REG_TSR_WRS_RESET        (2<<28)
#define CYGARC_REG_TSR_DIS              (1<<27)
#define CYGARC_REG_TSR_FIS              (1<<26)

#endif

//-----------------------------------------------------------------------------
// Exception Syndrome Register

#if defined(CYGHWR_HAL_POWERPC_BOOK_E)

#define CYGARC_REG_ESR          62

#define CYGARC_REG_ESR_PIL      (1<<27)
#define CYGARC_REG_ESR_PPR      (1<<26)
#define CYGARC_REG_ESR_PTR      (1<<25)
#define CYGARC_REG_ESR_FP       (1<<24)

#endif


#if defined(CYGHWR_HAL_POWERPC_BOOK_E)
#define CYGARC_REG_PID        48
#define CYGARC_REG_CSRR0      58
#define CYGARC_REG_CSRR1      59
#define CYGARC_REG_DEAR       61
#define CYGARC_REG_IPVR       63
#define CYGARC_REG_USPRG0    256
#define CYGARC_REG_SPRG4R    260  
#define CYGARC_REG_SPRG5R    261
#define CYGARC_REG_SPRG6R    262
#define CYGARC_REG_SPRG7R    263
#define CYGARC_REG_SPRG4W    276
#define CYGARC_REG_SPRG5W    277
#define CYGARC_REG_SPRG6W    278
#define CYGARC_REG_SPRG7W    279
#define CYGARC_REG_PIR       286
#define CYGARC_REG_DBSR      304
#define CYGARC_REG_DBCR0     308
#define CYGARC_REG_DBCR1     309
#define CYGARC_REG_DBCR2     310
#define CYGARC_REG_IAC1      312
#define CYGARC_REG_IAC2      313
#define CYGARC_REG_IAC3      314
#define CYGARC_REG_IAC4      315
#define CYGARC_REG_DAC1      316
#define CYGARC_REG_DAC2      317
#define CYGARC_REG_DVC1      318
#define CYGARC_REG_DVC2      319
#define CYGARC_REG_IVOR0     400
#define CYGARC_REG_IVOR1     401
#define CYGARC_REG_IVOR2     402
#define CYGARC_REG_IVOR3     403
#define CYGARC_REG_IVOR4     404
#define CYGARC_REG_IVOR5     405
#define CYGARC_REG_IVOR6     406
#define CYGARC_REG_IVOR7     407
#define CYGARC_REG_IVOR8     408
#define CYGARC_REG_IVOR9     409
#define CYGARC_REG_IVOR10    410
#define CYGARC_REG_IVOR11    411
#define CYGARC_REG_IVOR12    412
#define CYGARC_REG_IVOR13    413
#define CYGARC_REG_IVOR14    414
#define CYGARC_REG_IVOR15    415
#define CYGARC_REG_SVR       1023

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define PID        CYGARC_REG_PID
#define DECAR      CYGARC_REG_DECAR
#define CSRR0      CYGARC_REG_CSRR0
#define CSRR1      CYGARC_REG_CSRR1
#define DEAR       CYGARC_REG_DEAR
#define ESR        CYGARC_REG_ESR
#define IPVR       CYGARC_REG_IPVR
#define USPRG0     CYGARC_REG_USPRG0
#define SPRG4R     CYGARC_REG_SPRG4R
#define SPRG5R     CYGARC_REG_SPRG5R
#define SPRG6R     CYGARC_REG_SPRG6R
#define SPRG7R     CYGARC_REG_SPRG7R
#define SPRG4W     CYGARC_REG_SPRG4W
#define SPRG5W     CYGARC_REG_SPRG5W
#define SPRG6W     CYGARC_REG_SPRG6W
#define SPRG7W     CYGARC_REG_SPRG7W
#define PIR        CYGARC_REG_PIR
#define DBSR       CYGARC_REG_DBSR
#define DBCR0      CYGARC_REG_DBCR0
#define DBCR1      CYGARC_REG_DBCR1
#define DBCR2      CYGARC_REG_DBCR2
#define IAC1       CYGARC_REG_IAC1
#define IAC2       CYGARC_REG_IAC2
#define IAC3       CYGARC_REG_IAC3
#define IAC4       CYGARC_REG_IAC4
#define DAC1       CYGARC_REG_DAC1
#define DAC2       CYGARC_REG_DAC2
#define DVC1       CYGARC_REG_DVC1
#define DVC2       CYGARC_REG_DVC2
#define TSR        CYGARC_REG_TSR
#define TCR        CYGARC_REG_TCR
#define IVOR0      CYGARC_REG_IVOR0
#define IVOR1      CYGARC_REG_IVOR1
#define IVOR2      CYGARC_REG_IVOR2
#define IVOR3      CYGARC_REG_IVOR3
#define IVOR4      CYGARC_REG_IVOR4
#define IVOR5      CYGARC_REG_IVOR5
#define IVOR6      CYGARC_REG_IVOR6
#define IVOR7      CYGARC_REG_IVOR7
#define IVOR8      CYGARC_REG_IVOR8
#define IVOR9      CYGARC_REG_IVOR9
#define IVOR10     CYGARC_REG_IVOR10
#define IVOR11     CYGARC_REG_IVOR11
#define IVOR12     CYGARC_REG_IVOR12
#define IVOR13     CYGARC_REG_IVOR13
#define IVOR14     CYGARC_REG_IVOR14
#define IVOR15     CYGARC_REG_IVOR15
#define SVR        CYGARC_REG_SVR
#endif
#endif


#ifdef CYGHWR_HAL_POWERPC_E300
#define CYGARC_REG_SDR1             25
#define CYGARC_REG_EAR             282
#define CYGARC_REG_IBAT0U          528
#define CYGARC_REG_IBAT0L          529
#define CYGARC_REG_IBAT1U          530
#define CYGARC_REG_IBAT1L          531
#define CYGARC_REG_IBAT2U          532
#define CYGARC_REG_IBAT2L          533
#define CYGARC_REG_IBAT3U          534
#define CYGARC_REG_IBAT3L          535
#define CYGARC_REG_IBAT4U          560
#define CYGARC_REG_IBAT4L          561
#define CYGARC_REG_IBAT5U          562
#define CYGARC_REG_IBAT5L          563
#define CYGARC_REG_IBAT6U          564
#define CYGARC_REG_IBAT6L          565
#define CYGARC_REG_IBAT7U          566
#define CYGARC_REG_IBAT7L          567
#define CYGARC_REG_DBAT0U          536
#define CYGARC_REG_DBAT0L          537
#define CYGARC_REG_DBAT1U          538
#define CYGARC_REG_DBAT1L          539
#define CYGARC_REG_DBAT2U          540
#define CYGARC_REG_DBAT2L          541
#define CYGARC_REG_DBAT3U          542
#define CYGARC_REG_DBAT3L          543
#define CYGARC_REG_DBAT4U          568
#define CYGARC_REG_DBAT4L          569
#define CYGARC_REG_DBAT5U          570
#define CYGARC_REG_DBAT5L          571
#define CYGARC_REG_DBAT6U          572
#define CYGARC_REG_DBAT6L          573
#define CYGARC_REG_DBAT7U          574
#define CYGARC_REG_DBAT7L          575
#define CYGARC_REG_DMISS           976
#define CYGARC_REG_DCMP            977
#define CYGARC_REG_HASH1           978
#define CYGARC_REG_HASH2           979
#define CYGARC_REG_IMISS           980
#define CYGARC_REG_ICMP            981
#define CYGARC_REG_RPA             982
#define CYGARC_REG_HID0           1008
#define CYGARC_REG_HID1           1009
#define CYGARC_REG_IABR           1010
#define CYGARC_REG_HID2           1011
#define CYGARC_REG_DABR           1013

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define IBAT0U          528
#define IBAT0L          529
#define IBAT1U          530
#define IBAT1L          531
#define IBAT2U          532
#define IBAT2L          533
#define IBAT3U          534
#define IBAT3L          535
#define IBAT4U          560
#define IBAT4L          561
#define IBAT5U          562
#define IBAT5L          563
#define IBAT6U          564
#define IBAT6L          565
#define IBAT7U          566
#define IBAT7L          567
#define DBAT0U          536
#define DBAT0L          537
#define DBAT1U          538
#define DBAT1L          539
#define DBAT2U          540
#define DBAT2L          541
#define DBAT3U          542
#define DBAT3L          543
#define DBAT4U          568
#define DBAT4L          569
#define DBAT5U          570
#define DBAT5L          571
#define DBAT6U          572
#define DBAT6L          573
#define DBAT7U          574
#define DBAT7L          575
#define HID0            CYGARC_REG_HID0
#define HID1            CYGARC_REG_HID1
#define HID2            CYGARC_REG_HID2
#define SDR1            CYGARC_REG_SDR1
#define EAR             CYGARC_REG_EAR
#endif
#endif

#if defined(CYGHWR_HAL_POWERPC_E500_v1) || \
    defined(CYGHWR_HAL_POWERPC_E500_v2) || \
    defined(CYGHWR_HAL_POWERPC_E500_mc)
#define CYGARC_REG_L1CFG0    515
#define CYGARC_REG_L1CFG1    516
#define CYGARC_REG_IVOR35    531
#define CYGARC_REG_MCSRR0    570
#define CYGARC_REG_MCSRR1    571
#define CYGARC_REG_MCSR      572
#define CYGARC_REG_MCAR      573
#define CYGARC_REG_MAS0      624
#define CYGARC_REG_MAS1      625
#define CYGARC_REG_MAS2      626
#define CYGARC_REG_MAS3      627
#define CYGARC_REG_MAS4      628
#define CYGARC_REG_MAS5      629
#define CYGARC_REG_MAS6      630
#define CYGARC_REG_TLB0CFG   688
#define CYGARC_REG_TLB1CFG   689
#define CYGARC_REG_MAS7      944
#define CYGARC_REG_HID0     1008
#define CYGARC_REG_L1CSR0   1010
#define CYGARC_REG_L1CSR1   1011
#define CYGARC_REG_MMUCSR0  1012
#define CYGARC_REG_BUCSR    1013
#define CYGARC_REG_MMUCFG   1015

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define MAS0        CYGARC_REG_MAS0
#define MAS1        CYGARC_REG_MAS1
#define MAS2        CYGARC_REG_MAS2
#define MAS3        CYGARC_REG_MAS3
#define MAS4        CYGARC_REG_MAS4
#define MAS5        CYGARC_REG_MAS5
#define MAS6        CYGARC_REG_MAS6
#define MAS7        CYGARC_REG_MAS7
#define HID0        CYGARC_REG_HID0
#define IVOR35      CYGARC_REG_IVOR35
#define BUCSR       CYGARC_REG_BUCSR
#define L1CSR0      CYGARC_REG_L1CSR0
#define L1CSR1      CYGARC_REG_L1CSR1
#define L1CFG0      CYGARC_REG_L1CFG0
#define L1CFG1      CYGARC_REG_L1CFG1
#define MCSR        CYGARC_REG_MCSR
#define MCAR        CYGARC_REG_MCAR
#define MCSRR0      CYGARC_REG_MCSRR0
#define MCSRR1      CYGARC_REG_MCSRR1
#define MMUCSR0     CYGARC_REG_MMUCSR0
#define MMUCFG      CYGARC_REG_MMUCFG
#define TLB0CFG     CYGARC_REG_TLB0CFG
#define TLB1CFG     CYGARC_REG_TLB1CFG
#endif
#endif


#if defined(CYGHWR_HAL_POWERPC_E500_v1) || \
    defined(CYGHWR_HAL_POWERPC_E500_v2)
#define CYGARC_REG_PMC0       16
#define CYGARC_REG_PMC1       17
#define CYGARC_REG_PMC2       18
#define CYGARC_REG_PMC3       19
#define CYGARC_REG_PMLCA0    144
#define CYGARC_REG_PMLCA1    145
#define CYGARC_REG_PMLCA2    146
#define CYGARC_REG_PMLCA3    147
#define CYGARC_REG_PMLCB0    272
#define CYGARC_REG_PMLCB1    273
#define CYGARC_REG_PMLCB2    274
#define CYGARC_REG_PMLCB3    275
#define CYGARC_REG_PMGC0     400
#define CYGARC_REG_BBEAR     513
#define CYGARC_REG_BBTAR     514
#define CYGARC_REG_IVOR32    528
#define CYGARC_REG_IVOR33    529
#define CYGARC_REG_IVOR34    530
#define CYGARC_REG_HID1     1009

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define IVOR32      CYGARC_REG_IVOR32
#define IVOR33      CYGARC_REG_IVOR33
#define IVOR34      CYGARC_REG_IVOR34
#define HID1        CYGARC_REG_HID1
#define PMC0        CYGARC_REG_PMC0
#define PMC1        CYGARC_REG_PMC1
#define PMC2        CYGARC_REG_PMC2
#define PMC3        CYGARC_REG_PMC3
#define PMLCA0      CYGARC_REG_PMLCA0
#define PMLCA1      CYGARC_REG_PMLCA1
#define PMLCA2      CYGARC_REG_PMLCA2
#define PMLCA3      CYGARC_REG_PMLCA3
#define PMLCB0      CYGARC_REG_PMLCB0
#define PMLCB1      CYGARC_REG_PMLCB1
#define PMLCB2      CYGARC_REG_PMLCB2
#define PMLCB3      CYGARC_REG_PMLCB3
#define PMGC0       CYGARC_REG_PMGC0
#define BBEAR       CYGARC_REG_BBEAR
#define BBTAR       CYGARC_REG_BBTAR
#endif
#endif


#if defined(CYGHWR_HAL_POWERPC_E500_mc)
#define CYGARC_REG_DBSRWR        306
#define CYGARC_REG_EPCR          307
#define CYGARC_REG_MSRP          311
#define CYGARC_REG_LPIDR         338
#define CYGARC_REG_MAS8          341
#define CYGARC_REG_GSPRG0        368
#define CYGARC_REG_GSPRG1        369
#define CYGARC_REG_GSPRG2        370
#define CYGARC_REG_GSPRG3        371
#define CYGARC_REG_GSRR0         378
#define CYGARC_REG_GSRR1         379
#define CYGARC_REG_GEPR          380
#define CYGARC_REG_GDEAR         381
#define CYGARC_REG_GPIR          382
#define CYGARC_REG_GESR          383
#define CYGARC_REG_IVOR38        432
#define CYGARC_REG_IVOR39        433
#define CYGARC_REG_IVOR40        434
#define CYGARC_REG_IVOR41        435
#define CYGARC_REG_GIVOR2        440
#define CYGARC_REG_GIVOR3        441
#define CYGARC_REG_GIVOR4        442
#define CYGARC_REG_GIVOR8        443
#define CYGARC_REG_GIVOR13       444
#define CYGARC_REG_GIVOR14       445
#define CYGARC_REG_GIVPR         447
#define CYGARC_REG_NPIDR         517
#define CYGARC_REG_L2CFG0        519
#define CYGARC_REG_IVOR36        532
#define CYGARC_REG_IVOR37        533
#define CYGARC_REG_MCARU         569
#define CYGARC_REG_DSRR0         574
#define CYGARC_REG_DSRR1         575
#define CYGARC_REG_DDAM          576
#define CYGARC_REG_L1CSR2        606
#define CYGARC_REG_CDCSR0        696
#define CYGARC_REG_EPR           702
#define CYGARC_REG_L2ERRINTEN    720
#define CYGARC_REG_L2ERRATTR     721
#define CYGARC_REG_L2ERRADDR     722
#define CYGARC_REG_L2ERREADDR    723
#define CYGARC_REG_L2ERRCTL      724
#define CYGARC_REG_L2ERRDIS      725
#define CYGARC_REG_EPLC          947
#define CYGARC_REG_EPSC          948
#define CYGARC_REG_DEVENT        975
#define CYGARC_REG_NSPD          983
#define CYGARC_REG_NSPC          984
#define CYGARC_REG_L2ERRINJHI    985
#define CYGARC_REG_L2ERRINJLO    986
#define CYGARC_REG_L2ERRINJCTL   987
#define CYGARC_REG_L2CAPTDATAHI  988
#define CYGARC_REG_L2CAPTDATALO  989
#define CYGARC_REG_L2CAPTECC     990
#define CYGARC_REG_L2ERRDET      991
#define CYGARC_REG_L2CSR0        1017
#define CYGARC_REG_L2CSR1        1018

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define DBSRWR        CYGARC_REG_DBSRWR
#define EPCR          CYGARC_REG_EPCR
#define MSRP          CYGARC_REG_MSRP
#define LPIDR         CYGARC_REG_LPIDR
#define MAS8          CYGARC_REG_MAS8
#define GSPRG0        CYGARC_REG_GSPRG0
#define GSPRG1        CYGARC_REG_GSPRG1
#define GSPRG2        CYGARC_REG_GSPRG2
#define GSPRG3        CYGARC_REG_GSPRG3
#define GSRR0         CYGARC_REG_GSRR0
#define GSRR1         CYGARC_REG_GSRR1
#define GEPR          CYGARC_REG_GEPR
#define GDEAR         CYGARC_REG_GDEAR
#define GPIR          CYGARC_REG_GPIR
#define GESR          CYGARC_REG_GESR
#define IVOR38        CYGARC_REG_IVOR38
#define IVOR39        CYGARC_REG_IVOR39
#define IVOR40        CYGARC_REG_IVOR40
#define IVOR41        CYGARC_REG_IVOR41
#define GIVOR2        CYGARC_REG_GIVOR2
#define GIVOR3        CYGARC_REG_GIVOR3
#define GIVOR4        CYGARC_REG_GIVOR4
#define GIVOR8        CYGARC_REG_GIVOR8
#define GIVOR13       CYGARC_REG_GIVOR13
#define GIVOR14       CYGARC_REG_GIVOR14
#define GIVPR         CYGARC_REG_GIVPR
#define NPIDR         CYGARC_REG_NPIDR
#define L2CFG0        CYGARC_REG_L2CFG0
#define IVOR36        CYGARC_REG_IVOR36
#define IVOR37        CYGARC_REG_IVOR37
#define MCARU         CYGARC_REG_MCARU
#define DSRR0         CYGARC_REG_DSRR0
#define DSRR1         CYGARC_REG_DSRR1
#define DDAM          CYGARC_REG_DDAM
#define L1CSR2        CYGARC_REG_L1CSR2
#define CDCSR0        CYGARC_REG_CDCSR0
#define EPR           CYGARC_REG_EPR
#define L2ERRINTEN    CYGARC_REG_L2ERRINTEN
#define L2ERRATTR     CYGARC_REG_L2ERRATTR
#define L2ERRADDR     CYGARC_REG_L2ERRADDR
#define L2ERREADDR    CYGARC_REG_L2ERREADDR
#define L2ERRCTL      CYGARC_REG_L2ERRCTL
#define L2ERRDIS      CYGARC_REG_L2ERRDIS
#define EPLC          CYGARC_REG_EPLC
#define EPSC          CYGARC_REG_EPSC
#define DEVENT        CYGARC_REG_DEVENT
#define NSPD          CYGARC_REG_NSPD
#define NSPC          CYGARC_REG_NSPC
#define L2ERRINJHI    CYGARC_REG_L2ERRINJHI
#define L2ERRINJLO    CYGARC_REG_L2ERRINJLO
#define L2ERRINJCTL   CYGARC_REG_L2ERRINJCTL
#define L2CAPTDATAHI  CYGARC_REG_L2CAPTDATAHI
#define L2CAPTDATALO  CYGARC_REG_L2CAPTDATALO
#define L2CAPTECC     CYGARC_REG_L2CAPTECC
#define L2ERRDET      CYGARC_REG_L2ERRDET
#define L2CSR0        CYGARC_REG_L2CSR0
#define L2CSR1        CYGARC_REG_L2CSR1
#endif
#endif

//-----------------------------------------------------------------------------
#endif // ifdef CYGONCE_HAL_PPC_REGS_H
// End of ppc_regs.h
