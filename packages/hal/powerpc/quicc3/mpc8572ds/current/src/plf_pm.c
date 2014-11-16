//==========================================================================
//
//      plf_pm.c
//
//      HAL performance monitoring support
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 2010 Free Software Foundation, Inc.
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
// Description:  Performance monitoring for Freescale MPC8572DS
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
#include <cyg/hal/hal_smp.h>

//--------------------------------------------------------------------------
//
// Performance monitoring support
//
// The performance monitoring code example here is made for single core use.
// For dual core use, the system performance monitoring is identical but the
// the core monitoring part must be duplicated for every core therefore shall 
// be move in the per core CPU initialisation section. 
              
#include <cyg/hal/hal_intr.h>          // HAL interrupts/exceptions
#include <cyg/kernel/kapi.h>
  
#ifndef CYGPKG_HAL_SMP_SUPPORT
#define HAL_SMP_CPU_MAX 1
#define HAL_SMP_CPU_THIS() 0
#define HAL_SMP_CPU_TYPE cyg_uint32
#endif

static cyg_uint32 pmc[HAL_SMP_CPU_MAX][4];

#if defined(CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_CACHE)
/*
  * Here we configure the system events monitoring stuff ( checkout 8572 errata manual )
  * SE:C2:213 -> L2 Icache hit
  * SE:Ref:23 -> L2 Icache miss
  * SE:C4:121 -> L2 Dcache hit
  * SE:C5:115 -> L2 Dcache miss
  * SE:C1:118 -> Non core read hit in L2
  * SE:Ref25  -> Non core read miss in L2
  * SE:C6:120 -> Non core burst write to L2
  * SE:C7:116 -> Non core non-burst write to L2
  */
static
void hal_setup_sys_mon_cache( void )
{
  volatile struct sys_mon_85xx *sys_mon = \
            (volatile struct sys_mon_85xx *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_SYS_MON);

  /* Init counter*/
  sys_mon->pmlc0_hi = 0 ;
  sys_mon->pmlc0_lo = 0 ;
  sys_mon->pmlc1    = 0 ;
  sys_mon->pmlc2    = 0 ;
  sys_mon->pmlc3    = 0 ;
  sys_mon->pmlc4    = 0 ;
  sys_mon->pmlc5    = 0 ;
  sys_mon->pmlc6    = 0 ;
  sys_mon->pmlc7    = 0 ;
  sys_mon->pmlc8    = 0 ;
  sys_mon->pmlc9    = 0 ;  
  sys_mon->pmlc10   = 0 ;
  sys_mon->pmlc11   = 0 ;  

  /* Stop monitoring */
  sys_mon->pmgc0   = 0x80000000;

  /* Local Control Registers setting */
  sys_mon->pmlca0  = 0x04000000;
  sys_mon->pmlca2  = 0x04D50000;
  sys_mon->pmlca3  = 0x04170000;
  sys_mon->pmlca4  = 0x04790000;
  sys_mon->pmlca5  = 0x04730000;
  sys_mon->pmlca1  = 0x04760000;
  sys_mon->pmlca8  = 0x04190000;
  sys_mon->pmlca6  = 0x04780000;
  sys_mon->pmlca7  = 0x04740000;

  return;
}


/*
  * Here we configure the core events monitoring stuff
  * CE:Ref:2  -> Instruction completed
  * CE:Ref:1  -> Processor cycle
  * CE:Com:60 -> Instructions L1 cache reloads
  * CE:Com:41 -> Data L1 cache reloads
  * CE:Com:9  -> Load completed
  * CE:Com:10 -> Store completed
  */
static
void hal_setup_core_mon_cache( void )
{
  cyg_uint32 pmc = 0;

  /* Stop monitoring */
  CYGARC_MTPMR( CYGARC_REG_PMGC0, 0x80000000 );

  /* Reset counters */
  CYGARC_MTPMR( CYGARC_REG_PMC0 , pmc );
  CYGARC_MTPMR( CYGARC_REG_PMC1 , pmc );
  CYGARC_MTPMR( CYGARC_REG_PMC2 , pmc );
  CYGARC_MTPMR( CYGARC_REG_PMC3 , pmc );

  /* Local Control Registers setting */
  CYGARC_MTPMR( CYGARC_REG_PMLCA0, 0x04010000 );
  CYGARC_MTPMR( CYGARC_REG_PMLCA1, 0x04020000 );
#if 0
  CYGARC_MTPMR( CYGARC_REG_PMLCA2, 0x043C0000 );
  CYGARC_MTPMR( CYGARC_REG_PMLCA3, 0x04290000 );
#else
  CYGARC_MTPMR( CYGARC_REG_PMLCA2, 0x04090000 );
  CYGARC_MTPMR( CYGARC_REG_PMLCA3, 0x040A0000 );
#endif

  return;
}


/*
  * Display cache monitoring registers
  *
  */
