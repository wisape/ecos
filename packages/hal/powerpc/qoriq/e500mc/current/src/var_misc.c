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
// Description:  Miscellaneous functions for Freescale QorIQ variant
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
#include <cyg/hal/qoriq.h>
#include <cyg/hal/var_mem.h>
#include <cyg/hal/hal_intr.h>

#ifdef CYGSEM_HAL_POWERPC_QORIQ_E500MC_TRACE
# define qoriq_trace(args...) diag_printf(args)
#else
# define qoriq_trace(args...)
#endif

//--------------------------------------------------------------------------
// Identify processor family from SVR
static qoriq_cpu_type qoriq_cpu[] = {
  { {'P','2','0','4','0'}, 0x02100000, 4},
  { {'P','2','0','4','1'}, 0x02100100, 4},
  { {'P','3','0','4','0'}, 0x02110000, 4},
  { {'P','3','0','4','1'}, 0x02110300, 4},
  { {'P','4','0','4','0'}, 0x02010000, 4},
  { {'P','4','0','8','0'}, 0x02000000, 8},
  { {'P','?','?','?','?'}, 0, 0}
};


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
  // Enable time base from platform CLK/16
   HAL_WRITE_UINT32(
        ( CYGARC_IMM_BASE+CYGARC_REG_IMM_RCPM+CYGARC_REG_IMM_RCPM_CTBENR ),
        ((1<<CYGPKG_HAL_SMP_CPU_MAX)-1));

#ifdef CYGPKG_HAL_SMP_SUPPORT
   HAL_WRITE_UINT32(
        ( CYGARC_IMM_BASE+CYGARC_REG_IMM_RCPM+CYGARC_REG_IMM_RCPM_CTBENR ),
        ((1<<CYGPKG_HAL_SMP_CPU_MAX)-1));
#else
   HAL_WRITE_UINT32(
        ( CYGARC_IMM_BASE+CYGARC_REG_IMM_RCPM+CYGARC_REG_IMM_RCPM_CTBENR ),
          1);
#endif
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

    // This implementation support 32 bits addressing only

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
// If no platform specific requirement, the L2 Cache is enable
#ifndef CYGPLF_ENABLE_L2_CACHE

    // Enable the cache with default settings
    CYGARC_MTSPR(L2CSR0, L2CSR0_L2E);

#else
  CYGPLF_ENABLE_L2_CACHE();
#endif
}


// Enable CPC as cache
externC
void cyg_hal_enable_cpc(void)
{
#ifndef CYGPLF_ENABLE_L3_CACHE
  cyg_uint8 i;
  cyg_uint32 cpc_base, reg;

  for(i = 0; i < CYGHWR_HAL_QORIQ_E500MC_CPC; i++){
    cpc_base = CYGARC_IMM_BASE + CYGARC_REG_IMM_CPC1 + i*0x1000;

    HAL_READ_UINT32(cpc_base + CYGARC_REG_IMM_CPC_HDBCR0, reg);
#ifdef  CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_CPC_A002
    reg |= CYGARC_REG_IMM_CPC_HDBCR0_TECC_SCRUB_DIS;
#endif
#ifdef  CYGHWR_HAL_POWERPC_QORIQ_E500MC_ERRATUM_CPC_A003
    reg |= CYGARC_REG_IMM_CPC_HDBCR0_DECC_SCRUB_DIS;
#endif
    HAL_WRITE_UINT32(cpc_base + CYGARC_REG_IMM_CPC_HDBCR0, reg);

    asm volatile("mbar; isync;");
    HAL_WRITE_UINT32(cpc_base + CYGARC_REG_IMM_CPC_CPCCSR0,
                            ( CPCCSR0_CPCE | CPCCSR0_CPCPE ));
    HAL_READ_UINT32(cpc_base + CYGARC_REG_IMM_CPC_CPCCSR0, reg);
    asm volatile("mbar; isync;");
  }
#else
  CYGPLF_ENABLE_L3_CACHE();
#endif
}

// CPU core reset
externC void
cyg_hal_core_reset (int id)
{
  volatile cyg_uint32 *pir =   (volatile cyg_uint32 *) \
      (CYGARC_IMM_BASE + CYGARC_REG_IMM_PIC_GR_OFFSET + CYGARC_REG_IMM_PIC_PIR);

  *pir = (1 << id);
}


// CPU master reset
externC void
cyg_hal_cpu_hwreset (void)
{
#ifdef CYGSEM_HAL_POWERPC_QORIQ_HRESET_REQ_ENABLE
  volatile cyg_uint32 *rstcr =   (volatile cyg_uint32 *) \
     (CYGARC_IMM_BASE + CYGARC_REG_IMM_RSTCR);
  cyg_uint32 istate;

  HAL_DISABLE_INTERRUPTS(istate);

  *rstcr = CYGARC_REG_IMM_RSTCR_HRESET_REQ;

   hal_delay_us(10000);

#else
   // TODO
  diag_printf("CPU reset not implemented for QorIQ");
#endif
}


