#ifndef CYGONCE_HAL_IO_H
#define CYGONCE_HAL_IO_H
//==========================================================================
//
//      hal_io.h
//
//      Microblaze CPU definitions
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
// Date:         2010-12-13
// Purpose:      Provide Microblaze register definitions
// Description:
//
// Usage:        #include <cyg/hal/ppc_regs.h>
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/hal.h>

#include <cyg/hal/var_io.h>

//==========================================================================
// Handy macros for defining register bits and fields:
//

#define BIT_(__n)              (1<<(__n))
#define MASK_(__n,__s)         (((1<<(__s))-1)<<(__n))
#define VALUE_(__n,__v)        ((__v)<<(__n))

//--------------------------------------------------------------------------
// SPR access macros
//

#define CYGARC_MTS(_spr_, _v_)               \
    __asm__ volatile (                       \
          "mts  %0," #_spr_ ";"              \
          : : "r" (_v_)                      \
    );

#define CYGARC_MFS(_spr_, _v_)               \
    __asm__ volatile (                       \
          "mfs  %0," #_spr_ ";"              \
          : "=r" (_v_) :                     \
    );


//--------------------------------------------------------------------------
// FSL interface access macros
//

#ifdef CYGHWR_HAL_MICROBLAZE_ENABLE_FSL

// CYGARC_READ_FSL and CYGARC_WRITE_FSL are non-blocking calls
#define CYGARC_READ_FSL(_fsl_, _v_)          \
    __asm__ volatile (                       \
          "nget  %0, rfsl"#_fsl_";"          \
          : "=r" (_v_) :                     \
    );

#define CYGARC_WRITE_FSL(_fsl_, _v_)         \
    __asm__ volatile (                       \
          "nput  %0, rfsl"#_fsl_";"          \
          : : "r" (_v_) :                    \
    );

// CYGARC_BLOCK_READ_FSL and CYGARC_BLOCK_WRITE_FSL are blocking
// calls, core is stall until transfer is completed
#define CYGARC_BLOCK_READ_FSL(_fsl_, _v_)    \
    __asm__ volatile (                       \
          "get  %0, rfsl"#_fsl_";"           \
          : "=r" (_v_) :                     \
    );

#define CYGARC_BLOCK_WRITE_FSL(_fsl_, _v_)   \
    __asm__ volatile (                       \
          "put  %0, rfsl"#_fsl_";"           \
          : : "r" (_v_) :                    \
    );

// CYGARC_ABLOCK_READ_FSL and CYGARC_ABLOCK_WRITE_FSL are atomic
// blocking calls, core is stall and non-interruptible until the
// transfer is completed
#define CYGARC_ABLOCK_READ_FSL(_fsl_, _v_)   \
    __asm__ volatile (                       \
          "aget  %0, rfsl"#_fsl_";"          \
          : "=r" (_v_) :                     \
    );

#define CYGARC_ABLOCK_WRITE_FSL(_fsl_, _v_)  \
    __asm__ volatile (                       \
          "aput  %0, rfsl"#_fsl_";"          \
          : : "r" (_v_) :                    \
    );

#endif


// Special registers
#define CYGARC_REG_PC                       0
#define CYGARC_REG_MSR                      1
#define CYGARC_REG_EAR                      3
#define CYGARC_REG_ESR                      4
#define CYGARC_REG_FSR                      7
#define CYGARC_REG_BTR                     11
#define CYGARC_REG_EDR                     13
#define CYGARC_REG_PID                   4096
#define CYGARC_REG_ZPR                   4097
#define CYGARC_REG_TLBX                  4098
#define CYGARC_REG_TLBLO                 4099
#define CYGARC_REG_PLBHI                 4100
#define CYGARC_REG_PVR0                  8192
#define CYGARC_REG_PVR1                  8193
#define CYGARC_REG_PVR2                  8194
#define CYGARC_REG_PVR3                  8195
#define CYGARC_REG_PVR4                  8196
#define CYGARC_REG_PVR5                  8197
#define CYGARC_REG_PVR6                  8198
#define CYGARC_REG_PVR7                  8199
#define CYGARC_REG_PVR8                  8200
#define CYGARC_REG_PVR9                  8201
#define CYGARC_REG_PVR10                 8202
#define CYGARC_REG_PVR11                 8203