static
void hal_show_mpc8572_pm_cache( void )
{
  volatile struct sys_mon_85xx *sys_mon = \
             (volatile struct sys_mon_85xx *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_SYS_MON);

  HAL_SMP_CPU_TYPE cpu = 0;

  cyg_uint32 pmlc1;
  cyg_uint32 pmlc2;
  cyg_uint32 pmlc3;
  cyg_uint32 pmlc4;
  cyg_uint32 pmlc5;
  cyg_uint32 pmlc6;
  cyg_uint32 pmlc7;
  cyg_uint32 pmlc8; 

  pmlc1 = sys_mon->pmlc1;
  pmlc2 = sys_mon->pmlc2;
  pmlc3 = sys_mon->pmlc3;
  pmlc4 = sys_mon->pmlc4;
  pmlc5 = sys_mon->pmlc5;
  pmlc6 = sys_mon->pmlc6;
  pmlc7 = sys_mon->pmlc7;
  pmlc8 = sys_mon->pmlc8;

  while( cpu < HAL_SMP_CPU_MAX )
  {
    diag_printf("%s %15u\n\r", "Instructions Completed         ", pmc[cpu][0] );
    diag_printf("%s %15u\n\r", "Core Cycles                    ", pmc[cpu][1] );
#if 0
    diag_printf("%s %15u\n\r", "Instructions L1 cache reload   ", pmc[cpu][2] );
    diag_printf("%s %15u\n\r", "Data L1 cache reload           ", pmc[cpu][3] );
#else
    diag_printf("%s %15u\n\r", "Loads completed                ", pmc[cpu][2] );
    diag_printf("%s %15u\n\r", "Store completed                ", pmc[cpu][3] );
#endif
    cpu++;
  }
  diag_printf("%s %15u\n\r", "Instructions L2 cache hit      ", pmlc2);
  diag_printf("%s %15u\n\r", "Instructions L2 cache miss     ", pmlc3);
  diag_printf("%s %15u\n\r", "Data L2 cache hit              ", pmlc4);
  diag_printf("%s %15u\n\r", "Data L2 cache miss             ", pmlc5);
  diag_printf("%s %15u\n\r", "Non-core read hit in L2        ", pmlc1);
  diag_printf("%s %15u\n\r", "Non-core read miss in L2       ", pmlc8);
  diag_printf("%s %15u\n\r", "Non-core burst write to L2     ", pmlc6);
  diag_printf("%s %15u\n\r", "Non-core non-burst write to L2 ", pmlc7);

  return;
}

#endif // CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_CACHE


#if defined(CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_IO)
/*
  * Here we configure the system events monitoring stuff
  * SE:Ref:0   -> Plaftorm clock cycles
  * SE:C1:80   -> ECM dispatch from core 0
  * SE:Ref:16  -> ECM data bus grant LBC
  * SE:Ref:26  -> PIC total interrupt count
  * SE:C7:79   -> ECM dispatch to DDR1
  * SE:C10:102 -> ECM dispatch to L2
  * SE:Ref:15  -> ECM total disptach
  * SE:C9:83   -> ECM dispatch to LBIU
  * SE:C5:81   -> ECM dispatch from eTSEC4
  * SE:Ref:17  -> ECM data bus grant eTSEC1-4
  */
static
void hal_setup_sys_mon_io( void )
{
  volatile struct sys_mon_85xx *sys_mon = \
            (volatile struct sys_mon_85xx *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_SYS_MON);

  /* Init counter*/
  sys_mon->pmlc0_hi = 0 ;
  sys_mon->pmlc0_lo = 0 ;
  sys_mon->pmlc1    = 0 ;
  sys_mon->pmlc2    = 0 ;
  sys_mon->pmlc3    = 0 ;
  sys_mon->pmlc4    = 0 ;
  sys_mon->pmlc5    = 0 ;
  sys_mon->pmlc6    = 0 ;
  sys_mon->pmlc7    = 0 ;
  sys_mon->pmlc8    = 0 ;
  sys_mon->pmlc9    = 0 ;  
  sys_mon->pmlc10   = 0 ;
  sys_mon->pmlc11   = 0 ;  

  /* Stop monitoring */
  sys_mon->pmgc0    = 0x80000000;

  /* Local Control Registers setting */
  sys_mon->pmlca0   = 0x04000000;
  sys_mon->pmlca1   = 0x04500000;
  sys_mon->pmlca2   = 0x04100000;
  sys_mon->pmlca3   = 0x041A0000;
  sys_mon->pmlca4   = 0x04110000;
  sys_mon->pmlca5   = 0x04510000;
  sys_mon->pmlca6   = 0x040F0000;
  sys_mon->pmlca7   = 0x044F0000;
  sys_mon->pmlca9   = 0x04530000;
  sys_mon->pmlca10  = 0x04660000;

  return;
}


/*
  * Display io monitoring registers
  *
  */
