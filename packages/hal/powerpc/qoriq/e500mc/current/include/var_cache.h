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
// Description:  Variant HAL cache control API for Freescale QorIQ target
//               HAL_FLASH_CACHES_OFF and HAL_FLASH_CACHES_ON not tested
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_VAR_CACHE_H
#define CYGONCE_VAR_CACHE_H

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
 
#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/ppc_regs.h>
#include <cyg/hal/var_regs.h>

#include <cyg/hal/plf_cache.h>

//-----------------------------------------------------------------------------
// Cache dimensions

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P2040) ||   \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P2041) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P3040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P3041) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4080)
// Data cache
#define HAL_DCACHE_SIZE                 32768    // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            64       // Size of a data cache line
#define HAL_DCACHE_WAYS                 8        // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 32768    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            64       // Size of a cache line
#define HAL_ICACHE_WAYS                 8        // Associativity of the cache
#else
# error "Don't know the QorIQ cache size"
#endif

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

// Use the beginning of the text section for flushing the data cache
#ifdef CYG_HAL_STARTUP_ROM
# define FLUSH_START_ADDRESS (0)
#else
__externC cyg_uint32 _start;
# define FLUSH_START_ADDRESS ((cyg_uint32) &_start)
#endif

//-----------------------------------------------------------------------------
// Global control of data cache

// HAL_DCACHE_FLUSH_ALL() must be run with interrupt disable!!
// As described in e500 core user manual
//    First save HID0 (in spr_hid0) and enable data cache flush assist (HID0_CFA)
//    Perform read to 48Kbyte region, then execute dcbf instructions to that region.
//    Restore HID0. (52Kbyte region seems necessary to have reliable data cache flush)

#define HAL_DCACHE_FULL_FLUSH_SIZE (52 * 1024)

#define HAL_DCACHE_FLUSH_ASSIST_ENABLE( _old_ )       \
    CYG_MACRO_START                                   \
    register cyg_uint32 _spr_hid0 = 0;                \
    asm volatile ("msync;"                            \
                  "isync;"                            \
                  "mfspr %1, %2;"                     \
                  "mr    %0, %1;"                     \
                  "ori   %1, %1, %3;"                 \
                  "msync;"                            \
                  "isync;"                            \
                  "mtspr %2, %1;"                     \
                  "msync;"                            \
                  "isync;"                            \
                  : "+r" (_old_),                     \
                    "+r" (_spr_hid0)                  \
                  : "I" (CYGARC_REG_HID0),            \
                    "I" (CYGARC_REG_HID0_DCFA)        \
                  );                                  \
    CYG_MACRO_END
      
#define HAL_DCACHE_FLUSH_ASSIST_RESTORE( _old_ )      \
    CYG_MACRO_START                                   \
    asm volatile ("msync;"                            \
                  "isync;"                            \
                  "mtspr %1,  %0;"                    \
                  "isync;"                            \
                  :                                   \
                  : "r" (_old_),                      \
                    "I" (CYGARC_REG_HID0)             \
                  );                                  \
    CYG_MACRO_END


#define HAL_DCACHE_FLUSH_ALL()                            \
    CYG_MACRO_START                                       \
    register cyg_uint32 _spr_    = 0;                     \
    register cyg_uint32 _base_   = FLUSH_START_ADDRESS;   \
    register cyg_uint32 _reg_    = (HAL_DCACHE_FULL_FLUSH_SIZE / HAL_DCACHE_LINE_SIZE); \
    HAL_DCACHE_FLUSH_ASSIST_ENABLE( _spr_ );              \
    asm volatile (                                        \
                  "mtctr   %1;"                           \
               "1: lwz     %1,0(%0);"                     \
                  "addi    %0,%0,(%3);"                   \
                  "bdnz+   1b;"                           \
                  "msync;"                                \
                  "li      %1,%2;"                        \
                  "mtctr   %1;"                           \
                  "li      %0,0;"                         \
                  "li      %1,%3;"                        \
               "2: dcbf    %1,%0;"                        \
                  "addi    %0,%0,(%3);"                   \
                  "bdnz+   2b;"                           \
                  :                                       \
                  : "r" (_base_), "r" (_reg_),            \
                    "I" (HAL_DCACHE_FULL_FLUSH_SIZE / HAL_DCACHE_LINE_SIZE), \
                    "I" (HAL_DCACHE_LINE_SIZE),           \
                    "I" (FLUSH_START_ADDRESS)             \
                  : "ctr"                                 \
                  );                                      \
    HAL_DCACHE_FLUSH_ASSIST_RESTORE( _spr_ );             \
    CYG_MACRO_END