#ifdef CYGSEM_HAL_QORIQ_WATCHDOG_ENABLE

// Reset watchdog (7.3.6.1 Watchdog Timer Expiration Out to Platform)
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
// Return core PLL ratio
static cyg_uint32
hal_qoriq_get_pll_ratio(cyg_uint32 field){
   if ( field > 7 && field < 17 && field != 13 ) {
     return field;
   } else {
     diag_printf("Error decoding PLL ratio\n");
     return 1;
   }
}

//--------------------------------------------------------------------------
// Parse RCW to verify CDL entries from the user
externC void
hal_qoriq_parse_rcw( void ){
    cyg_uint32 val, id, coreFreq, pllSel, shift, pll1, pll2, pll3, pll4, ratio;
    cyg_uint32 plfClk, memClk;
    cyg_uint32 pll[7] = {0, 0, 0, 0, 0, 0, 0};
    cyg_uint8  i;

#ifdef CYGSEM_HAL_POWERPC_QORIQ_E500MC_TRACE

    //
    // Dump the Reset Configuration Words
    //
    diag_printf("\nReset Configuration Word\n");
    for(i=0 ; i < 16; i++){
       HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_DCFG_RCWSR(i), val);
       diag_printf("RCW%02d: 0x%08x\n", i, val);
    }

    diag_printf("\nSOC Version\n");

#endif

    //
    // Lookup the CPU type from the SVR SOC Device and Variant ID
    //
    CYGARC_MFSPR(CYGARC_REG_SVR, val);
    id = val & CYGARC_SPR_SVR_SOC_IDs_MASK;
    i  = 0;
    while( qoriq_cpu[i].id != 0 ){
      if ( qoriq_cpu[i].id == id )
        break;
      i++;
    }

#ifdef CYGSEM_HAL_POWERPC_QORIQ_E500MC_TRACE

    //
    // Display SVR information about the revision of the core and
    // processor
    //
    diag_printf("%s%s, Revision %d.%d, SVR (0x%x)\n",
       qoriq_cpu[i].name,
      (val & 0x80000) ? "E" : "",
      CYGARC_SPR_SVR_REV_MAJ(val),
      CYGARC_SPR_SVR_REV_MIN(val),
      val);

    CYGARC_MFSPR(CYGARC_REG_PVR, val);
    diag_printf("E500MC, Revision %d.%d, PVR (0x%x)\n",
      CYGARC_SPR_PVR_REV_MAJ(val),
      CYGARC_SPR_PVR_REV_MIN(val),
      val);

#endif

    //
    // Sanity check: Verify that the number of core configured matches
    // the HW capability
    //
#ifdef CYGPKG_HAL_SMP_SUPPORT
    if ( qoriq_cpu[i].ncore < CYGPKG_HAL_SMP_CPU_MAX )
      diag_printf("SMP target, number of cores mismatch\n");
#endif

    //
    // Sanity check: In this section, it is verified that the configuration
    // provided from the CDL matches the RCW values, provided that the system
    // clock frequency CDL is correct.
    //
    HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PLL1CGSR, pll1);
    HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PLL2CGSR, pll2);
    HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PLL3CGSR, pll3);
    HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_PLL4CGSR, pll4);

    pll[0] = (pll1 >> 1) & 0x3F;
    pll[2] = (pll2 >> 1) & 0x3F;
    pll[4] = (pll3 >> 1) & 0x3F;
    pll[6] = (pll4 >> 1) & 0x3F;

    //
    // Loop through all the cores, they should be configured with similar
    // core frequency.
    //
    for(i=0 ; i < CYGPKG_HAL_SMP_CPU_MAX; i++){
      HAL_READ_UINT32(
         CYGARC_IMM_BASE+(CYGARC_REG_IMM_CLK0C0CSR + ( i << 2 )),
         val);
      pllSel = CYGARC_REG_IMM_CLKxC0CSR_CLK_SEL(val);
      shift = 0;

      if( pllSel % 2 )
        shift = 1;

      if( pll[pllSel>>1] != 0 ) {
          ratio = hal_qoriq_get_pll_ratio(pll[pllSel>>1]);
      }
      else {
        diag_printf("Error decoding PLL configuration\n");
        return;
      }
      coreFreq = (( CYGHWR_HAL_POWERPC_QORIQ_SYS_CLK * ratio ) >> shift);
      qoriq_trace("\nCORE%d frequency:          %d [MHz]\n",
             i,
             coreFreq / 1000000);

      if( coreFreq != CYGHWR_HAL_POWERPC_CPU_SPEED ){
        diag_printf("Core frequency mismatch between CDL and RCW (%d - %d)\n",
          CYGHWR_HAL_POWERPC_CPU_SPEED,
          coreFreq);
      }

    }

    //
    // Verify the platform bus frequency
    //
    HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_DCFG_RCWSR(0), val);
    ratio = (val >> 25) & 0x1f;
    plfClk = CYGHWR_HAL_POWERPC_QORIQ_SYS_CLK * ratio;

    if( plfClk != CYGHWR_HAL_POWERPC_QORIQ_PLF_CLK ){
      diag_printf("Platform bus frequency mismatch between CDL and RCW (%d - %d)\n",
      CYGHWR_HAL_POWERPC_QORIQ_PLF_CLK,
      plfClk);
    }

    qoriq_trace("Platform Clock frequency:  %d [MHz]\n", plfClk/1000000);

    //
    // Verify the DDR bus frequency
    //
    ratio = (val >> 17) & 0x1f;
    HAL_READ_UINT32(CYGARC_IMM_BASE+CYGARC_REG_IMM_DCFG_RCWSR(5), val);
    val = (val >> 7) & 0x01;
    if( val ) // DDR_SYNC=1
      memClk = plfClk * ratio;
    else      // DDR_SYNC=0
      memClk = CYGHWR_HAL_POWERPC_QORIQ_SYS_CLK * ratio;

    if( memClk != CYGHWR_HAL_POWERPC_QORIQ_DDR_CLK ){
      diag_printf("BBR bus frequency mismatch between CDL and RCW (%d - %d)\n",
      CYGHWR_HAL_POWERPC_QORIQ_DDR_CLK,
      memClk);
    }

    qoriq_trace("Memory Clock frequency:   %d [MHz]\n", memClk/1000000);
}

