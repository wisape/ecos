#ifndef CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_INL
#define CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_INL
//==========================================================================
//
//      fman.inl
//
//      Freescale Datapath Frame Manager
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 2013 Free Software Foundation, Inc. 
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
// Date:         2013-01-30
// Purpose:      
// Description:  
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN1

//
// Fill Frame Manager Configuration structure

static struct fman_cfg fman1_cfg = {
  .bmi = {
    .pool_size = CYGNUM_HAL_POWERPC_FREESCALE_DPAA_FMAN1_BUFFER_POOL_SIZE
  },
  .qmi = {
    .tbd = 0
  },
  .fpm = {
    .tbd = 0
  },
  .dma = {
    .tbd = 0
  },
};

//
// Fill Frame Manager Info structure

static struct fman_t fman1 = { 
  .id         = FMAN1_INDEX,
#ifndef CYGNUM_HAL_POWERPC_FREESCALE_DPAA_FMAN_FIRM_ADDR
  .pmicrocode = (volatile cyg_uint32 *) &FmanMicrocode,
#else
  .pmicrocode = (volatile cyg_uint32 *) CYGNUM_HAL_POWERPC_FREESCALE_DPAA_FMAN_FIRM_ADDR,
#endif
  .imem_pool  = {
      .size = CYGNUM_FREESCALE_DPAA_FMAN_MEM_SIZE,
      .base = CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1,
   },
  .cfg        = &fman1_cfg,
  .imem       = CYGNUM_REG_IMM_DPAA_FMAN_IMEM_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .ccd        = CYGNUM_REG_IMM_DPAA_FMAN_CCD_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .fpm        = CYGNUM_REG_IMM_DPAA_FMAN_FPM_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .dma        = CYGNUM_REG_IMM_DPAA_FMAN_DMA_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .bmi_com    = CYGNUM_REG_IMM_DPAA_FMAN_BMI_COM_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .qmi_com    = CYGNUM_REG_IMM_DPAA_FMAN_QMI_COM_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .port       = CYGNUM_REG_IMM_DPAA_FMAN_PORT_BASE(CYGARC_REG_IMM_DPAA_FMAN1),
  .state      = FMAN_INIT_PENDING
};

#endif // CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN1

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN2

//
// Fill Frame Manager Configuration structure

static struct fman_cfg fman2_cfg = {
  .bmi = {
    .pool_size = CYGNUM_HAL_POWERPC_FREESCALE_DPAA_FMAN2_BUFFER_POOL_SIZE
  },
  .qmi = {
    .tbd = 0
  },
  .fpm = {
    .tbd = 0
  },
  .dma = {
    .tbd = 0
  },
};

//
// Fill Frame Manager Info structure

static struct fman_t fman2 = { 
  .id         = FMAN2_INDEX,
#ifndef CYGNUM_HAL_POWERPC_FREESCALE_DPAA_FMAN_FIRM_ADDR
  .pmicrocode = (volatile cyg_uint32 *) &FmanMicrocode,
#else
  .pmicrocode = (volatile cyg_uint32 *) CYGNUM_HAL_POWERPC_FREESCALE_DPAA_FMAN_FIRM_ADDR,
#endif
  .imem_pool  = {
      .size = CYGNUM_FREESCALE_DPAA_FMAN_MEM_SIZE,
      .base = CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN2,
   },
  .cfg        = &fman2_cfg,
  .imem       = CYGNUM_REG_IMM_DPAA_FMAN_IMEM_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .ccd        = CYGNUM_REG_IMM_DPAA_FMAN_CCD_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .fpm        = CYGNUM_REG_IMM_DPAA_FMAN_FPM_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .dma        = CYGNUM_REG_IMM_DPAA_FMAN_DMA_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .bmi_com    = CYGNUM_REG_IMM_DPAA_FMAN_BMI_COM_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .qmi_com    = CYGNUM_REG_IMM_DPAA_FMAN_QMI_COM_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .port       = CYGNUM_REG_IMM_DPAA_FMAN_PORT_BASE(CYGARC_REG_IMM_DPAA_FMAN2),
  .state      = FMAN_INIT_PENDING
};

#endif // CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN2

#endif // CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_INL