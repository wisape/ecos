//==========================================================================
//
//      var_cache.h
//
//      Variant HAL cache control API
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

#ifndef CYGONCE_VAR_CACHE_H
#define CYGONCE_VAR_CACHE_H

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/var_io.h>
#include <cyg/hal/plf_cache.h>


//-----------------------------------------------------------------------------
// Global control of data cache
//

#ifdef CYGSEM_HAL_PLF_DCACHE_ENABLE

//-----------------------------------------------------------------------------
// Cache dimensions

#if !defined(HAL_PLF_DCACHE_SIZE) || !defined(HAL_PLF_DCACHE_LINE_SIZE) \
        || !defined(HAL_PLF_DCACHE_BASE)
#error Data cache geometry information missing
#endif

// Data cache
#define HAL_DCACHE_SIZE                 HAL_PLF_DCACHE_SIZE         // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            HAL_PLF_DCACHE_LINE_SIZE    // Size of a data cache line
#define HAL_DCACHE_BASE                 HAL_PLF_DCACHE_BASE         // Base address of a cacheable area

// Invalidate the entire cache. Cache is invalidated using physical address
// HAL_DCACHE_SIZE-HAL_DCACHE_LINE_SIZE down to 0. It is not required for this
// memory area to be cacheable, all cache lines will be invalidated anyway.
#define HAL_DCACHE_INVALIDATE_ALL()                             \
    CYG_MACRO_START                                             \
    register cyg_uint32 c_size =                                \
                     HAL_DCACHE_SIZE - HAL_DCACHE_LINE_SIZE;    \
    asm volatile (                                              \
        "1:"                                                    \
           "blti  %0, 2f;"                                      \
           "wdc   %0, r0;"                                      \
           "brid  1b;"                                          \
           "addik %0, %0, -%1;"                                 \
        "2:"                                                    \
        :                                                       \
        : "r" (c_size),                                         \
          "I" (HAL_DCACHE_LINE_SIZE) );                         \
    CYG_MACRO_END

// Enable the data cache
#define HAL_DCACHE_ENABLE()                                     \
    CYG_MACRO_START                                             \
    register cyg_uint32 tmp = 0;                                \
    asm volatile (                                              \
        "mfs   %0, rmsr;"                                       \
        "ori   %0, %0, %1;"                                     \
        "mts   rmsr, %0;"                                       \
        :                                                       \
        : "r" (tmp),                                            \
          "I" (CYGARC_REG_MSR_DCE) );                           \
    CYG_MACRO_END

// Disable the data cache
#define HAL_DCACHE_DISABLE()                                    \
    CYG_MACRO_START                                             \
    register cyg_uint32 tmp = 0;                                \
    asm volatile (                                              \
        "mfs   %0, rmsr;"                                       \
        "andi  %0, %0, %1;"                                     \
        "mts   rmsr, %0;"                                       \
        :                                                       \
        : "r" (tmp),                                            \
          "I" (~CYGARC_REG_MSR_DCE) );                          \
    CYG_MACRO_END

// Synchronize the contents of the cache with memory.
// This should be changed to a wdc.flush?
#define HAL_DCACHE_SYNC()                                           \
    CYG_MACRO_START                                                 \
    register cyg_uint32 tmp = 0;                                    \
    register cyg_int32 i;                                           \
    register cyg_uint32 *__base =                                   \
                      (cyg_uint32 *) (HAL_PLF_DCACHE_BASE);         \
    for(i=0; i< (2 * HAL_DCACHE_SIZE/HAL_DCACHE_LINE_SIZE);         \
                            i++, __base += HAL_DCACHE_LINE_SIZE/4){ \
        asm volatile ("lwi %1, %0, 0;"::"r"(__base), "r" (tmp) );   \
    }                                                               \
    CYG_MACRO_END

// Query the state of the data cache
#define HAL_DCACHE_IS_ENABLED(_state_)                          \
    asm volatile ("mfs    %0, rmsr;"                            \
                  "andi   %0, %0, %1;"                          \
                  : "=r" (_state_) : "I" (CYGARC_REG_MSR_DCE))

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
#define HAL_DCACHE_FLUSH( _base_ , _size_ )                     \
    CYG_MACRO_START                                             \
    register cyg_uint32 __base = (cyg_uint32) (_base_);         \
    register cyg_int32 __size = (cyg_int32) (_size_);           \
    while (__size > 0) {                                        \
        asm volatile ("wdc.flush %0, r0" : : "r" (__base));     \
        __base += HAL_DCACHE_LINE_SIZE;                         \
        __size -= HAL_DCACHE_LINE_SIZE;                         \
    }                                                           \
    CYG_MACRO_END

