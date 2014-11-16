//==========================================================================
//
//      var_type.h
//
//      Variant type definition
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 
// 2008, 2009 Free Software Foundation, Inc. 
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
// Description:  Variant type definition for Freescale Quicc III
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_VAR_QUICC3_TYPE_H
#define CYGONCE_VAR_QUICC3_TYPE_H

#define OFFSET_16_BYTES (4 * 4)
#define OFFSET_32_BYTES (8 * 4)
#define OFFSET_64_BYTES (16 *4)

#ifndef CYGPKG_HAL_SMP_CPU_MAX
#define CYGPKG_HAL_SMP_CPU_MAX 1
#endif

typedef struct
{
   volatile cyg_uint32 *vpr;   // Base @ of the VPR (Vector/Priority) register
   volatile cyg_uint32 *idr;   // Base @ of the IDR (Interrupt Dispatch) register
} irq_type;

typedef struct
{
   volatile cyg_uint32 *vpr;                                               // Base @ of the VPR (Vector/Priority) register
   volatile cyg_uint32 *idr[CYGPKG_HAL_SMP_CPU_MAX];   // Base @ of the IDR (Interrupt Dispatch) register
} irq_ipi_type;

typedef struct
{
   volatile cyg_uint32 *eoi;
   volatile cyg_uint32 *whoami;
   volatile cyg_uint32 *iack;
} irq_mp_type;

typedef struct
{
   irq_type     irq[CYGNUM_HAL_INTERRUPT_IPI0];
   irq_ipi_type irq_ipi[CYGARC_REG_PIC_IPI_IRQ_NUM];
   irq_mp_type  irq_mp[CYGPKG_HAL_SMP_CPU_MAX];
} pic_info;

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_VAR_QUICC3_TYPE_H
// End of var_type.h
