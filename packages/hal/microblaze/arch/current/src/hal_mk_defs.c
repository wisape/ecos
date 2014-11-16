//==========================================================================
//
//      hal_mk_defs.c
//
//      HAL (architecture) "make defs" program
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
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
// Author(s):    gthomas
// Contributors: gthomas, jskov
// Date:         2000-02-21
// Purpose:      PowerPC architecture dependent definition generator
// Description:  This file contains code that can be compiled by the target
//               compiler and used to generate machine specific definitions
//               suitable for use in assembly code.
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include CYGHWR_MEMORY_LAYOUT_H     

#include <cyg/hal/hal_arch.h>           // HAL header
#include <cyg/hal/hal_intr.h>           // HAL header
#ifdef CYGPKG_KERNEL
# include <pkgconf/kernel.h>
# include <cyg/kernel/instrmnt.h>
#endif

/*
 * This program is used to generate definitions needed by
 * assembly language modules.
 *
 * This technique was first used in the OSF Mach kernel code:
 * generate asm statements containing #defines,
 * compile this file to assembler, and then extract the
 * #defines from the assembly-language output.
 */

#define DEFINE(sym, val) \
        asm volatile("\n\t.equ\t" #sym ",%0" : : "i" (val))

int
main(void)
{

    DEFINE(CYGARC_JMPBUF_SP, offsetof(hal_jmp_buf_t, sp));
    DEFINE(CYGARC_JMPBUF_R2, offsetof(hal_jmp_buf_t, r2));
    DEFINE(CYGARC_JMPBUF_R13, offsetof(hal_jmp_buf_t, r13));
    DEFINE(CYGARC_JMPBUF_R19, offsetof(hal_jmp_buf_t, r19));
    DEFINE(CYGARC_JMPBUF_R20, offsetof(hal_jmp_buf_t, r20));
    DEFINE(CYGARC_JMPBUF_R21, offsetof(hal_jmp_buf_t, r21));
    DEFINE(CYGARC_JMPBUF_R22, offsetof(hal_jmp_buf_t, r22));
    DEFINE(CYGARC_JMPBUF_R23, offsetof(hal_jmp_buf_t, r23));
    DEFINE(CYGARC_JMPBUF_R24, offsetof(hal_jmp_buf_t, r24));
    DEFINE(CYGARC_JMPBUF_R25, offsetof(hal_jmp_buf_t, r25));
    DEFINE(CYGARC_JMPBUF_R26, offsetof(hal_jmp_buf_t, r26));
    DEFINE(CYGARC_JMPBUF_R27, offsetof(hal_jmp_buf_t, r27));
    DEFINE(CYGARC_JMPBUF_R28, offsetof(hal_jmp_buf_t, r28));
    DEFINE(CYGARC_JMPBUF_R29, offsetof(hal_jmp_buf_t, r29));
    DEFINE(CYGARC_JMPBUF_R30, offsetof(hal_jmp_buf_t, r30));
    DEFINE(CYGARC_JMPBUF_R31, offsetof(hal_jmp_buf_t, r31));
    DEFINE(CYGARC_JMPBUF_LR, offsetof(hal_jmp_buf_t, lr));

    DEFINE(CYGARC_MBREG_REGS, offsetof(HAL_SavedRegisters, d[0]));
    DEFINE(CYGARC_MBREG_MSR, offsetof(HAL_SavedRegisters, msr));
    DEFINE(CYGARC_MBREG_PC, offsetof(HAL_SavedRegisters, pc));
    DEFINE(CYGARC_MBREG_LR, offsetof(HAL_SavedRegisters, lr));
    DEFINE(CYGARC_MB_CONTEXT_SIZE, offsetof(HAL_SavedRegisters, context_size));

    // Below only saved on exceptions/interrupts
    DEFINE(CYGARC_MBREG_VECTOR, offsetof(HAL_SavedRegisters, vector));
    DEFINE(CYGARC_MB_EXCEPTION_SIZE, sizeof(HAL_SavedRegisters));

    DEFINE(CYGARC_MB_STACK_FRAME_SIZE, CYGARC_MB_STACK_FRAME_SIZE);
    DEFINE(CYGARC_MB_EXCEPTION_DECREMENT, sizeof(HAL_SavedRegisters));

    // Variant definitions - want these to be included instead.
#ifdef CYGARC_VARIANT_DEFS
    CYGARC_VARIANT_DEFS
#endif
#ifdef CYGARC_PLATFORM_DEFS
    CYGARC_PLATFORM_DEFS
#endif

    // Memory layout values (since these aren't "asm"-safe)
#ifdef CYGMEM_REGION_rom    
    DEFINE(CYGMEM_REGION_rom, CYGMEM_REGION_rom);
#endif
#ifdef CYGMEM_REGION_ram    
    DEFINE(CYGMEM_REGION_ram, CYGMEM_REGION_ram);
#endif

    return 0;
}

//--------------------------------------------------------------------------
// EOF hal_mk_defs.c