//--------------------------------------------------------------------------
// Dump DDR ECC registers
void
hal_qoriq_dump_ddr_ecc( void ){
   cyg_uint32 reg;
   cyg_uint32 i = 0;

   for(i = 0; i < CYGHWR_HAL_QORIQ_E500MC_DDRC; i++) {
     diag_printf("DDR%d Memory Controller / ECC Configuration\n", (i+1));
     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_ERR_DISABLE,
       reg);
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_MME ){
       diag_printf("  Multiple memory errors      : Disable\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_APE ){
       diag_printf("  Address parity error        : Disable\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_ACE ){
       diag_printf("  Automatic calibration error : Disable\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_CDE ){
       diag_printf("  Corrupted data error        : Disable\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_MBE ){
       diag_printf("  Multiple-bit error          : Disable\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_SBE ){
       diag_printf("  Single-bit ECC error        : Disable\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DISABLE_MSE ){
       diag_printf("  Memory select error         : Disable\n");
     }

     diag_printf("DDR%d Memory Controller / ECC Error\n", (i+1));
     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_ERR_DETECT,
       reg);
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_MME ){
       diag_printf("  Multiple memory errors\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_APE ){
       diag_printf("  Address parity error\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_ACE ){
       diag_printf("  Automatic calibration error\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_CDE ){
       diag_printf("  Corrupted data error\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_MBE ){
       diag_printf("  Multiple-bit error\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_SBE ){
       diag_printf("  Single-bit ECC error\n");
     }
     if ( reg & CYGARC_REG_IMM_DDRC_ERR_DETECT_MSE ){
       diag_printf("  Memory select error\n");
     }

     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_CAPTURE_ADDRESS,
       reg);
     diag_printf("  Capture Address 0x%8x\n", reg);
     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_CAPTURE_DATA_HI,
       reg);
     diag_printf("  Capture Data HI 0x%8x\n", reg);
     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_CAPTURE_DATA_LO,
       reg);
     diag_printf("  Capture Data LO 0x%8x\n", reg);
     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_CAPTURE_ECC,
       reg);
     diag_printf("  Capture ECC     0x%8x\n", reg);

     HAL_READ_UINT32(
       CYGARC_IMM_BASE+CYGARC_REG_IMM_DDRC1+i*0x1000+CYGARC_REG_IMM_DDRC_CAPTURE_ATTRIBUTES,
       reg);
     if ( reg & CYGARC_REG_IMM_DDRC_CAP_ATTR_VLD ){
       diag_printf("  Valid capture flag ON\n");
     }
     else {
       diag_printf("  Valid capture flag OFF\n");
     }
     diag_printf("  Data beat number %d\n",
        CYGARC_REG_IMM_DDRC_CAP_ATTR_BNUM(reg));
     diag_printf("  Transaction size %d\n",
        CYGARC_REG_IMM_DDRC_CAP_ATTR_TSIZ(reg));
     diag_printf("  Transaction source %x\n",
        CYGARC_REG_IMM_DDRC_CAP_ATTR_TSRC(reg));
     diag_printf("  Transaction type %d\n",
        CYGARC_REG_IMM_DDRC_CAP_ATTR_TTYP(reg));

   }
}


//--------------------------------------------------------------------------
// End of var_misc.c