// Invalidate the entire cache
#define HAL_DCACHE_INVALIDATE_ALL()                   \
    CYG_MACRO_START                                   \
    register cyg_uint32 tmp = 0;                      \
    asm volatile ("msync;"                            \
                  "mfspr %0, %1;"                     \
                  "ori   %0, %0, %2;"                 \
                  "msync;"                            \
                  "isync;"                            \
                  "mtspr %1, %0;"                     \
                  "isync;"                            \
                  :                                   \
                  : "r" (tmp), "I" (CYGARC_REG_L1CSR0), \
                    "I" (CYGARC_REG_L1CSR0_CFI) /* %1 ==> L1CSR0 */);\
    CYG_MACRO_END

// Enable the data cache
#define HAL_DCACHE_ENABLE()                 \
    CYG_MACRO_START                         \
    register cyg_uint32 tmp = 0;            \
    asm volatile (                          \
        "msync;"                            \
        "mfspr %0, %1;"                     \
        "ori   %0, %0, %2;"                 \
        "oris  %0, %0, %3;"                 \
        "msync;"                            \
        "isync;"                            \
        "mtspr %1,%0;"                      \
        "isync;"                            \
        :                                   \
        : "r" (tmp), "I" (CYGARC_REG_L1CSR0), \
          "I" (CYGARC_REG_L1CSR0_CE),         \
          "I" (CYGARC_REG_L1CSR0_CPE>>16) /* %1 ==> L1CSR0 */); \
    CYG_MACRO_END

// Disable the data cache
#define HAL_DCACHE_DISABLE()                \
    CYG_MACRO_START                         \
    register cyg_uint32 tmp1 = 0, tmp2 = 0; \
    asm volatile (                          \
        "msync;"                            \
        "mfspr %0, %2;"                     \
        "li    %1, 0x0;"                    \
        "ori   %1, %1, %3;"                 \
        "andc  %0, %0, %1;"                 \
        "msync;"                            \
        "isync;"                            \
        "mtspr %2, %0;"                     \
        "isync;"                            \
        :                                   \
        : "r" (tmp1), "r" (tmp2),           \
          "I" (CYGARC_REG_L1CSR0), "I" (CYGARC_REG_L1CSR0_CE));   \
    CYG_MACRO_END

// Synchronize the contents of the cache with memory.
#define HAL_DCACHE_SYNC()                             \
    CYG_MACRO_START                                   \
    register cyg_uint32 _base_   = FLUSH_START_ADDRESS;                                 \
    register cyg_uint32 _reg_    = (HAL_DCACHE_FULL_FLUSH_SIZE / HAL_DCACHE_LINE_SIZE); \
    asm volatile ("msync;"                            \
                  "mtctr   %1;"                       \
               "1: lwz     %1,0(%0);"                 \
                  "addi    %0,%0,%2;"                 \
                  "bdnz+   1b;"                       \
                  "msync;"                            \
                  :                                   \
                  : "r" (_base_), "r" (_reg_),        \
                    "I" (HAL_DCACHE_LINE_SIZE)        \
                  : "ctr"                             \
                  );                                  \
    CYG_MACRO_END

// Query the state of the data cache
#define HAL_DCACHE_IS_ENABLED(_state_)                          \
    asm volatile ("msync;"                                      \
                  "mfspr  %0, %1;"                              \
                  "andi.  %0, %0, %2;"                          \
                  : "=r" (_state_) : "I" (CYGARC_REG_L1CSR0), "I" (CYGARC_REG_L1CSR0_CE))

//-----------------------------------------------------------------------------
// Data cache line control

// Allocate cache lines for the given address range without reading its
// contents from memory.
//#define HAL_DCACHE_ALLOCATE( _base_ , _size_ )

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
#define HAL_DCACHE_FLUSH( _base_ , _size_ )                     \
    CYG_MACRO_START                                             \
    cyg_uint32 __base = (cyg_uint32) (_base_);                  \
    cyg_int32 __size = (cyg_int32) (_size_);                    \
    while (__size > 0) {                                        \
        asm volatile ("dcbf 0,%0;sync;" : : "r" (__base));      \
        __base += HAL_DCACHE_LINE_SIZE;                         \
        __size -= HAL_DCACHE_LINE_SIZE;                         \
    }                                                           \
    CYG_MACRO_END