static
void hal_show_mpc8572_pm_io( void )
{
  volatile struct sys_mon_85xx *sys_mon = \
             (volatile struct sys_mon_85xx *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_SYS_MON);

  cyg_uint32 pmlc0_hi ;
  cyg_uint32 pmlc0_lo ;
  cyg_uint32 pmlc1 ;
  cyg_uint32 pmlc2 ;
  cyg_uint32 pmlc3 ;
  cyg_uint32 pmlc4 ;
  cyg_uint32 pmlc5 ;
  cyg_uint32 pmlc6 ;
  cyg_uint32 pmlc7 ;
  cyg_uint32 pmlc8 ;
  cyg_uint32 pmlc9 ;
  cyg_uint32 pmlc10 ;

  pmlc0_hi = sys_mon->pmlc0_hi ;
  pmlc0_lo = sys_mon->pmlc0_lo ;
  pmlc1    = sys_mon->pmlc1 ;
  pmlc2    = sys_mon->pmlc2 ;
  pmlc3    = sys_mon->pmlc3 ;
  pmlc4    = sys_mon->pmlc4 ;
  pmlc5    = sys_mon->pmlc5 ;
  pmlc6    = sys_mon->pmlc6 ;
  pmlc7    = sys_mon->pmlc7 ;
  pmlc8    = sys_mon->pmlc8 ;
  pmlc9    = sys_mon->pmlc9 ;
  pmlc10   = sys_mon->pmlc10 ;

  diag_printf("%s 0x%x%x\n\r", "Plaftorm clock cycles             ", pmlc0_hi, pmlc0_lo );
  diag_printf("%s %15u\n\r",   "ECM total dispatch            ", pmlc6 );
  diag_printf("%s %15u\n\r",   "PIC total interrupt count     ", pmlc3 );
  diag_printf("%s %15u\n\r",   "ECM dispatch to DDR1          ", pmlc7 );
  diag_printf("%s %15u\n\r",   "ECM dispatch to L2            ", pmlc10 );
  diag_printf("%s %15u\n\r",   "ECM dispatch to LBIU          ", pmlc9 );
  diag_printf("%s %15u\n\r",   "ECM dispatch from core 0      ", pmlc1 );
  diag_printf("%s %15u\n\r",   "ECM dispatch from eTSEC4      ", pmlc5 );
  diag_printf("%s %15u\n\r",   "ECM data bus grant LBC        ", pmlc2 );
  diag_printf("%s %15u\n\r",   "ECM data bus grant eTSEC      ", pmlc4 );

  return;
}
#endif // CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_IO



externC
void hal_show_mpc8572_pm( void )
{

#if defined(CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_CACHE)
  hal_show_mpc8572_pm_cache( );
#endif

#if defined(CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_IO)
  hal_show_mpc8572_pm_io( );
#endif

}


externC
void hal_setup_mpc8572_pm( void )
{

#if defined(CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_CACHE)
  hal_setup_sys_mon_cache( );
  hal_setup_core_mon_cache( );
#endif

#if defined(CYGSEM_HAL_POWERPC_MPC8572DS_MONITOR_IO)
  hal_setup_sys_mon_io( );
#endif

}


/*
 * Start counters
 */
externC
void hal_start_mpc8572_pm( void )
{
  volatile struct sys_mon_85xx *sys_mon = \
              (volatile struct sys_mon_85xx *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_SYS_MON);

  /* Start system monitoring */
  sys_mon->pmgc0 = 0x60000000;

  /* Start core monitoring */
  CYGARC_MTPMR( CYGARC_REG_PMGC0, 0x60000000 );

  return;
}

/*
 * Stop counters
 */
externC
void hal_stop_mpc8572_pm( void )
{
  volatile struct sys_mon_85xx *sys_mon = \
              (volatile struct sys_mon_85xx *) (CYGARC_IMM_BASE + CYGARC_REG_IMM_SYS_MON);

  /* Stop monitoring */
  sys_mon->pmgc0 = 0x80000000;

  CYGARC_MTPMR( CYGARC_REG_PMGC0, 0x80000000 );

  return;
}

static
void hal_mpc8572_pm_exception_handler( cyg_addrword_t data,
            cyg_code_t number,
            cyg_addrword_t info)
{
  HAL_SMP_CPU_TYPE cpu;
  cyg_uint32 pmc0 ;
  cyg_uint32 pmc1 ;
  cyg_uint32 pmc2 ;
  cyg_uint32 pmc3 ;

  cpu  = HAL_SMP_CPU_THIS();

  hal_stop_mpc8572_pm( );

  CYGARC_MFPMR( CYGARC_REG_PMC0 , pmc0 );
  CYGARC_MFPMR( CYGARC_REG_PMC1 , pmc1 );
  CYGARC_MFPMR( CYGARC_REG_PMC2 , pmc2 );
  CYGARC_MFPMR( CYGARC_REG_PMC3 , pmc3 ); 

  pmc[cpu][0] = pmc0 ;
  pmc[cpu][1] = pmc1 ;
  pmc[cpu][2] = pmc2 ;
  pmc[cpu][3] = pmc3 ;

  /* Place more user code here ... */
}


externC
void hal_mpc8572_pm_exception_register( void )
{
  cyg_exception_handler_t *handler;
  cyg_addrword_t data;

  cyg_exception_set_handler(
                  CYGNUM_HAL_VECTOR_PERFORMANCE_MON,
                  &hal_mpc8572_pm_exception_handler,
                  0,
                  &handler,
                  &data);
}

//--------------------------------------------------------------------------
// End of var_pm.c