// MSR Bits
#define CYGARC_REG_MSR_CC                BIT_(31) // Arithmetic Carry Copy
#define CYGARC_REG_MSR_VMS               BIT_(14) // Virtual Protected Mode Save
#define CYGARC_REG_MSR_VM                BIT_(13) // Virtual Protected Mode
#define CYGARC_REG_MSR_UMS               BIT_(12) // User Mode Save
#define CYGARC_REG_MSR_UM                BIT_(11) // User Mode
#define CYGARC_REG_MSR_PVR               BIT_(10) // Processor Version Register exists
#define CYGARC_REG_MSR_EIP               BIT_(9)  // Exception In Progress
#define CYGARC_REG_MSR_EE                BIT_(8)  // Exception Enable
#define CYGARC_REG_MSR_DCE               BIT_(7)  // Data Cache Enable
#define CYGARC_REG_MSR_DZO               BIT_(6)  // Division by Zero
#define CYGARC_REG_MSR_ICE               BIT_(5)  // Instruction Cache Enable
#define CYGARC_REG_MSR_FSL               BIT_(4)  // Stream (FSL or AXI) Error
#define CYGARC_REG_MSR_BIP               BIT_(3)  // Break in Progress
#define CYGARC_REG_MSR_C                 BIT_(2)  // Arithmetic Carry
#define CYGARC_REG_MSR_IE                BIT_(1)  // Interrupt Enable
#define CYGARC_REG_MSR_RES               BIT_(0)  // Reserved

// FSR bits
#define CYGARC_REG_FSR_IO                BIT_(4)  // Invalid Operation
#define CYGARC_REG_FSR_DZ                BIT_(3)  // Divide-by-zero
#define CYGARC_REG_FSR_OF                BIT_(2)  // Overflow
#define CYGARC_REG_FSR_UF                BIT_(1)  // Underflow
#define CYGARC_REG_FSR_DO                BIT_(0)  // De-normalized operand error

// PID bits
#define CYGARC_REG_PID_MASK              (0x000000FF)

// TLBLO bits
#define CYGARC_REG_TLBLO_EX              BIT_(9)  // Executable
#define CYGARC_REG_TLBLO_WR              BIT_(8)  // Writable
#define CYGARC_REG_TLBLO_ZSEL(__x)       (((__x) & 0xF) << 4)
#define CYGARC_REG_TLBLO_W               BIT_(3)  // Write through
#define CYGARC_REG_TLBLO_I               BIT_(2)  // Inhibit Caching
#define CYGARC_REG_TLBLO_M               BIT_(1)  // Memory Coherent
#define CYGARC_REG_TLBLO_G               BIT_(0)  // Guarded

// TLBHI bits
#define CYGARC_REG_TLBHI_SIZE(__x)       (__x) (((__x) & 0x7) << 7)
#define CYGARC_REG_TLBHI_V               (1<<2)   // Valid
#define CYGARC_REG_TLBHI_E               (1<<1)   // Endian
#define CYGARC_REG_TLBHI_U0              (1<<0)   // User Defined

// Exception
#define CYGARC_REG_ESR_STREAM             0x0     // Stream exception
#define CYGARC_REG_ESR_DUA                0x1     // Data unaligned access
#define CYGARC_REG_ESR_OPCODE             0x2     // Opcode exception
#define CYGARC_REG_ESR_IBUS               0x3     // Instruction bus error
#define CYGARC_REG_ESR_DBUS               0x4     // Data bus error
#define CYGARC_REG_ESR_DIVIDE             0x5     // Divide exception
#define CYGARC_REG_ESR_FPU                0x6     // Floating Point Unit
#define CYGARC_REG_ESR_PRIVILEGE          0x7     // Privilege exception
#define CYGARC_REG_ESR_DSTORAGE           0x16    // Data storage
#define CYGARC_REG_ESR_ISTORAGE           0x17    // Instruction storage
#define CYGARC_REG_ESR_DATA_TLBMISS       0x18    // Data TLB miss
#define CYGARC_REG_ESR_CODE_TLBMISS       0x19    // Instruction TLB miss

