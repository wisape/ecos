#ifndef CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_IMEM_INL
#define CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_IMEM_INL
//==========================================================================
//
//      fman_imem.inl
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


static void
fman_imem_init(struct imem_pool_t *pool, cyg_uint8 im){
  //
  // Clear FMAN internal memory
  memset((void *)pool->base, 0, pool->size);

  // In Independent mode, the BMI internal buffer allocation must not begin
  // from offset 0 so we skip the first 256 bytes
  if( im )
    pool->next_buf = pool->base + FMAN_FMBM_BUFFER_SIZE*FMAN_FMBM_IMEM_RES_BLOCK;
  else
    pool->next_buf = pool->base;
}

//
// This routine allocate memory from the FMAN internal memory given a required
// memory alignment. This is a first implementation, only allocation is allowed.
// An implementation requiring release of memory will require the use of a link
// list to keep track of allocated and free memory. This could be implemented in
// the future
//

externC cyg_uint32
fman_imem_alloc(struct imem_pool_t *pool, cyg_uint32 size, cyg_uint32 alignment){
  cyg_uint32 n    = alignment;
  cyg_uint32 mask = alignment - 1;
  cyg_uint32 new_buff = pool->next_buf;

  // Verify the alignment is power of two
  if(n==0 || n&(n-1)){
    CYG_ASSERT((n==0 || n&(n-1)), "Invalid memory alignment requested");
    return 0;
  }

#ifdef CYGPKG_KERNEL
  cyg_interrupt_disable();
#endif

  // Align memory
  if(pool->next_buf & mask){ 
    new_buff = (pool->next_buf + alignment) & ~mask;
  }

  // Check out-of-memory condition
  if ((new_buff + size) >= (pool->base + pool->size))
    return 0;

  pool->next_buf = new_buff + size;

#ifdef CYGPKG_KERNEL
  cyg_interrupt_enable();
#endif

  return new_buff;
}

#endif