// Invalidate cache lines in the given range without writing to memory.
#define HAL_DCACHE_INVALIDATE( _base_ , _size_ )                \
    CYG_MACRO_START                                             \
    register cyg_uint32 __base = (cyg_uint32) (_base_);         \
    register cyg_int32 __size = (cyg_int32) (_size_);           \
    while (__size > 0) {                                        \
        asm volatile ("wdc 0,%0;" : : "r" (__base));            \
        __base += HAL_DCACHE_LINE_SIZE;                         \
        __size -= HAL_DCACHE_LINE_SIZE;                         \
    }                                                           \
    CYG_MACRO_END

#else

// Enable the data cache
#define HAL_DCACHE_ENABLE()

// Disable the data cache
#define HAL_DCACHE_DISABLE()

// Invalidate the entire cache
#define HAL_DCACHE_INVALIDATE_ALL()

// Synchronize the contents of the cache with memory.
#define HAL_DCACHE_SYNC()

// Purge contents of data cache
#define HAL_DCACHE_PURGE_ALL()

// Query the state of the data cache (does not affect the caching)
#define HAL_DCACHE_IS_ENABLED(_state_)          \
    CYG_MACRO_START                             \
    (_state_) = 0;                              \
    CYG_MACRO_END

#endif // CYGSEM_HAL_PLF_DCACHE_ENABLE


//-----------------------------------------------------------------------------
// Global control of Instruction cache
//

#ifdef CYGSEM_HAL_PLF_ICACHE_ENABLE

#if !defined(HAL_PLF_ICACHE_SIZE) || !defined(HAL_PLF_ICACHE_LINE_SIZE)
#error Instuction cache geometry information missing
#endif

// Instruction cache
#define HAL_ICACHE_SIZE                 HAL_PLF_ICACHE_SIZE         // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            HAL_PLF_ICACHE_LINE_SIZE    // Size of a cache line

// Invalidate the entire cache. Cache is invalidated using physical address
// HAL_DCACHE_SIZE-HAL_DCACHE_LINE_SIZE down to 0. It is not required for this
// memory area to be cacheable, all cache lines will be invalidated anyway.
#define HAL_ICACHE_INVALIDATE_ALL()                             \
    CYG_MACRO_START                                             \
    register cyg_uint32 i_size =                                \
                     HAL_ICACHE_SIZE - HAL_ICACHE_LINE_SIZE;    \
    asm volatile (                                              \
        "1:"                                                    \
           "blti  %0, 2f;"                                      \
           "wic   %0, r0;"                                      \
           "brid  1b;"                                          \
           "addik %0, %0, -%1;"                                 \
        "2:"                                                    \
        :                                                       \
        : "r" (i_size),                                         \
          "I" (HAL_ICACHE_LINE_SIZE) );                         \
    CYG_MACRO_END

// Enable the instruction cache
#define HAL_ICACHE_ENABLE()                                     \
    CYG_MACRO_START                                             \
    register cyg_uint32 tmp= 0;                                 \
    asm volatile (                                              \
        "mfs   %0, rmsr;"                                       \
        "ori   %0, %0, %1;"                                     \
        "mts   rmsr, %0;"                                       \
        :                                                       \
        : "r" (tmp),                                            \
          "I" (CYGARC_REG_MSR_ICE) );                           \
    CYG_MACRO_END


// Disable the instruction cache
#define HAL_ICACHE_DISABLE()                                    \
    CYG_MACRO_START                                             \
    register cyg_uint32 tmp= 0;                                 \
    asm volatile (                                              \
        "mfs   %0, rmsr;"                                       \
        "andi  %0, %0, %1;"                                     \
        "mts   rmsr, %0;"                                       \
        :                                                       \
        : "r" (tmp),                                            \
          "I" (~CYGARC_REG_MSR_ICE) );                          \
    CYG_MACRO_END


// Synchronize the contents of the cache with memory.
#define HAL_ICACHE_SYNC()                                       \
    HAL_ICACHE_INVALIDATE_ALL()


// Query the state of the instruction cache
#define HAL_ICACHE_IS_ENABLED(_state_)                          \
    asm volatile ("mfs    %0, rmsr;"                            \
                  "andi   %0, %0, %1;"                          \
                  : "=r" (_state_) : "I" (CYGARC_REG_MSR_ICE))

#else

// Enable the instruction cache
#define HAL_ICACHE_ENABLE()

// Disable the instruction cache
#define HAL_ICACHE_DISABLE()

// Invalidate the entire cache
#define HAL_ICACHE_INVALIDATE_ALL()

// Synchronize the contents of the cache with memory.
#define HAL_ICACHE_SYNC()

// Query the state of the instruction cache (does not affect the caching)
#define HAL_ICACHE_IS_ENABLED(_state_)          \
    CYG_MACRO_START                             \
    (_state_) = 0;                              \
    CYG_MACRO_END

#endif // CYGSEM_HAL_PLF_ICACHE_ENABLE


#endif // ifndef CYGONCE_VAR_CACHE_H
// End of var_cache.h