// Invalidate cache lines in the given range without writing to memory.
// NOTE: The errata for the 603e processor indicates use of the dcbf
// command as the dcbi command will only invalidate modified blocks.
#define HAL_DCACHE_INVALIDATE( _base_ , _size_ )                \
    CYG_MACRO_START                                             \
    cyg_uint32 __base = (cyg_uint32) (_base_);                  \
    cyg_int32 __size = (cyg_int32) (_size_);                    \
    while (__size > 0) {                                        \
        asm volatile ("dcbf 0,%0;sync;" : : "r" (__base));      \
        __base += HAL_DCACHE_LINE_SIZE;                         \
        __size -= HAL_DCACHE_LINE_SIZE;                         \
    }                                                           \
    CYG_MACRO_END

// Write dirty cache lines to memory for the given address range.
#define HAL_DCACHE_STORE( _base_ , _size_ )                     \
    CYG_MACRO_START                                             \
    cyg_uint32 __base = (cyg_uint32) (_base_);                  \
    cyg_int32 __size = (cyg_int32) (_size_);                    \
    while (__size > 0) {                                        \
        asm volatile ("dcbst 0,%0;sync;" : : "r" (__base));     \
        __base += HAL_DCACHE_LINE_SIZE;                         \
        __size -= HAL_DCACHE_LINE_SIZE;                         \
    }                                                           \
    CYG_MACRO_END

//-----------------------------------------------------------------------------
// Global control of Instruction cache
// Invalidate the entire cache
#define HAL_ICACHE_INVALIDATE_ALL()                   \
    CYG_MACRO_START                                   \
    register cyg_uint32 tmp = 0;                      \
    asm volatile ("msync;"                            \
                  "mfspr %0, %1;"                     \
                  "ori   %0, %0, %2;"                 \
                  "msync;"                            \
                  "isync;"                            \
                  "mtspr %1, %0;"                     \
                  "isync;"                            \
                  :                                   \
                  : "r" (tmp), "I" (CYGARC_REG_L1CSR1), \
                    "I" (CYGARC_REG_L1CSR1_ICFI) /* %1 ==> L1CSR1 */);\
    CYG_MACRO_END

// Enable the instruction cache
#define HAL_ICACHE_ENABLE()                 \
    CYG_MACRO_START                         \
    register cyg_uint32 tmp = 0;            \
    asm volatile (                          \
        "msync;"                            \
        "mfspr %0, %1;"                     \
        "ori   %0, %0, %2;"                 \
        "oris  %0, %0, %3;"                 \
        "msync;"                            \
        "isync;"                            \
        "mtspr %1,%0;"                      \
        "isync;"                            \
        :                                   \
        : "r" (tmp), "I" (CYGARC_REG_L1CSR1),  \
          "I" (CYGARC_REG_L1CSR1_ICE),         \
          "I" (CYGARC_REG_L1CSR1_ICECE>>16) /* %1 ==> L1CSR1 */); \
    CYG_MACRO_END

// Disable the instruction cache
#define HAL_ICACHE_DISABLE()                \
    CYG_MACRO_START                         \
    register cyg_uint32 tmp1 = 0, tmp2 = 0; \
    asm volatile (                          \
        "msync;"                            \
        "mfspr %0, %2;"                     \
        "li    %1, 0x0;"                    \
        "ori   %1, %1, %3;"                 \
        "andc  %0, %0, %1;"                 \
        "msync;"                            \
        "isync;"                            \
        "mtspr %2, %0;"                     \
        "isync;"                            \
        :                                   \
        : "r" (tmp1), "r" (tmp2),           \
          "I" (CYGARC_REG_L1CSR1), "I" (CYGARC_REG_L1CSR1_ICE));  \
    CYG_MACRO_END


// Synchronize the contents of the cache with memory.
#define HAL_ICACHE_SYNC()                             \
    HAL_ICACHE_INVALIDATE_ALL()


// Query the state of the instruction cache
#define HAL_ICACHE_IS_ENABLED(_state_)                          \
    asm volatile ("msync;"                                      \
                  "mfspr  %0, %1;"                              \
                  "andi.  %0, %0, %2;"                          \
                  : "=r" (_state_) : "I" (CYGARC_REG_L1CSR1), "I" (CYGARC_REG_L1CSR1_ICE))

#endif // ifndef CYGONCE_VAR_CACHE_H
// End of var_cache.h
