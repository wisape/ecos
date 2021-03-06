#ifndef CYGONCE_PLF_CACHE_H
#define CYGONCE_PLF_CACHE_H

//=============================================================================
//
//      plf_cache.h
//
//      Platform HAL cache details
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2002 Gary Thomas
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   jskov
// Contributors:jskov
// Date:        2000-01-26
// Purpose:     Platform cache control API
// Description: The macros defined here provide the platform specific
//              cache control operations / behavior.
// Usage:       Is included via the architecture cache header:
//              #include <cyg/hal/hal_cache.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================


//---------------------------------------------------------------------------
// Initial cache enabling
#if 1
#ifdef CYGSEM_HAL_ROM_MONITOR
# define CYGPRI_INIT_CACHES
# define CYGPRI_ENABLE_CACHES
#endif
#else
// Disable caches for now
# define CYGPRI_INIT_CACHES
# define CYGPRI_ENABLE_CACHES
#endif

//---------------------------------------------------------------------------
// No cache for the flash
#define HAL_FLASH_CACHES_OFF(_d_, _i_)    \
    CYG_MACRO_START                       \
    CYG_MACRO_END

#define HAL_FLASH_CACHES_ON(_d_, _i_)     \
    CYG_MACRO_START                       \
    CYG_MACRO_END


#define HAL_FLASH_CACHES_STATE(_d_, _i_) int _d_, _i_

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_PLF_CACHE_H
// End of plf_cache.h
