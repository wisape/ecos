//==========================================================================
//
//      plf_cache.h
//
//      Platform cache control API
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2011 Free Software Foundation, Inc.
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
// Date:         2011-11-25
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_PLF_CACHE_H
#define CYGONCE_PLF_CACHE_H

#include <pkgconf/hal.h>

#ifdef CYGSEM_HAL_PLF_DCACHE_ENABLE

# define HAL_PLF_DCACHE_SIZE                 CYGNUM_HAL_DCACHE_SIZE
# define HAL_PLF_DCACHE_LINE_SIZE            CYGNUM_HAL_DCACHE_LINE_SIZE
# define HAL_PLF_DCACHE_BASE                 CYGNUM_HAL_DCACHE_CACHEABLE_BASE_ADDRESS

#endif

#ifdef CYGSEM_HAL_PLF_ICACHE_ENABLE

# define HAL_PLF_ICACHE_SIZE                 CYGNUM_HAL_ICACHE_SIZE
# define HAL_PLF_ICACHE_LINE_SIZE            CYGNUM_HAL_ICACHE_LINE_SIZE

#endif

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_PLF_CACHE_H
// End of plf_cache.h