#define CYGARC_REG_PVR0_CFG               BIT_(31)
#define CYGARC_REG_PVR0_BS                BIT_(30)
#define CYGARC_REG_PVR0_DIV               BIT_(29)
#define CYGARC_REG_PVR0_MUL               BIT_(28)
#define CYGARC_REG_PVR0_FPU               BIT_(27)
#define CYGARC_REG_PVR0_EXC               BIT_(26)
#define CYGARC_REG_PVR0_ICU               BIT_(25)
#define CYGARC_REG_PVR0_DCU               BIT_(24)
#define CYGARC_REG_PVR0_MMU               BIT_(23)
#define CYGARC_REG_PVR0_BTC               BIT_(22)
#define CYGARC_REG_PVR0_ENDI              BIT_(21)
#define CYGARC_REG_PVR0_FT                BIT_(20)
#define CYGARC_REG_PVR0_MBV(_pvr_)        ((_pvr_ >> 8 ) & 0xff)


#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#define MSR        CYGARC_REG_MSR
#define EAR        CYGARC_REG_EAR
#define ESR        CYGARC_REG_ESR
#define FSR        CYGARC_REG_FSR
#define BTR        CYGARC_REG_BTR
#define EDR        CYGARC_REG_EDR
#define PID        CYGARC_REG_PID
#define ZPR        CYGARC_REG_ZPR
#define PID        CYGARC_REG_PID
#define TLBX       CYGARC_REG_TLBX
#define TLBLO      CYGARC_REG_TLBLO
#define PLBHI      CYGARC_REG_PLBHI

#define PVR0       CYGARC_REG_PVR0
#define PVR1       CYGARC_REG_PVR1
#define PVR2       CYGARC_REG_PVR2
#define PVR3       CYGARC_REG_PVR3
#define PVR4       CYGARC_REG_PVR4
#define PVR5       CYGARC_REG_PVR5
#define PVR6       CYGARC_REG_PVR6
#define PVR7       CYGARC_REG_PVR7
#define PVR8       CYGARC_REG_PVR8
#define PVR9       CYGARC_REG_PVR9
#define PVR10      CYGARC_REG_PVR10
#define PVR11      CYGARC_REG_PVR11

#define MSR_CC     CYGARC_REG_MSR_CC
#define MSR_VMS    CYGARC_REG_MSR_VMS
#define MSR_VM     CYGARC_REG_MSR_VM
#define MSR_UMS    CYGARC_REG_MSR_UMS
#define MSR_UM     CYGARC_REG_MSR_UM
#define MSR_PVR    CYGARC_REG_MSR_PVR
#define MSR_EIP    CYGARC_REG_MSR_EIP
#define MSR_EE     CYGARC_REG_MSR_EE
#define MSR_DCE    CYGARC_REG_MSR_DCE
#define MSR_DZO    CYGARC_REG_MSR_DZO
#define MSR_ICE    CYGARC_REG_MSR_ICE
#define MSR_FSL    CYGARC_REG_MSR_FSL
#define MSR_BIP    CYGARC_REG_MSR_BIP
#define MSR_C      CYGARC_REG_MSR_C
#define MSR_IE     CYGARC_REG_MSR_IE
#define MSR_RES    CYGARC_REG_MSR_RES

#define FSR_IO     CYGARC_REG_FSR_IO
#define FSR_DZ     CYGARC_REG_FSR_DZ
#define FSR_OF     CYGARC_REG_FSR_OF
#define FSR_UF     CYGARC_REG_FSR_UF
#define FSR_DO     CYGARC_REG_FSR_DO

#define TLBLO_EX   CYGARC_REG_TLBLO_EX
#define TLBLO_WR   CYGARC_REG_TLBLO_WR
#define TLBLO_ZSEL CYGARC_REG_TLBLO_ZSEL
#define TLBLO_W    CYGARC_REG_TLBLO_W
#define TLBLO_I    CYGARC_REG_TLBLO_I
#define TLBLO_M    CYGARC_REG_TLBLO_M
#define TLBLO_G    CYGARC_REG_TLBLO_G

#define TLBHI_SIZE CYGARC_REG_TLBHI_SIZE
#define TLBHI_V    CYGARC_REG_TLBHI_V
#define TLBHI_E    CYGARC_REG_TLBHI_E
#define TLBHI_U0   CYGARC_REG_TLBHI_U0
#endif

//--------------------------------------------------------------------------
// Enforce in-order IO for all HAL reads/writes using this macro.
//

#ifndef HAL_IO_BARRIER_DEFINED
# define HAL_IO_BARRIER()
#endif

