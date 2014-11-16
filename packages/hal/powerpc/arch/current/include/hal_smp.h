//==========================================================================
//
//      hal_smp.h
//
//      HAL SMP support to PowerPC architecture
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
// Author(s):    nickg
// Contributors: Updated for powerpc by ccoutand
// Date:         2009-11-01
// Purpose:      
// Description:  SMP support to PowerPC
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_HAL_SMP_H
#define CYGONCE_HAL_SMP_H

#include <pkgconf/hal.h>

#ifdef CYGPKG_HAL_SMP_SUPPORT

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/var_smp.h>

#define HAL_SMP_CPU_TYPE      cyg_uint32
#define HAL_SMP_CPU_MAX       CYGPKG_HAL_SMP_CPU_MAX
#define HAL_SMP_CPU_COUNT()   CYGPKG_HAL_SMP_CPU_MAX
#define HAL_SMP_CPU_NONE      (CYGPKG_HAL_SMP_CPU_MAX + 1)

#define HAL_SMP_CPU_THIS()        \
({                                \
  cyg_uint32 __id;                \
  asm volatile (                  \
         "mfspr  %0, %1;"         \
         : "=r" (__id)            \
         : "I" (CYGARC_REG_PIR)); \
  __id;                           \
})

// Defines the ID of the main CPU, the one which performs the initialization
// of the system

#ifndef CYGPKG_HAL_SMP_MAIN_CPU_ID
#define CYGPKG_HAL_SMP_MAIN_CPU_ID 0
#endif
	
//-----------------------------------------------------------------------------
// CPU startup

__externC void cyg_hal_cpu_release(HAL_SMP_CPU_TYPE cpu);

#define HAL_SMP_CPU_START( __cpu ) cyg_hal_cpu_release( __cpu )

//-----------------------------------------------------------------------------
// CPU message exchange

__externC void cyg_hal_cpu_message( HAL_SMP_CPU_TYPE cpu,
                                    CYG_WORD32 msg,
                                    CYG_WORD32 arg,
                                    CYG_WORD32 wait);

#define HAL_SMP_CPU_RESCHEDULE_INTERRUPT( __cpu, __wait ) \
         cyg_hal_cpu_message( __cpu, HAL_SMP_MESSAGE_RESCHEDULE, 0, __wait);

#define HAL_SMP_CPU_TIMESLICE_INTERRUPT( __cpu, __wait ) \
         cyg_hal_cpu_message( __cpu, HAL_SMP_MESSAGE_TIMESLICE, 0, __wait);


#define HAL_SMP_MESSAGE_TYPE            0xF0000000
#define HAL_SMP_MESSAGE_ARG             (~HAL_SMP_MESSAGE_TYPE)

#define HAL_SMP_MESSAGE_RESCHEDULE      0x10000000
#define HAL_SMP_MESSAGE_MASK            0x20000000
#define HAL_SMP_MESSAGE_UNMASK          0x30000000
#define HAL_SMP_MESSAGE_REVECTOR        0x40000000
#define HAL_SMP_MESSAGE_TIMESLICE       0x50000000


//-----------------------------------------------------------------------------
// Test-and-set support
// These macros provide test-and-set support for the least significant bit
// in a word.

#define HAL_TAS_TYPE    volatile CYG_WORD32

#define HAL_SPINLOCK_INIT_CLEAR 0x0

#define HAL_SPINLOCK_INIT_SET   0x1

// lwarx and stwcx. permit to achieve an atomic update
#define HAL_TAS_SET( _tas_, _oldb_ )                     \
CYG_MACRO_START                                          \
{                                                        \
    CYG_WORD32 __old;                                    \
    register CYG_WORD32 __set = HAL_SPINLOCK_INIT_SET;   \
    asm volatile (                                       \
      "1:;"                                              \
          "lwarx %0,0,%2;"                               \
          "cmpwi 0,%0,0;"                                \
          "bne- 2f;"                                     \
          "stwcx. %1,0,%2;"                              \
          "bne- 1b;"                                     \
      "2:;"                                              \
          : "=&r" (__old)                                \
          : "r" (__set), "r" (&_tas_)                    \
          : "cr0", "memory");                            \
    _oldb_ = (( __old & HAL_SPINLOCK_INIT_SET ) != 0);   \
}                                                        \
CYG_MACRO_END

#define HAL_TAS_CLEAR( _tas_, _oldb_ )                   \
CYG_MACRO_START                                          \
{                                                        \
    CYG_WORD32 __old;                                    \
    register CYG_WORD32 __set = HAL_SPINLOCK_INIT_CLEAR; \
    asm volatile (                                       \
      "1:;" \
          "lwarx %0,0,%2;"                               \
          "stwcx. %1,0,%2;"                              \
          "bne- 1b;"                                     \
          : "=&r" (__old)                                \
          : "r" (__set), "r" (&_tas_)                    \
          : "memory");                                   \
    _oldb_ = (( __old & HAL_SPINLOCK_INIT_SET ) != 0);   \
}                                                        \
CYG_MACRO_END

//-----------------------------------------------------------------------------
// Spinlock support.
// Built on top of test-and-set code.

#define HAL_SPINLOCK_TYPE       volatile CYG_WORD32

#define HAL_SPINLOCK_SPIN( _lock_ )             \
CYG_MACRO_START                                 \
{                                               \
    cyg_bool _val_;                             \
    do                                          \
    {                                           \
        HAL_TAS_SET( _lock_, _val_ );           \
    } while( _val_ );                           \
}                                               \
CYG_MACRO_END

#define HAL_SPINLOCK_CLEAR( _lock_ )            \
CYG_MACRO_START                                 \
{                                               \
    cyg_bool _val_;                             \
    HAL_TAS_CLEAR( _lock_ , _val_ );            \
}                                               \
CYG_MACRO_END

#define HAL_SPINLOCK_TRY( _lock_, _val_ )       \
    HAL_TAS_SET( _lock_, _val_ );               \
    (_val_) = ((_val_) == 0)

#define HAL_SPINLOCK_TEST( _lock_, _val_ )      \
    (_val_) = (((_lock_) & 0x1) != 0)

//-----------------------------------------------------------------------------
// Diagnostic output serialization

__externC HAL_SPINLOCK_TYPE cyg_hal_smp_diag_lock;

#define CYG_HAL_DIAG_LOCK_DATA_DEFN \
        HAL_SPINLOCK_TYPE cyg_hal_smp_diag_lock = HAL_SPINLOCK_INIT_CLEAR

#define CYG_HAL_DIAG_LOCK() HAL_SPINLOCK_SPIN( cyg_hal_smp_diag_lock )

#define CYG_HAL_DIAG_UNLOCK() HAL_SPINLOCK_CLEAR( cyg_hal_smp_diag_lock )

//-----------------------------------------------------------------------------
// Some extra definitions

__externC HAL_SPINLOCK_TYPE cyg_hal_smp_lock;

#endif // CYGPKG_HAL_SMP_SUPPORT

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_SMP_H
// End of hal_smp.h