//--------------------------------------------------------------------------
// IO Register access.
//
//

#ifndef HAL_IO_MACROS_DEFINED

//--------------------------------------------------------------------------
// BYTE Register access.
// Individual and vectorized access to 8 bit registers.
//

#define HAL_READ_UINT8( _register_, _value_ )                           \
    CYG_MACRO_START                                                     \
    ((_value_) = *((volatile CYG_BYTE *)(_register_)));                 \
    HAL_IO_BARRIER ();                                                  \
    CYG_MACRO_END

#define HAL_WRITE_UINT8( _register_, _value_ )                          \
    CYG_MACRO_START                                                     \
    (*((volatile CYG_BYTE *)(_register_)) = (_value_));                 \
    HAL_IO_BARRIER ();                                                  \
    CYG_MACRO_END

#define HAL_READ_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )     \
    CYG_MACRO_START                                                     \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_)) {   \
        (_buf_)[_i_] = ((volatile CYG_BYTE *)(_register_))[_j_];        \
        HAL_IO_BARRIER ();                                              \
    }                                                                   \
    CYG_MACRO_END

#define HAL_WRITE_UINT8_VECTOR( _register_, _buf_, _count_, _step_ )    \
    CYG_MACRO_START                                                     \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_)) {   \
        ((volatile CYG_BYTE *)(_register_))[_j_] = (_buf_)[_i_];        \
        HAL_IO_BARRIER ();                                              \
    }                                                                   \
    CYG_MACRO_END


//--------------------------------------------------------------------------
// 16 bit access.
// Individual and vectorized access to 16 bit registers.
//

#define HAL_READ_UINT16( _register_, _value_ )                          \
    CYG_MACRO_START                                                     \
    ((_value_) = *((volatile CYG_WORD16 *)(_register_)));               \
    HAL_IO_BARRIER ();                                                  \
    CYG_MACRO_END

#define HAL_WRITE_UINT16( _register_, _value_ )                         \
    CYG_MACRO_START                                                     \
    (*((volatile CYG_WORD16 *)(_register_)) = (_value_));               \
    HAL_IO_BARRIER ();                                                  \
    CYG_MACRO_END

#define HAL_READ_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )    \
    CYG_MACRO_START                                                     \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_)) {   \
        (_buf_)[_i_] = ((volatile CYG_WORD16 *)(_register_))[_j_];      \
        HAL_IO_BARRIER ();                                              \
    }                                                                   \
    CYG_MACRO_END

#define HAL_WRITE_UINT16_VECTOR( _register_, _buf_, _count_, _step_ )   \
    CYG_MACRO_START                                                     \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_)) {   \
        ((volatile CYG_WORD16 *)(_register_))[_j_] = (_buf_)[_i_];      \
        HAL_IO_BARRIER ();                                              \
    }                                                                   \
    CYG_MACRO_END

//--------------------------------------------------------------------------
// 32 bit access.
// Individual and vectorized access to 32 bit registers.
//

#define HAL_READ_UINT32( _register_, _value_ )                          \
    CYG_MACRO_START                                                     \
    ((_value_) = *((volatile CYG_WORD32 *)(_register_)));               \
    HAL_IO_BARRIER ();                                                  \
    CYG_MACRO_END

#define HAL_WRITE_UINT32( _register_, _value_ )                         \
    CYG_MACRO_START                                                     \
    (*((volatile CYG_WORD32 *)(_register_)) = (_value_));               \
    HAL_IO_BARRIER ();                                                  \
    CYG_MACRO_END

#define HAL_READ_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )    \
    CYG_MACRO_START                                                     \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_)) {   \
        (_buf_)[_i_] = ((volatile CYG_WORD32 *)(_register_))[_j_];      \
        HAL_IO_BARRIER ();                                              \
    }                                                                   \
    CYG_MACRO_END

#define HAL_WRITE_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )   \
    CYG_MACRO_START                                                     \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_)) {   \
        ((volatile CYG_WORD32 *)(_register_))[_j_] = (_buf_)[_i_];      \
        HAL_IO_BARRIER ();                                              \
    }                                                                   \
    CYG_MACRO_END

#endif

//-----------------------------------------------------------------------------
#endif // ifdef CYGONCE_HAL_IO_H
// End of hal_io.h
