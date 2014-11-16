//==========================================================================
//
//      var_reg.h
//
//      Variant HAL memory map register definition 
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
// Description:  Variant HAL memory map register definition for Freescale
//               QorIQ target
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/hal/plf_regs.h> // For CYGARC_IMM_BASE

#ifndef CYGONCE_HAL_VAR_REGS_H
#define CYGONCE_HAL_VAR_REGS_H

//memory mapped registers

#define CYGARC_IMM_BASE_STARTUP                 0xFE000000        //Internal Memory Mapped Registers
#ifndef CYGARC_IMM_BASE
# error "Don't know Internal Memory Mapped Registers location"
#endif
//
// Base Address
//
//
#define CYGARC_REG_IMM_CPC1                     0x010000
#if defined(CYGHWR_HAL_POWERPC_QORIQ_P4040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4080)
# define CYGHWR_HAL_QORIQ_E500MC_CPC            2                 // 2 CPC
# define CYGARC_REG_IMM_CPC2                    0x011000
#else
# define CYGHWR_HAL_QORIQ_E500MC_CPC            1                 // 1 CPC
#endif
#define CYGARC_REG_IMM_PIC                      0x040000          // Interrupt controller base
#define CYGARC_REG_IMM_DCFG                     0x0E0000          // Device Configuration
#define CYGARC_REG_IMM_RCPM                     0x0E2000          // RCPM Memory
#define CYGARC_REG_IMM_I2C1                     0x118000          // I2C1 base
#define CYGARC_REG_IMM_I2C2                     0x118100          // I2C2 base
#define CYGARC_REG_IMM_I2C3                     0x119000          // I2C3 base
#define CYGARC_REG_IMM_I2C4                     0x119100          // I2C4 base
#define CYGARC_REG_IMM_UART1                    0x11C500          // UART1 base
#define CYGARC_REG_IMM_UART2                    0x11C600          // UART2 base
#define CYGARC_REG_IMM_UART3                    0x11D500          // UART3 base
#define CYGARC_REG_IMM_UART4                    0x11D600          // UART4 base
#define CYGARC_REG_IMM_GPIO                     0x130000          // GPIO base



//
// Local-address registers (Configuration / Control / Status)
//
//
#define CYGARC_REG_IMM_LCC_CCSRBARH             0x0000            // Configuration / control / status (MSB)
#define CYGARC_REG_IMM_LCC_CCSRBARL             0x0004            // Configuration / control / status (LSB)
#define CYGARC_REG_IMM_LCC_CCSRAR               0x0008            // Configuration, control, and status registers attribute register
#define CYGARC_REG_IMM_LCC_ALTCBARH             0x0010            // Alternate configuration base address register (MSB)
#define CYGARC_REG_IMM_LCC_ALTCBARL             0x0014            // Alternate configuration base address register (LSB)
#define CYGARC_REG_IMM_LCC_ALTCAR               0x0018            // Alternate configuration attribute register
#define CYGARC_REG_IMM_LCC_BSTRH                0x0020            // Boot page translation register (MSB)
#define CYGARC_REG_IMM_LCC_BSTRL                0x0024            // Boot page translation register (LSB)
#define CYGARC_REG_IMM_LCC_BSTAR                0x0028            // Boot space translation attribute register

#define CYGARC_REG_IMM_LCC_CCSRAR_C             0x80000000        // Commit
#define CONFIG_SYS_CCSRBAR_LBLAWAR0             0x81E0000B        // LAWARn[TRGT_ID] = 0x1E (a reserved setting) for this temporary LAW.

#define CYGARC_REG_IMM_LCC_BSTAR_SET_TRGT_ID(x) ((x & 0xFF) << 20)// Identifies the destination for accesses covered by boot space translation
#define CYGARC_REG_IMM_LCC_BSTAR_EN             0x80000000        // Enable
#define CYGARC_REG_IMM_LCC_BSTAR_4K             0x0B

//
// Device Configuration
//
//
#define CYGARC_REG_IMM_DCFG_BRR                 0x00E4
#define CYGARC_REG_IMM_DCFG_RCWSR(_n_)         ( CYGARC_REG_IMM_DCFG + 0x100 + (_n_ << 2) )


//
// Global Source and Target IDs
//
//
#define CYGARC_TRANS_SRC_ID_PCI1                0x01
#define CYGARC_TRANS_SRC_ID_PCI2                0x02
#define CYGARC_TRANS_SRC_ID_PCI3                0x03
#define CYGARC_TRANS_SRC_ID_SRIO1               0x08
#define CYGARC_TRANS_SRC_ID_SRIO2               0x09
#define CYGARC_TRANS_SRC_ID_BMAN                0x18
#define CYGARC_TRANS_SRC_ID_PAMU                0x1C
#define CYGARC_TRANS_SRC_ID_PME                 0x20
#define CYGARC_TRANS_SRC_ID_SEC                 0x21
#define CYGARC_TRANS_SRC_ID_QMAN                0x3C
#define CYGARC_TRANS_SRC_ID_USB1                0x40
#define CYGARC_TRANS_SRC_ID_USB2                0x41
#define CYGARC_TRANS_SRC_ID_eSDHC               0x44
#define CYGARC_TRANS_SRC_ID_PLB                 0x48
#define CYGARC_TRANS_SRC_ID_NPC                 0x4B
#define CYGARC_TRANS_SRC_ID_RMU                 0x5D
#define CYGARC_TRANS_SRC_ID_DMA1                0x70
#define CYGARC_TRANS_SRC_ID_DMA2                0x71
#define CYGARC_TRANS_SRC_ID_ICORE0              0x80
#define CYGARC_TRANS_SRC_ID_DCORE0              0x81
#define CYGARC_TRANS_SRC_ID_ICOREx(_x_)         (CYGARC_TRANS_SRC_ID_ICORE0 + (_x_ << 1))
#define CYGARC_TRANS_SRC_ID_DCOREx(_x_)         (CYGARC_TRANS_SRC_ID_DCORE0 + (_x_ << 1))
#define CYGARC_TRANS_SRC_ID_FMA1_ID1            0xC0
#define CYGARC_TRANS_SRC_ID_FMA1_IDx(_x_)       (CYGARC_TRANS_SRC_ID_FMA1_ID1 + (_x_ - 1))
#define CYGARC_TRANS_SRC_ID_FMA2_ID1            0xC0
#define CYGARC_TRANS_SRC_ID_FMA2_IDx(_x_)       (CYGARC_TRANS_SRC_ID_FMA2_ID1 + (_x_ - 1))

#define CYGARC_TRANS_DST_ID_PCI1                0x01
#define CYGARC_TRANS_DST_ID_PCI2                0x02
#define CYGARC_TRANS_DST_ID_PCI3                0x03
#define CYGARC_TRANS_DST_ID_SRIO1               0x08
#define CYGARC_TRANS_DST_ID_SRIO2               0x09
#define CYGARC_TRANS_DST_ID_LOCAL_SPACE         0x0F
#define CYGARC_TRANS_DST_ID_MEM_COMPLEX1        0x10
#define CYGARC_TRANS_DST_ID_MEM_COMPLEX2        0x11
#define CYGARC_TRANS_DST_ID_INTL_MEM_COMPLEX1_2 0x14
#define CYGARC_TRANS_DST_ID_BMAN_SW_PORTAL      0x18
#define CYGARC_TRANS_DST_ID_DCSR                0x1D
#define CYGARC_TRANS_DST_ID_eLBC                0x1F
#define CYGARC_TRANS_DST_ID_QMAN_SW_PORTAL      0x3C


//
// Clocking Memory Map/Register Definition
//
//
#define CYGARC_REG_IMM_CLK0C0CSR                0xE1000
#define CYGARC_REG_IMM_CLK1C0CSR                0xE1020
#define CYGARC_REG_IMM_CLK2C0CSR                0xE1040
#define CYGARC_REG_IMM_CLK3C0CSR                0xE1060
#define CYGARC_REG_IMM_CLK4C0CSR                0xE1080
#define CYGARC_REG_IMM_CLK5C0CSR                0xE10A0
#define CYGARC_REG_IMM_CLK6C0CSR                0xE10C0
#define CYGARC_REG_IMM_CLK7C0CSR                0xE10E0
#define CYGARC_REG_IMM_PLL1CGSR                 0xE1800
#define CYGARC_REG_IMM_PLL2CGSR                 0xE1820
#define CYGARC_REG_IMM_PLL3CGSR                 0xE1840
#define CYGARC_REG_IMM_PLL4CGSR                 0xE1860
#define CYGARC_REG_IMM_CLKPCSR                  0xE1A00
#define CYGARC_REG_IMM_PLLPGSR                  0xE1C00
#define CYGARC_REG_IMM_PLLDGSR                  0xE1C20

#define CYGARC_REG_IMM_CLKxC0CSR_CLK_SEL(_x_)  ( (_x_ >> 27) & 0x0f)

//
// Local-address registers (Local Access Window)
//
//
#define CYGARC_REG_IMM_LBLAWBARH0               0x0C00            //LBLAWBARH0 LBC local access window 0 base address register
#define CYGARC_REG_IMM_LBLAWBARL0               0x0C04            //LBLAWBARL0 LBC local access window 0 base address register
#define CYGARC_REG_IMM_LBLAWAR0                 0x0C08            //LBLAWAR0 LBC local access window 0 attribute register
#define CYGARC_REG_IMM_LBLAWBARH1               0x0C10
#define CYGARC_REG_IMM_LBLAWBARL1               0x0C14
#define CYGARC_REG_IMM_LBLAWAR1                 0x0C18
#define CYGARC_REG_IMM_LBLAWBARH2               0x0C20
#define CYGARC_REG_IMM_LBLAWBARL2               0x0C24
#define CYGARC_REG_IMM_LBLAWAR2                 0x0C28
#define CYGARC_REG_IMM_LBLAWBARH3               0x0C30
#define CYGARC_REG_IMM_LBLAWBARL3               0x0C34
#define CYGARC_REG_IMM_LBLAWAR3                 0x0C38
#define CYGARC_REG_IMM_LBLAWBARH4               0x0C40
#define CYGARC_REG_IMM_LBLAWBARL4               0x0C44
#define CYGARC_REG_IMM_LBLAWAR4                 0x0C48
#define CYGARC_REG_IMM_LBLAWBARH5               0x0C50
#define CYGARC_REG_IMM_LBLAWBARL5               0x0C54
#define CYGARC_REG_IMM_LBLAWAR5                 0x0C58
#define CYGARC_REG_IMM_LBLAWBARH6               0x0C60
#define CYGARC_REG_IMM_LBLAWBARL6               0x0C64
#define CYGARC_REG_IMM_LBLAWAR6                 0x0C68
#define CYGARC_REG_IMM_LBLAWBARH7               0x0C70
#define CYGARC_REG_IMM_LBLAWBARL7               0x0C74
#define CYGARC_REG_IMM_LBLAWAR7                 0x0C78
#define CYGARC_REG_IMM_LBLAWBARH8               0x0C80
#define CYGARC_REG_IMM_LBLAWBARL8               0x0C84
#define CYGARC_REG_IMM_LBLAWAR8                 0x0C88
#define CYGARC_REG_IMM_LBLAWBARH9               0x0C90
#define CYGARC_REG_IMM_LBLAWBARL9               0x0C94
#define CYGARC_REG_IMM_LBLAWAR9                 0x0C98
#define CYGARC_REG_IMM_LBLAWBARH10              0x0CA0
#define CYGARC_REG_IMM_LBLAWBARL10              0x0CA4
#define CYGARC_REG_IMM_LBLAWAR10                0x0CA8
#define CYGARC_REG_IMM_LBLAWBARH11              0x0CB0
#define CYGARC_REG_IMM_LBLAWBARL11              0x0CB4
#define CYGARC_REG_IMM_LBLAWAR11                0x0CB8
#define CYGARC_REG_IMM_LBLAWBARH12              0x0CC0
#define CYGARC_REG_IMM_LBLAWBARL12              0x0CC4
#define CYGARC_REG_IMM_LBLAWAR12                0x0CC8
#define CYGARC_REG_IMM_LBLAWBARH13              0x0CD0
#define CYGARC_REG_IMM_LBLAWBARL13              0x0CD4
#define CYGARC_REG_IMM_LBLAWAR13                0x0CD8
#define CYGARC_REG_IMM_LBLAWBARH14              0x0CE0
#define CYGARC_REG_IMM_LBLAWBARL14              0x0CE4
#define CYGARC_REG_IMM_LBLAWAR14                0x0CE8
#define CYGARC_REG_IMM_LBLAWBARH15              0x0CF0
#define CYGARC_REG_IMM_LBLAWBARL15              0x0CF4
#define CYGARC_REG_IMM_LBLAWAR15                0x0CF8
#define CYGARC_REG_IMM_LBLAWBARH16              0x0D00
#define CYGARC_REG_IMM_LBLAWBARL16              0x0D04
#define CYGARC_REG_IMM_LBLAWAR16                0x0D08
#define CYGARC_REG_IMM_LBLAWBARH17              0x0D10
#define CYGARC_REG_IMM_LBLAWBARL17              0x0D14
#define CYGARC_REG_IMM_LBLAWAR17                0x0D18
#define CYGARC_REG_IMM_LBLAWBARH18              0x0D20
#define CYGARC_REG_IMM_LBLAWBARL18              0x0D24
#define CYGARC_REG_IMM_LBLAWAR18                0x0D28
#define CYGARC_REG_IMM_LBLAWBARH19              0x0D30
#define CYGARC_REG_IMM_LBLAWBARL19              0x0D34
#define CYGARC_REG_IMM_LBLAWAR19                0x0D38
#define CYGARC_REG_IMM_LBLAWBARH20              0x0D40
#define CYGARC_REG_IMM_LBLAWBARL20              0x0D44
#define CYGARC_REG_IMM_LBLAWAR20                0x0D48
#define CYGARC_REG_IMM_LBLAWBARH21              0x0D50
#define CYGARC_REG_IMM_LBLAWBARL21              0x0D54
#define CYGARC_REG_IMM_LBLAWAR21                0x0D58
#define CYGARC_REG_IMM_LBLAWBARH22              0x0D60
#define CYGARC_REG_IMM_LBLAWBARL22              0x0D64
#define CYGARC_REG_IMM_LBLAWAR22                0x0D68
#define CYGARC_REG_IMM_LBLAWBARH23              0x0D70
#define CYGARC_REG_IMM_LBLAWBARL23              0x0D74
#define CYGARC_REG_IMM_LBLAWAR23                0x0D78
#define CYGARC_REG_IMM_LBLAWBARH24              0x0D80
#define CYGARC_REG_IMM_LBLAWBARL24              0x0D84
#define CYGARC_REG_IMM_LBLAWAR24                0x0D88
#define CYGARC_REG_IMM_LBLAWBARH25              0x0D90
#define CYGARC_REG_IMM_LBLAWBARL25              0x0D94
#define CYGARC_REG_IMM_LBLAWAR25                0x0D98
#define CYGARC_REG_IMM_LBLAWBARH26              0x0DA0
#define CYGARC_REG_IMM_LBLAWBARL26              0x0DA4
#define CYGARC_REG_IMM_LBLAWAR26                0x0DA8
#define CYGARC_REG_IMM_LBLAWBARH27              0x0DB0
#define CYGARC_REG_IMM_LBLAWBARL27              0x0DB4
#define CYGARC_REG_IMM_LBLAWAR27                0x0DB8
#define CYGARC_REG_IMM_LBLAWBARH28              0x0DC0
#define CYGARC_REG_IMM_LBLAWBARL28              0x0DC4
#define CYGARC_REG_IMM_LBLAWAR28                0x0DC8
#define CYGARC_REG_IMM_LBLAWBARH29              0x0DD0
#define CYGARC_REG_IMM_LBLAWBARL29              0x0DD4
#define CYGARC_REG_IMM_LBLAWAR29                0x0DD8
#define CYGARC_REG_IMM_LBLAWBARH30              0x0DE0
#define CYGARC_REG_IMM_LBLAWBARL30              0x0DE4
#define CYGARC_REG_IMM_LBLAWAR30                0x0DE8
#define CYGARC_REG_IMM_LBLAWBARH31              0x0DF0
#define CYGARC_REG_IMM_LBLAWBARL31              0x0DF4
#define CYGARC_REG_IMM_LBLAWAR31                0x0DF8


//
// DDR memory controller
//
//
#define CYGARC_REG_IMM_DDRC1                     0x8000            // DDR 1 memory controller offset
#if defined(CYGHWR_HAL_POWERPC_QORIQ_P4040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4080)
#define CYGARC_REG_IMM_DDRC2                     0x9000            // DDR 2 memory controller offset
#define CYGHWR_HAL_QORIQ_E500MC_DDRC             2                 // 2 Controller
#else
#define CYGHWR_HAL_QORIQ_E500MC_DDRC             1                 // 1 Controller
#endif

// relative to offset
#define CYGARC_REG_IMM_DDRC_CS0_BNDS             0x0000            // CS0_BNDS Chip select 0 memory bounds
#define CYGARC_REG_IMM_DDRC_CS1_BNDS             0x0008            // CS1_BNDS Chip select 1 memory bounds
#define CYGARC_REG_IMM_DDRC_CS2_BNDS             0x0010            // CS2_BNDS Chip select 2 memory bounds
#define CYGARC_REG_IMM_DDRC_CS3_BNDS             0x0018            // CS3_BNDS Chip select 3 memory bounds
#define CYGARC_REG_IMM_DDRC_CS0_CONFIG           0x0080            // CS0_CONFIG Chip select 0 configuration
#define CYGARC_REG_IMM_DDRC_CS1_CONFIG           0x0084            // CS1_CONFIG Chip select 1 configuration
#define CYGARC_REG_IMM_DDRC_CS2_CONFIG           0x0088            // CS2_CONFIG Chip select 2 configuration
#define CYGARC_REG_IMM_DDRC_CS3_CONFIG           0x008C            // CS3_CONFIG Chip select 3 configuration
#define CYGARC_REG_IMM_DDRC_CS0_CONFIG_2         0x00C0            // CS0_CONFIG_2 Chip select 0 configuration
#define CYGARC_REG_IMM_DDRC_CS1_CONFIG_2         0x00C4            // CS1_CONFIG_2 Chip select 1 configuration
#define CYGARC_REG_IMM_DDRC_CS2_CONFIG_2         0x00C8            // CS2_CONFIG_2 Chip select 2 configuration
#define CYGARC_REG_IMM_DDRC_CS3_CONFIG_2         0x00CC            // CS3_CONFIG_2 Chip select 3 configuration
#define CYGARC_REG_IMM_DDRC_TIMING_CFG_3         0x0100            // TIMING_CFG_0 DDR SDRAM timing configuration 3
#define CYGARC_REG_IMM_DDRC_TIMING_CFG_0         0x0104            // TIMING_CFG_0 DDR SDRAM timing configuration 0
#define CYGARC_REG_IMM_DDRC_TIMING_CFG_1         0x0108            // TIMING_CFG_0 DDR SDRAM timing configuration 1
#define CYGARC_REG_IMM_DDRC_TIMING_CFG_2         0x010c            // TIMING_CFG_0 DDR SDRAM timing configuration 2
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG        0x0110            // DDR_SDRAM_CFG DDR SDRAM control configuration
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_CFG_2      0x0114            // DDR_SDRAM_CFG_2 DDR SDRAM control configuration
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE       0x0118            // DDR_SDRAM_MODE DDR SDRAM mode configuration
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_2     0x011c            // DDR_SDRAM_MODE DDR SDRAM mode configuration
#define CYGARC_REG_IMM_DDRC_DDR_MD_CNTL          0x0120            // SDRAM Mode Control
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_INTERVAL   0x0124            // DDR_SDRAM_INTERVAL DDR SDRAM interval configuration
#define CYGARC_REG_IMM_DDRC_DDR_DATA_INIT        0x0128            // SDRAM data initialization
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_CLK_CNTL   0x0130            // DDR_SDRAM_CLK_CNTL DDR SDRAM clock control
#define CYGARC_REG_IMM_DDRC_DDR_INIT_ADDR        0x0148            // DDR Training initialization address
#define CYGARC_REG_IMM_DDRC_DDR_INIT_EXT_ADDR    0x014C            // DDR Training initialization extended address
#define CYGARC_REG_IMM_DDRC_TIMING_CFG_4         0x0160            // TIMING_CFG_0 DDR SDRAM timing configuration 4
#define CYGARC_REG_IMM_DDRC_TIMING_CFG_5         0x0164            // TIMING_CFG_0 DDR SDRAM timing configuration 5
#define CYGARC_REG_IMM_DDRC_DDR_ZQ_CNTL          0x0170            // DDR ZQ calibration control
#define CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL       0x0174            // DDR write leveling control
#define CYGARC_REG_IMM_DDRC_DDR_SR_CNTR          0x017c            // DDR Self Refresh Counter
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_RCW_1      0x0180            // DDR Register Control Words 1
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_RCW_2      0x0184            // DDR Register Control Words 2
#define CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL_2     0x0190            // DDR write leveling control 2
#define CYGARC_REG_IMM_DDRC_DDR_WRLVL_CNTL_3     0x0194            // DDR write leveling control 3
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_3     0x0200            // DDR SDRAM mode configuration 3
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_4     0x0204            // DDR SDRAM mode configuration 4
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_5     0x0208            // DDR SDRAM mode configuration 5
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_6     0x020C            // DDR SDRAM mode configuration 6
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_7     0x0210            // DDR SDRAM mode configuration 7
#define CYGARC_REG_IMM_DDRC_DDR_SDRAM_MODE_8     0x0214            // DDR SDRAM mode configuration 8
#define CYGARC_REG_IMM_DDRC_DDRDSR_1             0x0B20            // DDR Debug Status Register 1
#define CYGARC_REG_IMM_DDRC_DDRDSR_2             0x0B24            // DDR Debug Status Register 2
#define CYGARC_REG_IMM_DDRC_DDRCDR_1             0x0B28            // DDR Control Driver Register 1
#define CYGARC_REG_IMM_DDRC_DDRCDR_2             0x0B2C            // DDR Control Driver Register 2
#define CYGARC_REG_IMM_DDRC_DDR_IP_REV1          0x0BF8            // DDR IP block revision 1
#define CYGARC_REG_IMM_DDRC_DDR_IP_REV2          0x0BFC            // DDR IP block revision 2
#define CYGARC_REG_IMM_DDRC_DDR_MTCR             0x0D00            // DDR Memory Test Control Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP1             0x0D20            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP2             0x0D24            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP3             0x0D28            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP4             0x0D2C            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP5             0x0D30            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP6             0x0D34            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP7             0x0D38            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP8             0x0D3C            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP9             0x0D40            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DDR_MTP10            0x0D44            // DDR Memory Test Pattern n Register
#define CYGARC_REG_IMM_DDRC_DATA_ERR_INJECT_HI   0x0E00            // Memory data path error injection mask high
#define CYGARC_REG_IMM_DDRC_DATA_ERR_INJECT_LO   0x0E04            // Memory data path error injection mask low
#define CYGARC_REG_IMM_DDRC_ECC_ERR_INJECT       0x0E08            // Memory data path error injection mask ECC
#define CYGARC_REG_IMM_DDRC_CAPTURE_DATA_HI      0x0E20            // Memory data path read capture high
#define CYGARC_REG_IMM_DDRC_CAPTURE_DATA_LO      0x0E24            // Memory data path read capture low
#define CYGARC_REG_IMM_DDRC_CAPTURE_ECC          0x0E28            // Memory data path read capture ECC
#define CYGARC_REG_IMM_DDRC_ERR_DETECT           0x0E40            // Memory error detect
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE          0x0E44            // Memory error disable
#define CYGARC_REG_IMM_DDRC_ERR_INT_EN           0x0E48            // Memory error interrupt enable
#define CYGARC_REG_IMM_DDRC_CAPTURE_ATTRIBUTES   0x0E4C            // Memory error attributes capture
#define CYGARC_REG_IMM_DDRC_CAPTURE_ADDRESS      0x0E50            // Memory error address capture
#define CYGARC_REG_IMM_DDRC_CAPTURE_EXT_ADDRESS  0x0E54            // Memory error extended address capture
#define CYGARC_REG_IMM_DDRC_ERR_SBE              0x0E58            // Single-Bit ECC memory error management
#define CYGARC_REG_IMM_DDRC_DDR_DEBUG(_x_)      ( 0x0F00 + (_x_ << 2) )

#define CYGARC_REG_DDRC_SDRAM_CFG_MEM_EN         0x80000000
#define CYGARC_REG_DDRC_SDRAM_CFG2_INIT          0x00000010

#define CYGARC_REG_IMM_DDRC_ERR_DETECT_MME       0x80000000        // Multiple memory errors
#define CYGARC_REG_IMM_DDRC_ERR_DETECT_APE       0x00000100        // Address parity error.
#define CYGARC_REG_IMM_DDRC_ERR_DETECT_ACE       0x00000080        // Automatic calibration error.
#define CYGARC_REG_IMM_DDRC_ERR_DETECT_CDE       0x00000010        // Corrupted data error.
#define CYGARC_REG_IMM_DDRC_ERR_DETECT_MBE       0x00000008        // Multiple-bit error
#define CYGARC_REG_IMM_DDRC_ERR_DETECT_SBE       0x00000004        // Single-bit ECC error
#define CYGARC_REG_IMM_DDRC_ERR_DETECT_MSE       0x00000001        // Memory select error

#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_MME      0x80000000        // Multiple memory errors
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_APE      0x00000100        // Address parity error.
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_ACE      0x00000080        // Automatic calibration error.
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_CDE      0x00000010        // Corrupted data error.
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_MBE      0x00000008        // Multiple-bit error
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_SBE      0x00000004        // Single-bit ECC error
#define CYGARC_REG_IMM_DDRC_ERR_DISABLE_MSE      0x00000001        // Memory select error

#define CYGARC_REG_IMM_DDRC_CAP_ATTR_BNUM(_x_)  ((_x_ & 0x70000000) >> 28)    // Data beat number.
#define CYGARC_REG_IMM_DDRC_CAP_ATTR_TSIZ(_x_)  ((_x_ & 0x03000000) >> 24)    // Transaction size for the error
#define CYGARC_REG_IMM_DDRC_CAP_ATTR_TSRC(_x_)  ((_x_ & 0x00FF0000) >> 16)    // Transaction source for the error
#define CYGARC_REG_IMM_DDRC_CAP_ATTR_TTYP(_x_)  ((_x_ & 0x00003000) >> 12)    // Transaction type for the error
#define CYGARC_REG_IMM_DDRC_CAP_ATTR_VLD        0x00000001                    // Valid

//
// Enhanced Local Bus Controller (eLBC)
//
//
#define CYGARC_REG_IMM_ELBC                     0x00124000        // Enhanced Local Bus Controller offset

#define CYGARC_REG_IMM_ELBC_BR0                 0x0000            //BR0 Options register 0
#define CYGARC_REG_IMM_ELBC_OR0                 0x0004            //OR0 Options register 0
#define CYGARC_REG_IMM_ELBC_BR1                 0x0008            //BR1 Options register 1
#define CYGARC_REG_IMM_ELBC_OR1                 0x000C            //OR1 Options register 1
#define CYGARC_REG_IMM_ELBC_BR2                 0x0010            //BR2 Options register 2
#define CYGARC_REG_IMM_ELBC_OR2                 0x0014            //OR2 Options register 2
#define CYGARC_REG_IMM_ELBC_BR3                 0x0018            //BR3 Options register 3
#define CYGARC_REG_IMM_ELBC_OR3                 0x001c            //OR3 Options register 3
#define CYGARC_REG_IMM_ELBC_BR4                 0x0020            //BR4 Options register 4
#define CYGARC_REG_IMM_ELBC_OR4                 0x0024            //OR4 Options register 4
#define CYGARC_REG_IMM_ELBC_BR5                 0x0028            //BR5 Options register 5
#define CYGARC_REG_IMM_ELBC_OR5                 0x002C            //OR5 Options register 5
#define CYGARC_REG_IMM_ELBC_BR6                 0x0030            //BR6 Options register 6
#define CYGARC_REG_IMM_ELBC_OR6                 0x0034            //OR6 Options register 6
#define CYGARC_REG_IMM_ELBC_BR7                 0x0038            //BR7 Options register 7
#define CYGARC_REG_IMM_ELBC_OR7                 0x003C            //OR7 Options register 7
#define CYGARC_REG_IMM_ELBC_MBAR                0x0068            // UPM address register
#define CYGARC_REG_IMM_ELBC_MAMR                0x0070            // UPM mode register
#define CYGARC_REG_IMM_ELBC_MBMR                0x0074            // UPM mode register
#define CYGARC_REG_IMM_ELBC_MCMR                0x0078            // UPM mode register
#define CYGARC_REG_IMM_ELBC_MRTPR               0x0084            // Memory refresh timer prescaler register
#define CYGARC_REG_IMM_ELBC_MDRu                0x0088            // UPM / FCM data register
#define CYGARC_REG_IMM_ELBC_LSOR                0x0090            // Special operation initiation register
#define CYGARC_REG_IMM_ELBC_LURT                0x00A0            // UPM refresh timer
#define CYGARC_REG_IMM_ELBC_LTESR               0x00B0            // Transfer error status register
#define CYGARC_REG_IMM_ELBC_LTEDR               0x00B4            // Transfer error disable register
#define CYGARC_REG_IMM_ELBC_LTEIR               0x00B8            // Transfer error interrupt register
#define CYGARC_REG_IMM_ELBC_LTEATR              0x00BC            // Transfer error attributes register
#define CYGARC_REG_IMM_ELBC_LTEAR               0x00C0            // Transfer error address register
#define CYGARC_REG_IMM_ELBC_LTECCR              0x00C4            // Transfer error ECC register
#define CYGARC_REG_IMM_ELBC_LBCR                0x00D0            // Configuration register
#define CYGARC_REG_IMM_ELBC_LCRR                0x00D4            // Clock ratio register
#define CYGARC_REG_IMM_ELBC_FMR                 0x00E0            // Flash mode register
#define CYGARC_REG_IMM_ELBC_FIR                 0x00E4            // Flash instruction register
#define CYGARC_REG_IMM_ELBC_FCR                 0x00E8            // Flash command register
#define CYGARC_REG_IMM_ELBC_FBAR                0x00EC            // Flash block address register
#define CYGARC_REG_IMM_ELBC_FPAR                0x00F0            // Flash page address register
#define CYGARC_REG_IMM_ELBC_FBCR                0x00F4            // Flash byte count register
#define CYGARC_REG_IMM_ELBC_FECC0               0x0100            // Flash ECC block n registers
#define CYGARC_REG_IMM_ELBC_FECC1               0x0104            // Flash ECC block n registers
#define CYGARC_REG_IMM_ELBC_FECC2               0x0108            // Flash ECC block n registers
#define CYGARC_REG_IMM_ELBC_FECC3               0x010C            // Flash ECC block n registers

#define CYGARC_REG_IMM_ELBC_LCRR_CLKDIV_MASK    0x0000001F
#define CYGARC_REG_IMM_ELBC_LCRR_CLKDIV_8       0x00000002
#define CYGARC_REG_IMM_ELBC_LCRR_CLKDIV_16      0x00000004
#define CYGARC_REG_IMM_ELBC_LCRR_CLKDIV_32      0x00000008

//
// GPIO
//
//
#define CYGARC_REG_IMM_GP1DIR                   0x0000            //GPIO direction register R/W 0x0000_0000 21.3.1/21-3
#define CYGARC_REG_IMM_GP1DR                    0x0004            //GPIO open drain register R/W 0x0000_0000 21.3.2/21-3
#define CYGARC_REG_IMM_GP1DAT                   0x0008            //GPIO data register R/W 0x0000_0000 21.3.3/21-4
#define CYGARC_REG_IMM_GP1IER                   0x000C            //GPIO interrupt event register R/W Undefined 21.3.4/21-4
#define CYGARC_REG_IMM_GP1IMR                   0x0010            //GPIO interrupt mask register R/W 0x0000_0000 21.3.5/21-4
#define CYGARC_REG_IMM_GP1ICR                   0x0014            //GPIO external interrupt control register R/W 0x0000_0000 21.3.6/21-5

#define CYGARC_REG_GPIO(_x_)                    (0x80000000 >> _x_)

 
#define CYGARC_REG_GPPORCR                      0xE0020


//
// Serial RapidIO
//
//
#define CYGARC_REG_IMM_SRIO_DIDCAR              0x00C0000 // Device identity capability
#define CYGARC_REG_IMM_SRIO_DICAR               0x00C0004 // Device information capability
#define CYGARC_REG_IMM_SRIO_AIDCAR              0x00C0008 // Assembly identity capability
#define CYGARC_REG_IMM_SRIO_AICAR               0x00C000C // Assembly information capability
#define CYGARC_REG_IMM_SRIO_PEFCAR              0x00C0010 // Processing element features capability
#define CYGARC_REG_IMM_SRIO_SOCAR               0x00C0018 // Source operations capability
#define CYGARC_REG_IMM_SRIO_DOCAR               0x00C001C // Destination operations capability
#define CYGARC_REG_IMM_SRIO_MCSR                0x00C0040 // Mailbox command and status
#define CYGARC_REG_IMM_SRIO_PWDCSR              0x00C0044 // Port -Write and doorbell command and status
#define CYGARC_REG_IMM_SRIO_PELLCCSR            0x00C004C // Processing element logical layer control command and status
#define CYGARC_REG_IMM_SRIO_LCSBA1CSR           0x00C005C // Local configuration space base address 1 command
#define CYGARC_REG_IMM_SRIO_BDIDCSR             0x00C0060 // Base device ID command and status
#define CYGARC_REG_IMM_SRIO_HBDIDLCSR           0x00C0068 // Host base device ID lock command and status
#define CYGARC_REG_IMM_SRIO_CTCSR               0x00C006C // Component tag command and status
#define CYGARC_REG_IMM_SRIO_PMBH0               0x00C0100 // Port maintenance block header 0
#define CYGARC_REG_IMM_SRIO_PLTOCCSR            0x00C0120 // Port link time-out control command and status
#define CYGARC_REG_IMM_SRIO_PRTOCCSR            0x00C0124 // Port response time-out control command and status
#define CYGARC_REG_IMM_SRIO_PGCCSR              0x00C013C // Port General control command and status
#define CYGARC_REG_IMM_SRIO_P1LMREQCSR          0x00C0140 // Port 1 Link maintenance request command and status
#define CYGARC_REG_IMM_SRIO_P1LMRESPCSR         0x00C0144 // Port 1 Link maintenance response command and status
#define CYGARC_REG_IMM_SRIO_P1LASCSR            0x00C0148 // Port 1 Local ackID status command and status
#define CYGARC_REG_IMM_SRIO_P1ESCSR             0x00C0158 // Port 1 Error and status command and status
#define CYGARC_REG_IMM_SRIO_P1CCSR              0x00C015C // Port 1 Control command and status
#define CYGARC_REG_IMM_SRIO_P2LMREQCSR          0x00C0160 // Port 2 Link maintenance request command and status
#define CYGARC_REG_IMM_SRIO_P2LMRESPCSR         0x00C0164 // Port 2 Link maintenance response command and status
#define CYGARC_REG_IMM_SRIO_P2LASCSR            0x00C0168 // Port 2 Local ackID status command and status
#define CYGARC_REG_IMM_SRIO_P2ESCSR             0x00C0178 // Port 2 Error and status command and status
#define CYGARC_REG_IMM_SRIO_P2CCSR              0x00C017C // Port 2 Control command and status
#define CYGARC_REG_IMM_SRIO_ERBH                0x00C0600 // Error reporting block header
#define CYGARC_REG_IMM_SRIO_LTLEDCSR            0x00C0608 // Logical/Transport layer error detect command and status
#define CYGARC_REG_IMM_SRIO_LTLEECSR            0x00C060C // Logical/Transport layer error enable command and status
#define CYGARC_REG_IMM_SRIO_LTLACCSR            0x00C0614 // Logical/Transport layer address capture command and status
#define CYGARC_REG_IMM_SRIO_LTLDIDCCSR          0x00C0618 // Logical/Transport layer device ID capture command and status
#define CYGARC_REG_IMM_SRIO_LTLCCCSR            0x00C061C // Logical/Transport layer control capture command and status
#define CYGARC_REG_IMM_SRIO_P1EDCSR             0x00C0640 // Port 1 Error detect command and status
#define CYGARC_REG_IMM_SRIO_P1ERECSR            0x00C0644 // Port 1 Error rate enable command and status
#define CYGARC_REG_IMM_SRIO_P1ECACSR            0x00C0648 // Port 1 Error capture attributes command and status
#define CYGARC_REG_IMM_SRIO_P1PCSECCSR0         0x00C064C // Port 1 Packet/control symbol error capture command and status
#define CYGARC_REG_IMM_SRIO_P1PECCSR1           0x00C0650 // Port 1 Packet error capture command and status register 1
#define CYGARC_REG_IMM_SRIO_P1PECCSR2           0x00C0654 // Port 1 Packet error capture command and status register 2
#define CYGARC_REG_IMM_SRIO_P1PECCSR3           0x00C0658 // Port 1 Packet error capture command and status register 3
#define CYGARC_REG_IMM_SRIO_P1ERCSR             0x00C0668 // Port 1 Error rate command and status
#define CYGARC_REG_IMM_SRIO_P1ERTCSR            0x00C066C // Port 1 Error rate threshold command and status
#define CYGARC_REG_IMM_SRIO_P2EDCSR             0x00C0680 // Port 2 Error detect command and status
#define CYGARC_REG_IMM_SRIO_P2ERECSR            0x00C0684 // Port 2 Error rate enable command and status
#define CYGARC_REG_IMM_SRIO_P2ECACSR            0x00C0688 // Port 2 Error capture attributes command and status
#define CYGARC_REG_IMM_SRIO_P2PCSECCSR0         0x00C068C // Port 2 Packet/control symbol error capture command and status
#define CYGARC_REG_IMM_SRIO_P2PECCSR1           0x00C0690 // Port 2 Packet error capture command and status register 1
#define CYGARC_REG_IMM_SRIO_P2PECCSR2           0x00C0694 // Port 2 Packet error capture command and status register 2
#define CYGARC_REG_IMM_SRIO_P2PECCSR3           0x00C0698 // Port 2 Packet error capture command and status register 3
#define CYGARC_REG_IMM_SRIO_P2ERCSR             0x00C06A8 // Port 2 Error rate command and status
#define CYGARC_REG_IMM_SRIO_P2ERTCSR            0x00C06AC // Port 2 Error rate threshold command and status

#define CYGARC_REG_IMM_SRIO_LLCR                0x00D0004 // Logical layer configuration
#define CYGARC_REG_IMM_SRIO_EPWISR              0x00D0010 // Error / port-write interrupt status
#define CYGARC_REG_IMM_SRIO_LRETCR              0x00D0020 // Logical retry error threshold configuration
#define CYGARC_REG_IMM_SRIO_PRETCR              0x00D0080 // Physical retry error threshold configuration
#define CYGARC_REG_IMM_SRIO_P1ADIDCSR           0x00D0100 // Port 1 Alternate device ID command and status
#define CYGARC_REG_IMM_SRIO_P1AACR              0x00D0120 // Port 1 accept-all configuration register
#define CYGARC_REG_IMM_SRIO_P1LOPTTLCR          0x00D0124 // Port 1 Logical Outbound Packet time-to-live configuration
#define CYGARC_REG_IMM_SRIO_P1IECSR             0x00D0130 // Port 1 Implementation error command and status
#define CYGARC_REG_IMM_SRIO_P1PCR               0x00D0140 // Port 1 Physical configuration
#define CYGARC_REG_IMM_SRIO_P1SLCSR             0x00D0158 // Port 1 Serial link command and status
#define CYGARC_REG_IMM_SRIO_P1SLEICR            0x00D0160 // Port 1 Serial link error injection configuration
#define CYGARC_REG_IMM_SRIO_P2ADIDCSR           0x00D0180 // Port 2 Alternate device ID command and status
#define CYGARC_REG_IMM_SRIO_P2AACR              0x00D01A0 // Port 2 accept-all configuration register
#define CYGARC_REG_IMM_SRIO_P2LOPTTLCR          0x00D01A4 // Port 2 Logical Outbound Packet time-to-live configuration
#define CYGARC_REG_IMM_SRIO_P2IECSR             0x00D01B0 // Port 2 Implementation error command and status
#define CYGARC_REG_IMM_SRIO_P2PCR               0x00D01C0 // Port 2 Physical configuration
#define CYGARC_REG_IMM_SRIO_P2SLCSR             0x00D01D8 // Port 2 Serial link command and status
#define CYGARC_REG_IMM_SRIO_P2SLEICR            0x00D01E0 // Port 2 Serial link error injection configuration
#define CYGARC_REG_IMM_SRIO_IPBRR1              0x00D0BF8 // IP Block Revision Register 1
#define CYGARC_REG_IMM_SRIO_IPBRR2              0x00D0BFC // IP Block Revision Register 2
#define CYGARC_REG_IMM_SRIO_P1ROWTAR0           0x00D0C00 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR0          0x00D0C04 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWARn0           0x00D0C10 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR1           0x00D0C20 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR1          0x00D0C24 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn1          0x00D0C28 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn1           0x00D0C30 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R1           0x00D0C34 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R1           0x00D0C38 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R1           0x00D0C3C // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR2           0x00D0C40 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR2          0x00D0C44 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn2          0x00D0C48 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn2           0x00D0C50 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R2           0x00D0C54 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R2           0x00D0C58 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R2           0x00D0C5C // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR3           0x00D0C60 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR3          0x00D0C64 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn3          0x00D0C68 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn3           0x00D0C70 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R3           0x00D0C74 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R3           0x00D0C78 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R3           0x00D0C7C // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR4           0x00D0C80 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR4          0x00D0C84 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn4          0x00D0C88 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn4           0x00D0C90 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R4           0x00D0C94 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R4           0x00D0C98 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R4           0x00D0C9C // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR5           0x00D0CA0 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR5          0x00D0CA4 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn5          0x00D0CA8 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn5           0x00D0CB0 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R5           0x00D0CB4 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R5           0x00D0CB8 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R5           0x00D0CBC // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR6           0x00D0CC0 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR6          0x00D0CC4 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn6          0x00D0CC8 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn6           0x00D0CD0 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R6           0x00D0CD4 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R6           0x00D0CD8 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R6           0x00D0CDC // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR7           0x00D0CE0 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR7          0x00D0CE4 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn7          0x00D0CE8 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn7           0x00D0CF0 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R7           0x00D0CF4 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R7           0x00D0CF8 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R7           0x00D0CFC // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1ROWTAR8           0x00D0D00 // Port 1 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1ROWTEAR8          0x00D0D04 // Port 1 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P1ROWBARn8          0x00D0D08 // Port 1 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1ROWARn8           0x00D0D10 // Port 1 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1ROWS1R8           0x00D0D14 // Port 1 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS2R8           0x00D0D18 // Port 1 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P1ROWS3R8           0x00D0D1C // Port 1 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P1RIWTAR4           0x00D0D60 // Port 1 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1RIWBARn4          0x00D0D68 // Port 1 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1RIWAR4            0x00D0D70 // Port 1 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1RIWTAR3           0x00D0D80 // Port 1 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1RIWBARn3          0x00D0D88 // Port 1 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1RIWAR3            0x00D0D90 // Port 1 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1RIWTAR2           0x00D0DA0 // Port 1 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1RIWBARn2          0x00D0DA8 // Port 1 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1RIWAR2            0x00D0DB0 // Port 1 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1RIWTAR1           0x00D0DC0 // Port 1 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1RIWBARn1          0x00D0DC8 // Port 1 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P1RIWAR1            0x00D0DD0 // Port 1 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P1RIWTAR0           0x00D0DE0 // Port 1 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P1RIWAR0            0x00D0DF0 // Port 0 RapidIO inbound window attributes register 0
#define CYGARC_REG_IMM_SRIO_P2ROWTAR0           0x00D0E00 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR0          0x00D0E04 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWARn0           0x00D0E10 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR1           0x00D0E20 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR1          0x00D0E24 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn1          0x00D0E28 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn1           0x00D0E30 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R1           0x00D0E34 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R1           0x00D0E38 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R1           0x00D0E3C // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR2           0x00D0E40 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR2          0x00D0E44 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn2          0x00D0E48 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn2           0x00D0E50 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R2           0x00D0E54 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R2           0x00D0E58 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R2           0x00D0E5C // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR3           0x00D0E60 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR3          0x00D0E64 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn3          0x00D0E68 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn3           0x00D0E70 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R3           0x00D0E74 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R3           0x00D0E78 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R3           0x00D0E7C // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR4           0x00D0E80 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR4          0x00D0E84 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn4          0x00D0E88 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn4           0x00D0E90 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R4           0x00D0E94 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R4           0x00D0E98 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R4           0x00D0E9C // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR5           0x00D0EA0 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR5          0x00D0EA4 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn5          0x00D0EA8 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn5           0x00D0EB0 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R5           0x00D0EB4 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R5           0x00D0EB8 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R5           0x00D0EBC // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR6           0x00D0EC0 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR6          0x00D0EC4 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn6          0x00D0EC8 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn6           0x00D0ED0 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R6           0x00D0ED4 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R6           0x00D0ED8 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R6           0x00D0EDC // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR7           0x00D0EE0 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR7          0x00D0EE4 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn7          0x00D0EE8 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn7           0x00D0EF0 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R7           0x00D0EF4 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R7           0x00D0EF8 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R7           0x00D0EFC // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2ROWTAR8           0x00D0F00 // Port 2 RapidIO outbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2ROWTEAR8          0x00D0F04 // Port 2 RapidIO outbound window translation extended address register 0
#define CYGARC_REG_IMM_SRIO_P2ROWBARn8          0x00D0F08 // Port 2 RapidIO outbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2ROWARn8           0x00D0F10 // Port 2 RapidIO outbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2ROWS1R8           0x00D0F14 // Port 2 RapidIO outbound window segment 1 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS2R8           0x00D0F18 // Port 2 RapidIO outbound window segment 2 register n
#define CYGARC_REG_IMM_SRIO_P2ROWS3R8           0x00D0F1C // Port 2 RapidIO outbound window segment 3 register n
#define CYGARC_REG_IMM_SRIO_P2RIWTAR4           0x00D0F60 // Port 2 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2RIWBARn4          0x00D0F68 // Port 2 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2RIWAR4            0x00D0F70 // Port 2 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2RIWTAR3           0x00D0F80 // Port 2 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2RIWBARn3          0x00D0F88 // Port 2 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2RIWAR3            0x00D0F90 // Port 2 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2RIWTAR2           0x00D0FA0 // Port 2 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2RIWBARn2          0x00D0FA8 // Port 2 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2RIWAR2            0x00D0FB0 // Port 2 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2RIWTAR1           0x00D0FC0 // Port 2 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2RIWBARn1          0x00D0FC8 // Port 2 RapidIO Inbound window base address register n
#define CYGARC_REG_IMM_SRIO_P2RIWAR1            0x00D0FD0 // Port 2 RapidIO inbound window attributes register n
#define CYGARC_REG_IMM_SRIO_P2RIWTAR0           0x00D0FE0 // Port 2 RapidIO Inbound window translation address register n
#define CYGARC_REG_IMM_SRIO_P2RIWAR0            0x00D0FF0 // Port 0 RapidIO inbound window attributes register 0
#define CYGARC_REG_IMM_SRIO_OM0MR               0x00D3000 // Outbound message n mode
#define CYGARC_REG_IMM_SRIO_OM0SR               0x00D3004 // Outbound message n status
#define CYGARC_REG_IMM_SRIO_EOM0DQDPAR          0x00D3008 // Extended outbound message n descriptor queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_OM0DQDPAR           0x00D300C // Outbound message n descriptor queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_EOM0SAR             0x00D3010 // Extended outbound message n source address
#define CYGARC_REG_IMM_SRIO_OM0SAR              0x00D3014 // Outbound message n source address
#define CYGARC_REG_IMM_SRIO_OM0DPR              0x00D3018 // Outbound message n destination port
#define CYGARC_REG_IMM_SRIO_OM0DATR             0x00D301C // Outbound message n destination attributes
#define CYGARC_REG_IMM_SRIO_OM0DCR              0x00D3020 // Outbound message n double-word count
#define CYGARC_REG_IMM_SRIO_EOM0DQEPAR          0x00D3024 // Extended outbound message n descriptor queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_OM0DQEPAR           0x00D3028 // Outbound message n descriptor queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_OM0RETCR            0x00D302C // Outbound message n retry error threshold configuration
#define CYGARC_REG_IMM_SRIO_OM0MGR              0x00D3030 // Outbound message n multicast group
#define CYGARC_REG_IMM_SRIO_OM0MLR              0x00D3034 // Outbound message n multicast list
#define CYGARC_REG_IMM_SRIO_IM0MR               0x00D3060 // Inbound message n mode
#define CYGARC_REG_IMM_SRIO_IM0SR               0x00D3064 // Inbound message n status
#define CYGARC_REG_IMM_SRIO_EIM0FQDPAR          0x00D3068 // Extended inbound message n frame queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_IM0FQDPAR           0x00D306C // Inbound message n frame queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_EIM0FQEPAR          0x00D3070 // Extended inbound message n frame queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_IM0FQEPAR           0x00D3074 // Inbound message n frame queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_IM0MIRIR            0x00D3078 // Inbound message n maximum interrupt report interval
#define CYGARC_REG_IMM_SRIO_OM1MR               0x00D3100 // Outbound message n mode
#define CYGARC_REG_IMM_SRIO_OM1SR               0x00D3104 // Outbound message n status
#define CYGARC_REG_IMM_SRIO_EOM1DQDPAR          0x00D3108 // Extended outbound message n descriptor queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_OM1DQDPAR           0x00D310C // Outbound message n descriptor queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_EOM1SAR             0x00D3110 // Extended outbound message n source address
#define CYGARC_REG_IMM_SRIO_OM1SAR              0x00D3114 // Outbound message n source address
#define CYGARC_REG_IMM_SRIO_OM1DPR              0x00D3118 // Outbound message n destination port
#define CYGARC_REG_IMM_SRIO_OM1DATR             0x00D311C // Outbound message n destination attributes
#define CYGARC_REG_IMM_SRIO_OM1DCR              0x00D3120 // Outbound message n double-word count
#define CYGARC_REG_IMM_SRIO_EOM1DQEPAR          0x00D3124 // Extended outbound message n descriptor queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_OM1DQEPAR           0x00D3128 // Outbound message n descriptor queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_OM1RETCR            0x00D312C // Outbound message n retry error threshold configuration
#define CYGARC_REG_IMM_SRIO_OM1MGR              0x00D3130 // Outbound message n multicast group
#define CYGARC_REG_IMM_SRIO_OM1MLR              0x00D3134 // Outbound message n multicast list
#define CYGARC_REG_IMM_SRIO_IM1MR               0x00D3160 // Inbound message n mode
#define CYGARC_REG_IMM_SRIO_IM1SR               0x00D3164 // Inbound message n status
#define CYGARC_REG_IMM_SRIO_EIM1FQDPAR          0x00D3168 // Extended inbound message n frame queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_IM1FQDPAR           0x00D316C // Inbound message n frame queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_EIM1FQEPAR          0x00D3170 // Extended inbound message n frame queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_IM1FQEPAR           0x00D3174 // Inbound message n frame queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_IM1MIRIR            0x00D3178 // Inbound message n maximum interrupt report interval
#define CYGARC_REG_IMM_SRIO_ODMR                0x00D3400 // Outbound doorbell mode
#define CYGARC_REG_IMM_SRIO_ODSR                0x00D3404 // Outbound doorbell status
#define CYGARC_REG_IMM_SRIO_ODDPR               0x00D3418 // Outbound doorbell n destination port
#define CYGARC_REG_IMM_SRIO_ODDATR              0x00D341C // Outbound doorbell n destination attributes
#define CYGARC_REG_IMM_SRIO_ODRETCR             0x00D342C // Outbound doorbell n retry error threshold configuration
#define CYGARC_REG_IMM_SRIO_IDMR                0x00D3460 // Inbound doorbell n mode
#define CYGARC_REG_IMM_SRIO_IDSR                0x00D3464 // Inbound doorbell n status
#define CYGARC_REG_IMM_SRIO_EIDQDPAR            0x00D3468 // Extended inbound doorbell n queue dequeue pointer address
#define CYGARC_REG_IMM_SRIO_IDQDPAR             0x00D346C // Inbound doorbell n queue dequeue Pointer address
#define CYGARC_REG_IMM_SRIO_EIDQEPAR            0x00D3470 // Extended inbound doorbell n queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_IDQEPAR             0x00D3474 // Inbound doorbell n Queue enqueue pointer address
#define CYGARC_REG_IMM_SRIO_IDMIRIR             0x00D3478 // Inbound doorbell n maximum interrupt report interval
#define CYGARC_REG_IMM_SRIO_IPWMR               0x00D34E0 // Inbound port-write n mode
#define CYGARC_REG_IMM_SRIO_IPWSR               0x00D34E4 // Inbound port-write n status
#define CYGARC_REG_IMM_SRIO_EIPWQBAR            0x00D34E8 // Extended inbound port-write n queue base address
#define CYGARC_REG_IMM_SRIO_IPWQBAR             0x00D34EC // Inbound port-write n queue base address


//
//
// CoreNet Coherency Fabric (CCF)
//
//
#define CYGARC_REG_IMM_CCF_CEDR                 0x018A00 // CCF Error Detect Register
#define CYGARC_REG_IMM_CCF_CEER                 0x018A04 // CCF Error Enable Register
#define CYGARC_REG_IMM_CCF_CECAR                0x018A0C // CCF Error Capture Attribute Register
#define CYGARC_REG_IMM_CCF_CECADRH              0x018A10 // CCF Error Capture Address Register High
#define CYGARC_REG_IMM_CCF_CECADRL              0x018A14 // CCF Error Capture Address Register Low
#define CYGARC_REG_IMM_CCF_CECAR2               0x018A18 // CCF Error Capture Attribute Register 2


//
//
// RCPM Memory
//
//
#define CYGARC_REG_IMM_RCPM_CDOZSR              0x00004 // Core doze status
#define CYGARC_REG_IMM_RCPM_CDOZCR              0x0000C // Core doze control
#define CYGARC_REG_IMM_RCPM_CNAPSR              0x00014 // Core nap status
#define CYGARC_REG_IMM_RCPM_CNAPCR              0x0001C // Core nap control
#define CYGARC_REG_IMM_RCPM_CDOZPSR             0x00024 // Core doze previous status
#define CYGARC_REG_IMM_RCPM_CNAPPSR             0x0002C // Core nap previous status
#define CYGARC_REG_IMM_RCPM_CWAITSR             0x00034 // Core wait status
#define CYGARC_REG_IMM_RCPM_CWDTDSR             0x0003C // Core watchdog detect status
#define CYGARC_REG_IMM_RCPM_POWMGTCSR           0x00040 // Power management control and status
#define CYGARC_REG_IMM_RCPM_IPPDEXPCR           0x00050 // IP powerdown exception control
#define CYGARC_REG_IMM_RCPM_CPMIMR              0x00064 // Core Power Management Interrupt Mask
#define CYGARC_REG_IMM_RCPM_CPMCIMR             0x0006C // Core Power Management Critical Interrupt Mask
#define CYGARC_REG_IMM_RCPM_CPMMCMR             0x00074 // Core Power Management Machine Check Mask
#define CYGARC_REG_IMM_RCPM_CPMNMIMR            0x0007C // Core Power Management NMI Mask
#define CYGARC_REG_IMM_RCPM_CTBENR              0x00084 // Core timebase enable
#define CYGARC_REG_IMM_RCPM_CTBCKSELR           0x0008C // Core timebase clock select
#define CYGARC_REG_IMM_RCPM_CTBHLTCR            0x00094 // Core timebase halt control
#define CYGARC_REG_IMM_RCPM_CMCPMASKCR          0x000A4 // Core machine check mask control

//
//
// CPC
//
//

#define CYGARC_REG_IMM_CPC_CPCCSR0              0x000
#define CYGARC_REG_IMM_CPC_CPCCFG0              0x008
#define CYGARC_REG_IMM_CPC_CPCEWCR0             0x010
#define CYGARC_REG_IMM_CPC_CPCEWBAR0            0x014
#define CYGARC_REG_IMM_CPC_CPCEWCR1             0x020
#define CYGARC_REG_IMM_CPC_CPCEWBAR1            0x024
#define CYGARC_REG_IMM_CPC_CPCSRCR1             0x100
#define CYGARC_REG_IMM_CPC_CPCSRCR0             0x104
#define CYGARC_REG_IMM_CPC_HDBCR0               0xF00

// HDBCR0
#define CYGARC_REG_IMM_CPC_HDBCR0_SPEC_DIS       0x08000000
#define CYGARC_REG_IMM_CPC_HDBCR0_TECC_SCRUB_DIS 0x01000000
#define CYGARC_REG_IMM_CPC_HDBCR0_DECC_SCRUB_DIS 0x00400000
// CPCSRCR0
#define CYGARC_REG_IMM_CPC_CPCSRCR0_SRAMEN       0x00000001
#define CYGARC_REG_IMM_CPC_CPCSRCR0_SRAMSZ_64K   0x00000002
#define CYGARC_REG_IMM_CPC_CPCSRCR0_SRAMSZ_256K  0x0000000c
#define CYGARC_REG_IMM_CPC_CPCSRCR0_SRAMSZ_1024K 0x00000022
#define CYGARC_REG_IMM_CPC_CPCSRCR0_INTLVEN      0x00000080
// CPCCSR0
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCE          0x80000000 // Cache Enable
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCPE         0x40000000 // Enable ECC
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCFI         0x00200000 // Cache Flash Invalidate
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCWT         0x00080000 // Write-through mode
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCFL         0x00000800 // Hardware cache flush
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCLFC        0x00000400 // Cache Lock Flash Clear
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCLOA        0x00000080 // CPC lock overflow allocate.
#define CYGARC_REG_IMM_CPC_CPCCSR0_CPCLO         0x00000020 // CPC lock overflow.


//
//
// PIC
//
//
#define CYGARC_REG_IMM_PIC_GR_OFFSET            0x40000           // Offset of Global register section
// Feature reporting register
#define CYGARC_REG_IMM_PIC_FRR                  0x1000
// Global Configuration register
#define CYGARC_REG_IMM_PIC_GCR                  0x1020
// Global Configuration register field
#define CYGARC_REG_IMM_PIC_GCR_RST              0x80000000       // Reset PIC
#define CYGARC_REG_IMM_PIC_GCR_M                0x20000000       // PIC Mode
// Processor Core initialisation register
#define CYGARC_REG_IMM_PIC_PIR                  0x1090
// Interrupt vector registers
#define CYGARC_REG_IMM_PIC_IPIVPR0              0x10A0
#define CYGARC_REG_IMM_PIC_IPIVPR1              0x10B0
#define CYGARC_REG_IMM_PIC_IPIVPR2              0x10C0
#define CYGARC_REG_IMM_PIC_IPIVPR3              0x10D0
#define CYGARC_REG_IMM_PIC_IPIVPR_MSK           0x80000000        // Mask interrupts source
#define CYGARC_REG_IMM_PIC_IPIVPR_A             0x40000000        // Activity
#define CYGARC_REG_IMM_PIC_IPIVPR_PRIORITY      0x000F0000        // Priority number mask
#define CYGARC_REG_IMM_PIC_IPIVPR_VECTOR        0x0000FFFF        // Vector number mask
#define CYGARC_REG_IMM_PIC_EXT_IRQ_HIGH_OR_UP   0x00800000        // Active high or rising edge
#define CYGARC_REG_IMM_PIC_EXT_IRQ_SENSE_LEVEL  0x00400000        // Level sensitive
// Priority field
#define CYGARC_PIC_PRIORITY_0                   1
#define CYGARC_PIC_PRIORITY_1                   2
#define CYGARC_PIC_PRIORITY_2                   3
#define CYGARC_PIC_PRIORITY_3                   4
#define CYGARC_PIC_PRIORITY_4                   5
#define CYGARC_PIC_PRIORITY_5                   6
#define CYGARC_PIC_PRIORITY_6                   7
#define CYGARC_PIC_PRIORITY_7                   8
#define CYGARC_PIC_PRIORITY_8                   9
#define CYGARC_PIC_PRIORITY_9                   10
#define CYGARC_PIC_PRIORITY_10                  11
#define CYGARC_PIC_PRIORITY_11                  12
#define CYGARC_PIC_PRIORITY_12                  13
#define CYGARC_PIC_PRIORITY_13                  14
#define CYGARC_PIC_PRIORITY_14                  15
#define CYGARC_PIC_PRIORITY_15                  16
// Spurious vector register
#define CYGARC_REG_IMM_PIC_SVR                  0x10E0
// Message enable register
//#define CYGARC_REG_IMM_MER0                   0x1500
//#define CYGARC_REG_IMM_MER1                   0x2500
// Global Timer Current Count
#define CYGARC_REG_IMM_PIC_GTCCRA0              0x1100
#define CYGARC_REG_IMM_PIC_GTCCRA1              0x1140
#define CYGARC_REG_IMM_PIC_GTCCRA2              0x1180
#define CYGARC_REG_IMM_PIC_GTCCRA3              0x11C0
#define CYGARC_REG_IMM_PIC_GTCCRB0              0x2100
#define CYGARC_REG_IMM_PIC_GTCCRB1              0x2140
#define CYGARC_REG_IMM_PIC_GTCCRB2              0x2180
#define CYGARC_REG_IMM_PIC_GTCCRB3              0x21C0
// Global Timer Base Count
#define CYGARC_REG_IMM_PIC_GTBCRA0              0x1110
#define CYGARC_REG_IMM_PIC_GTBCRA1              0x1150
#define CYGARC_REG_IMM_PIC_GTBCRA2              0x1190
#define CYGARC_REG_IMM_PIC_GTBCRA3              0x11D0
#define CYGARC_REG_IMM_PIC_GTBCRB0              0x2110
#define CYGARC_REG_IMM_PIC_GTBCRB1              0x2150
#define CYGARC_REG_IMM_PIC_GTBCRB2              0x2190
#define CYGARC_REG_IMM_PIC_GTBCRB3              0x21D0
// Global timer vector/priority
#define CYGARC_REG_IMM_PIC_GTVPRA0              0x1120
#define CYGARC_REG_IMM_PIC_GTVPRA1              0x1160
#define CYGARC_REG_IMM_PIC_GTVPRA2              0x11A0
#define CYGARC_REG_IMM_PIC_GTVPRA3              0x11E0
#define CYGARC_REG_IMM_PIC_GTVPRB0              0x2120
#define CYGARC_REG_IMM_PIC_GTVPRB1              0x2160
#define CYGARC_REG_IMM_PIC_GTVPRB2              0x21A0
#define CYGARC_REG_IMM_PIC_GTVPRB3              0x21E0
// Global timer destination register
#define CYGARC_REG_IMM_PIC_GTDRA0               0x1130
#define CYGARC_REG_IMM_PIC_GTDRA1               0x1170
#define CYGARC_REG_IMM_PIC_GTDRA2               0x11B0
#define CYGARC_REG_IMM_PIC_GTDRA3               0x11F0
#define CYGARC_REG_IMM_PIC_GTDRB0               0x2130
#define CYGARC_REG_IMM_PIC_GTDRB1               0x2170
#define CYGARC_REG_IMM_PIC_GTDRB2               0x21B0
#define CYGARC_REG_IMM_PIC_GTDRB3               0x21F0
// Message register [Group A]
#define CYGARC_REG_IMM_PIC_MSGRA                0x1400
// Message register [Group B]
#define CYGARC_REG_IMM_PIC_MSGRB                0x2400
// Message register
#define CYGARC_REG_IMM_PIC_MSGR0                0x0000
#define CYGARC_REG_IMM_PIC_MSGR1                0x0010
#define CYGARC_REG_IMM_PIC_MSGR2                0x0020
#define CYGARC_REG_IMM_PIC_MSGR3                0x0030
#define CYGARC_REG_IMM_PIC_MER                  0x0100  // Message enable register
#define CYGARC_REG_IMM_PIC_MSR                  0x0110  // Message status register
// Shared message signaled interrupt register (offset) [Bank A]
#define CYGARC_REG_IMM_PIC_MSIRA                0x1600
// Shared message signaled interrupt register (offset) [Bank B]
#define CYGARC_REG_IMM_PIC_MSIRB                0x1800
// Shared message signaled interrupt register (offset) [Bank C]
#define CYGARC_REG_IMM_PIC_MSIRC                0x1A00
// Shared message signaled interrupt register
#define CYGARC_REG_IMM_PIC_MSIR0                0x0000
#define CYGARC_REG_IMM_PIC_MSIR1                0x0010
#define CYGARC_REG_IMM_PIC_MSIR2                0x0020
#define CYGARC_REG_IMM_PIC_MSIR3                0x0030
#define CYGARC_REG_IMM_PIC_MSIR4                0x0040
#define CYGARC_REG_IMM_PIC_MSIR5                0x0050
#define CYGARC_REG_IMM_PIC_MSIR6                0x0060
#define CYGARC_REG_IMM_PIC_MSIR7                0x0070
// Shared message signaled interrupt status register
#define CYGARC_REG_IMM_PIC_MSISR                0x0120
// Shared message signaled interrupt index register
#define CYGARC_REG_IMM_PIC_MSIIR                0x0140
// Performance monitor TBD

// Misc
#define CYGARC_REG_IMM_PIC_EISR0                0x3900 // Error interrupt summary register 0
#define CYGARC_REG_IMM_PIC_EIMR0                0x3910 // Error interrupt mask register 0
#define CYGARC_REG_IMM_PIC_WSRSR0               0x3A00 // Watchdog status register summary register 0
#define CYGARC_REG_IMM_PIC_CISR0                0x3B00 // Critical interrupt summary register 0
#define CYGARC_REG_IMM_PIC_CISR1                0x3B40 // Critical interrupt summary register 1
#define CYGARC_REG_IMM_PIC_CISR2                0x3B50 // Critical interrupt summary register 2
#define CYGARC_REG_IMM_PIC_CISR3                0x3B60 // Critical interrupt summary register 3
#define CYGARC_REG_IMM_PIC_CISR4                0x3B70 // Critical interrupt summary register 4
#define CYGARC_REG_IMM_PIC_MCSR0                0x3C00 // Machine check summary register 0
#define CYGARC_REG_IMM_PIC_MCSR1                0x3C40 // Machine check summary register 1
#define CYGARC_REG_IMM_PIC_MCSR2                0x3C50 // Machine check summary register 2
#define CYGARC_REG_IMM_PIC_MCSR3                0x3C60 // Machine check summary register 3
#define CYGARC_REG_IMM_PIC_MCSR4                0x3C70 // Machine check summary register 4
#define CYGARC_REG_IMM_PIC_IRQSIESR0            0x3D00 // IRQ_OUT/Soc Interrupt Event summary register 0
#define CYGARC_REG_IMM_PIC_IRQSIESR1            0x3D40 // IRQ_OUT/Soc Interrupt Event summary register 1
#define CYGARC_REG_IMM_PIC_IRQSIESR2            0x3D50 // IRQ_OUT/Soc Interrupt Event summary register 2
#define CYGARC_REG_IMM_PIC_IRQSIESR3            0x3D60 // IRQ_OUT/Soc Interrupt Event summary register 3
#define CYGARC_REG_IMM_PIC_IRQSIESR4            0x3D70 // IRQ_OUT/Soc Interrupt Event summary register 4
//Shared message signaled interrupt index register [Bank A]
#define CYGARC_REG_IMM_PIC_MSIIRA               0x4140
// Shared message signaled interrupt index register [Bank B]
#define CYGARC_REG_IMM_PIC_MSIIRB               0x5140
// Shared message signaled interrupt index register [Bank C]
#define CYGARC_REG_IMM_PIC_MSIIRC               0x6140

// Offset of interrupt source configuration register
#define CYGARC_REG_IMM_PIC_IRQ_SRC_CONF_OFFSET      0x50000
#define CYGARC_REG_IMM_PIC_IRQ_SRC_EXT_OFFSET_VPR   0x0000
#define CYGARC_REG_IMM_PIC_IRQ_SRC_EXT_OFFSET_IDR   0x0010
#define CYGARC_REG_IMM_PIC_IRQ_SRC_EXT_OFFSET_ILR   0x0018
#define CYGARC_REG_IMM_PIC_IRQ_SRC_INT_OFFSET_VPR   0x0200
#define CYGARC_REG_IMM_PIC_IRQ_SRC_INT_OFFSET_IDR   0x0210
#define CYGARC_REG_IMM_PIC_IRQ_SRC_INT_OFFSET_ILR   0x0218
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSGA_OFFSET_VPR  0x1600
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSGA_OFFSET_IDR  0x1610
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSGB_OFFSET_VPR  0x1680
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSGB_OFFSET_IDR  0x1690
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSIA_OFFSET_VPR  0x1C00
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSIA_OFFSET_IDR  0x1C10
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSIB_OFFSET_VPR  0x1D00
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSIB_OFFSET_IDR  0x1D10
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSIC_OFFSET_VPR  0x1E00
#define CYGARC_REG_IMM_PIC_IRQ_SRC_MSIC_OFFSET_IDR  0x1E10

// Default interrupt setting
#define CYGARC_REG_PIC_VECPRI_MASK              0x80000000
#define CYGARC_REG_PIC_PRI_DEFAULT              8
#define CYGARC_REG_PIC_DEST_CPU0                0x00000001   // Default destination

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P4080)
  #define CYGARC_REG_PIC_DEST_CPU_MASK          0x000000FF
#elif defined(CYGHWR_HAL_POWERPC_QORIQ_P4040)
  #define CYGARC_REG_PIC_DEST_CPU_MASK          0x0000000F
#else
# error "Don't know the QorIQ max core number"
#endif

// Number of interrupt source
#define CYGHWR_HAL_QORIQ_PIC_INT_IRQ_NUM        112  // Internal interrupts
#define CYGHWR_HAL_QORIQ_PIC_EXT_IRQ_NUM        12   // External interrupts
#define CYGHWR_HAL_QORIQ_PIC_MSGA_IRQ_NUM       4    // Message interrupts
#define CYGHWR_HAL_QORIQ_PIC_MSGB_IRQ_NUM       4    // Message interrupts
#define CYGHWR_HAL_QORIQ_PIC_MSI_BANK_NUM       3    // Shared Message Signaled Interrupts (number of bank)
#define CYGHWR_HAL_QORIQ_PIC_MSIA_IRQ_NUM       8    // Shared Message Signaled Interrupts
#define CYGHWR_HAL_QORIQ_PIC_MSIB_IRQ_NUM       8    // Shared Message Signaled Interrupts
#define CYGHWR_HAL_QORIQ_PIC_MSIC_IRQ_NUM       8    // Shared Message Signaled Interrupts
#define CYGHWR_HAL_QORIQ_PIC_GTA_IRQ_NUM        4    // Global timer A
#define CYGHWR_HAL_QORIQ_PIC_GTB_IRQ_NUM        4    // Global timer B
#define CYGHWR_HAL_QORIQ_PIC_IPI_IRQ_NUM        4    // Interprocessor interrupts

// Per CPU registers
#define CYGARC_REG_IMM_PIC_MP_CPU_OFFSET        0x60000
#define CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET     0x01000
// Interrupt acknowledge register
#define CYGARC_REG_IMM_PIC_IACK                 0x00A0
#define CYGARC_REG_IMM_PIC_IACK_CPU0            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*0 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU1            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*1 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU2            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*2 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU3            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*3 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU4            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*4 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU5            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*5 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU6            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*6 + CYGARC_REG_IMM_PIC_IACK)
#define CYGARC_REG_IMM_PIC_IACK_CPU7            (CYGARC_REG_IMM_PIC_CPU_LOCAL_OFFSET*7 + CYGARC_REG_IMM_PIC_IACK)
// End of Interrupt
#define CYGARC_REG_IMM_PIC_IEO                  0x00B0
// who I am register
#define CYGARC_REG_IMM_PIC_WHOAMI               0x0090
// Inter-processor dispatch register
#define CYGARC_REG_IMM_PIC_IPIDR0               0x0040
#define CYGARC_REG_IMM_PIC_IPIDR1               0x0050
#define CYGARC_REG_IMM_PIC_IPIDR2               0x0060
#define CYGARC_REG_IMM_PIC_IPIDR3               0x0070
// Current task priority register
#define CYGARC_REG_IMM_PIC_CTPR                 0x0080

/* Timers */
#define CYGARC_REG_IMM_PIC_TFRRA                0x10F0             // Time frequency Reporting
#define CYGARC_REG_IMM_PIC_TFRRB                0x20F0             // Time frequency Reporting
#define CYGARC_REG_IMM_PIC_TCRA                 0x1300             // Time Control Registers
#define CYGARC_REG_IMM_PIC_TCRB                 0x2300             // Time Control Registers

#define CYGARC_REG_IMM_PIC_GTCCR_CI             0x80000000         // Count inhibited


//
//
// SEC
//
//
#if 0
#define CYGARC_REG_IMM_SEC_CHANNEL1_ALTERNATE  0x30100
#define CYGARC_REG_IMM_SEC_CHANNEL2_ALTERNATE  0x30200
#define CYGARC_REG_IMM_SEC_CHANNEL3_ALTERNATE  0x30300
#define CYGARC_REG_IMM_SEC_CHANNEL4_ALTERNATE  0x30400

#define CYGARC_REG_IMM_SEC_CONTROLLER          0x31000
#define CYGARC_REG_IMM_SEC_CONTROLLER_IER_LSB  0x31008
#define CYGARC_REG_IMM_SEC_CONTROLLER_IER_MSB  0x3100C
#define CYGARC_REG_IMM_SEC_CONTROLLER_ISR_LSB  0x31010
#define CYGARC_REG_IMM_SEC_CONTROLLER_ISR_MSB  0x31014
#define CYGARC_REG_IMM_SEC_CONTROLLER_ICR_LSB  0x31018
#define CYGARC_REG_IMM_SEC_CONTROLLER_ICR_MSB  0x3101C

#define CYGARC_REG_IMM_SEC_CHANNEL1            0x31100
#define CYGARC_REG_IMM_SEC_CHANNEL2            0x31200
#define CYGARC_REG_IMM_SEC_CHANNEL3            0x31300
#define CYGARC_REG_IMM_SEC_CHANNEL4            0x31400

// Offsets in the channels memory map
#define CYGARC_REG_IMM_SEC_CHANNELx_CCR_LSB    0x00008
#define CYGARC_REG_IMM_SEC_CHANNELx_CCR_MSB    0x0000C
#define CYGARC_REG_IMM_SEC_CHANNELx_CSR_LSB    0x00010
#define CYGARC_REG_IMM_SEC_CHANNELx_CSR_MSB    0x00014
#define CYGARC_REG_IMM_SEC_CHANNELx_CDPR       0x00040
#define CYGARC_REG_IMM_SEC_CHANNELx_FF         0x00048 
#define CYGARC_REG_IMM_SEC_CHANNELx_GLT        0x000C0
#define CYGARC_REG_IMM_SEC_CHANNELx_SLT        0x000E0

#define CYGARC_REG_IMM_SEC_FFEC                0x31500
#define CYGARC_REG_IMM_SEC_DFC                 0x31508
#define CYGARC_REG_IMM_SEC_DBIC                0x31510
#define CYGARC_REG_IMM_SEC_DBOC                0x31518

#define CYGARC_REG_IMM_SEC_CONTROLLER_IPBRR    0x31BF8

#define CYGARC_REG_IMM_SEC_DEU                 0x32000
#define CYGARC_REG_IMM_SEC_AESEU               0x34000 
#define CYGARC_REG_IMM_SEC_MDEU                0x36000
#define CYGARC_REG_IMM_SEC_AFEU                0x38000
#define CYGARC_REG_IMM_SEC_RNGU                0x3A000
#define CYGARC_REG_IMM_SEC_PKEU                0x3C000
#define CYGARC_REG_IMM_SEC_STEU                0x3D000
#define CYGARC_REG_IMM_SEC_KEU                 0x3E000
#define CYGARC_REG_IMM_SEC_CRCU                0x3F000

#define CYGARC_REG_BPTR_EN                     0x80000000
#endif


//
//
// DMA
//
//
#define CYGARC_REG_IMM_DMA1_CH1                0x0100100 // Base
#define CYGARC_REG_IMM_DMA1_CH2                0x0100180 // Base
#define CYGARC_REG_IMM_DMA1_CH3                0x0100200 // Base
#define CYGARC_REG_IMM_DMA1_CH4                0x0100280 // Base

#define CYGARC_REG_IMM_DMA2_CH1                0x0101100 // Base
#define CYGARC_REG_IMM_DMA2_CH2                0x0101180 // Base
#define CYGARC_REG_IMM_DMA2_CH3                0x0101200 // Base
#define CYGARC_REG_IMM_DMA2_CH4                0x0101280 // Base

#define CYGARC_REG_IMM_DMA_MR                  0x000     // DMA n mode register
#define CYGARC_REG_IMM_DMA_SR                  0x004     // DMA n status register
#define CYGARC_REG_IMM_DMA_ECLNDAR             0x008     // DMA n current link descriptor extended address
#define CYGARC_REG_IMM_DMA_CLNDAR              0x00C     // DMA n current link descriptor address
#define CYGARC_REG_IMM_DMA_SATR                0x010     // DMA n source attributes
#define CYGARC_REG_IMM_DMA_SAR                 0x014     // DMA n source address register
#define CYGARC_REG_IMM_DMA_DATR                0x018     // DMA n destination attributes register
#define CYGARC_REG_IMM_DMA_DAR                 0x01C     // DMA n destination address register
#define CYGARC_REG_IMM_DMA_BCR                 0x020     // DMA n byte count register
#define CYGARC_REG_IMM_DMA_ENLNDAR             0x024     // DMA n extended next link descriptor address register
#define CYGARC_REG_IMM_DMA_NLNDAR              0x028     // DMA n next link descriptor address register
#define CYGARC_REG_IMM_DMA_ECLSDAR             0x030     // DMA n extended current list descriptor address register
#define CYGARC_REG_IMM_DMA_CLSDAR              0x034     // DMA n current list descriptor address register
#define CYGARC_REG_IMM_DMA_ENLSDAR             0x038     // DMA n extended next list descriptor address register
#define CYGARC_REG_IMM_DMA_NLSDAR              0x03C     // DMA n next list descriptor address register
#define CYGARC_REG_IMM_DMA_SSR                 0x040     // DMA n source stride register
#define CYGARC_REG_IMM_DMA_DSR                 0x044     // DMA n destination stride register


//
//
// PCI Express
//
//
#define CYGARC_PCIE_1_BASE_ADDR                0x00200000
#define CYGARC_PCIE_2_BASE_ADDR                0x00201000
#define CYGARC_PCIE_3_BASE_ADDR                0x00202000

#define CYGARC_PCIE_CONFIG_ADDR                0x000
#define CYGARC_PCIE_CONFIG_DATA                0x004
#define CYGARC_PCIE_RESERVED                   0x008
#define CYGARC_PCIE_OTB_CPL_TOR                0x00C
#define CYGARC_PCIE_RTY_TOR                    0x010
#define CYGARC_PCIE_CONFIG                     0x014

#define CYGARC_PCIE_PME_MES_DR                 0x020
#define CYGARC_PCIE_PME_MES_DISR               0x024
#define CYGARC_PCIE_PME_MES_IER                0x028
#define CYGARC_PCIE_PMCR                       0x02C

#define CYGARC_PCIE_IP_BLK_REV1                0xBF8
#define CYGARC_PCIE_IP_BLK_REV2                0xBFC

#define CYGARC_PCIE_OTAR0                      0xC00
#define CYGARC_PCIE_OTEAR0                     0xC04
#define CYGARC_PCIE_OWAR0                      0xC10
#define CYGARC_PCIE_OTAR1                      0xC20
#define CYGARC_PCIE_OTEAR1                     0xC24
#define CYGARC_PCIE_OWBAR1                     0xC28
#define CYGARC_PCIE_OWAR1                      0xC30
#define CYGARC_PCIE_OTAR2                      0xC40
#define CYGARC_PCIE_OTEAR2                     0xC44
#define CYGARC_PCIE_OWBAR2                     0xC48
#define CYGARC_PCIE_OWAR2                      0xC50
#define CYGARC_PCIE_OTAR3                      0xC60
#define CYGARC_PCIE_OTEAR3                     0xC64
#define CYGARC_PCIE_OWBAR3                     0xC68
#define CYGARC_PCIE_OWAR3                      0xC70
#define CYGARC_PCIE_OTAR4                      0xC80
#define CYGARC_PCIE_OTEAR4                     0xC84
#define CYGARC_PCIE_OWBAR4                     0xC88
#define CYGARC_PCIE_OWAR4                      0xC90

#define CYGARC_PCIE_MSIITAR                    0xD00
#define CYGARC_PCIE_MSIIWBAR                   0xD08
#define CYGARC_PCIE_MSIIWBEAR                  0xD0C
#define CYGARC_PCIE_MSIIWAR                    0xD10

#define CYGARC_PCIE_ITAR3                      0xDA0
#define CYGARC_PCIE_IWBAR3                     0xDA8
#define CYGARC_PCIE_IWBEAR3                    0xDAC
#define CYGARC_PCIE_IWAR3                      0xDB0
#define CYGARC_PCIE_ITAR2                      0xDC0
#define CYGARC_PCIE_IWBAR2                     0xDC8
#define CYGARC_PCIE_IWBEAR2                    0xDCC
#define CYGARC_PCIE_IWAR2                      0xDD0
#define CYGARC_PCIE_ITAR1                      0xDE0
#define CYGARC_PCIE_IWBAR1                     0xDE8
#define CYGARC_PCIE_IWBEAR1                    0xDEC
#define CYGARC_PCIE_IWAR1                      0xDF0

#define CYGARC_PCIE_ERR_DR                     0xE00
#define CYGARC_PCIE_ERR_EN                     0xE08
#define CYGARC_PCIE_ERR_DISR                   0xE10
#define CYGARC_PCIE_ERR_CAP_STAT               0xE20
#define CYGARC_PCIE_ERR_CAP_R0                 0xE28
#define CYGARC_PCIE_ERR_CAP_R1                 0xE2C
#define CYGARC_PCIE_ERR_CAP_R2                 0xE30
#define CYGARC_PCIE_ERR_CAP_R3                 0xE34

//OWARn
#define CYGARC_PCIE_OWAR_ENABLE                0x80000000
#define CYGARC_PCIE_OWAR_IO_READ               0x00080000
#define CYGARC_PCIE_OWAR_IO_WRITE              0x00008000
#define CYGARC_PCIE_OWAR_MEM_READ              0x00040000
#define CYGARC_PCIE_OWAR_MEM_WRITE             0x00004000

//
//
// SerDes Control Memory Map
//
//
#define CYGARC_REG_SRDSB1RSTCTL             0x000EA000
#define CYGARC_REG_SRDSB1PLLCR0             0x000EA004
#define CYGARC_REG_SRDSB1PLLCR1             0x000EA008
#define CYGARC_REG_SRDSB2RSTCTL             0x000EA020
#define CYGARC_REG_SRDSB2PLLCR0             0x000EA024
#define CYGARC_REG_SRDSB2PLLCR1             0x000EA028
#define CYGARC_REG_SRDSB3RSTCTL             0x000EA040
#define CYGARC_REG_SRDSB3PLLCR0             0x000EA044
#define CYGARC_REG_SRDSB3PLLCR1             0x000EA048
#define CYGARC_REG_SRDSTCALCR               0x000EA090
#define CYGARC_REG_SRDSRCALCR               0x000EA0A0
#define CYGARC_REG_SRDSGR0                  0x000EA0B0
#define CYGARC_REG_SRDSPCCR0                0x000EA0E0
#define CYGARC_REG_SRDSPCCR1                0x000EA0E4
#define CYGARC_REG_SRDSPCCR2                0x000EA0E8
#define CYGARC_REG_B1GCRA0                  0x000EA400
#define CYGARC_REG_B1GCRA1                  0x000EA404
#define CYGARC_REG_B1TECRA0                 0x000EA418
#define CYGARC_REG_B1TTLCRA0                0x000EA420
#define CYGARC_REG_B1GCRB0                  0x000EA440
#define CYGARC_REG_B1GCRB1                  0x000EA444
#define CYGARC_REG_B1TECRB0                 0x000EA458
#define CYGARC_REG_B1TTLCRB0                0x000EA460
#define CYGARC_REG_B1GCRC0                  0x000EA480
#define CYGARC_REG_B1GCRC1                  0x000EA484
#define CYGARC_REG_B1TECRC0                 0x000EA498
#define CYGARC_REG_B1TTLCRC0                0x000EA4A0
#define CYGARC_REG_B1GCRD0                  0x000EA4C0
#define CYGARC_REG_B1GCRD1                  0x000EA4C4
#define CYGARC_REG_B1TECRD0                 0x000EA4D8
#define CYGARC_REG_B1TTLCRD0                0x000EA4E0
#define CYGARC_REG_B1GCRE0                  0x000EA500
#define CYGARC_REG_B1GCRE1                  0x000EA504
#define CYGARC_REG_B1TECRE0                 0x000EA518
#define CYGARC_REG_B1TTLCRE0                0x000EA520
#define CYGARC_REG_B1GCRF0                  0x000EA540
#define CYGARC_REG_B1GCRF1                  0x000EA544
#define CYGARC_REG_B1TECRF0                 0x000EA558
#define CYGARC_REG_B1TTLCRF0                0x000EA560
#define CYGARC_REG_B1GCRG0                  0x000EA580
#define CYGARC_REG_B1GCRG1                  0x000EA584
#define CYGARC_REG_B1TECRG0                 0x000EA598
#define CYGARC_REG_B1TTLCRG0                0x000EA5A0
#define CYGARC_REG_B1GCRH0                  0x000EA5C0
#define CYGARC_REG_B1GCRH1                  0x000EA5C4
#define CYGARC_REG_B1TECRH0                 0x000EA5D8
#define CYGARC_REG_B1TTLCRH0                0x000EA5E0
#define CYGARC_REG_B1GCRI0                  0x000EA600
#define CYGARC_REG_B1GCRI1                  0x000EA604
#define CYGARC_REG_B1TECRI0                 0x000EA618
#define CYGARC_REG_B1TTLCRI0                0x000EA620
#define CYGARC_REG_B1GCRJ0                  0x000EA640
#define CYGARC_REG_B1GCRJ1                  0x000EA644
#define CYGARC_REG_B1TECRJ0                 0x000EA658
#define CYGARC_REG_B1TTLCRJ0                0x000EA660
#define CYGARC_REG_B2GCRA0                  0x000EA800
#define CYGARC_REG_B2GCRA1                  0x000EA804
#define CYGARC_REG_B2TECRA0                 0x000EA818
#define CYGARC_REG_B2TTLCRA0                0x000EA820
#define CYGARC_REG_B2GCRB0                  0x000EA840
#define CYGARC_REG_B2GCRB1                  0x000EA844
#define CYGARC_REG_B2TECRB0                 0x000EA858
#define CYGARC_REG_B2TTLCRB0                0x000EA860
#define CYGARC_REG_B2GCRC0                  0x000EA880
#define CYGARC_REG_B2GCRC1                  0x000EA884
#define CYGARC_REG_B2TECRC0                 0x000EA898
#define CYGARC_REG_B2TTLCRC0                0x000EA8A0
#define CYGARC_REG_B2GCRD0                  0x000EA8C0
#define CYGARC_REG_B2GCRD1                  0x000EA8C4
#define CYGARC_REG_B2TECRD0                 0x000EA8D8
#define CYGARC_REG_B2TTLCRD0                0x000EA8E0
#define CYGARC_REG_B3GCRA0                  0x000EA900
#define CYGARC_REG_B3GCRA1                  0x000EA904
#define CYGARC_REG_B3TECRA0                 0x000EA918
#define CYGARC_REG_B3TTLCRA0                0x000EA920
#define CYGARC_REG_B3GCRB0                  0x000EA940
#define CYGARC_REG_B3GCRB1                  0x000EA944
#define CYGARC_REG_B3TECRB0                 0x000EA958
#define CYGARC_REG_B3TTLCRB0                0x000EA960
#define CYGARC_REG_B3GCRC0                  0x000EA980
#define CYGARC_REG_B3GCRC1                  0x000EA984
#define CYGARC_REG_B3TECRC0                 0x000EA998
#define CYGARC_REG_B3TTLCRC0                0x000EA9A0
#define CYGARC_REG_B3GCRD0                  0x000EA9C0
#define CYGARC_REG_B3GCRD1                  0x000EA9C4
#define CYGARC_REG_B3TECRD0                 0x000EA9D8
#define CYGARC_REG_B3TTLCRD0                0x000EA9E0


//
//
// General
//
//
#define CYGARC_REG_IMM_DEVDISR              0x000E0070
#define CYGARC_REG_IMM_MCPSUMR              0x000E0090
#define CYGARC_REG_IMM_RSTRSCR              0x000E0094
#define CYGARC_REG_IMM_ECTRSTCR             0x000E0098
#define CYGARC_REG_IMM_PVR                  0x000E00A0
#define CYGARC_REG_IMM_SVR                  0x000E00A4
#define CYGARC_REG_IMM_RSTCR                0x000E00B0
#define CYGARC_REG_IMM_RSTCR_HRESET_REQ     0x00000002

//
// L1 cache
//

// L1CSR0
#define CYGARC_REG_L1CSR0_CE                0x00000001 // Data Cache enable
#define CYGARC_REG_L1CSR0_CFI               0x00000002 // Data Cache flash invalidate
#define CYGARC_REG_L1CSR0_DCBZ32            0x00000008 // Data cache operation length.
#define CYGARC_REG_L1CSR0_CEIT              0x00000600 // Cache error injection type.
#define CYGARC_REG_L1CSR0_CLFR              0x00000100 // Data Cache lock bits flash reset
#define CYGARC_REG_L1CSR0_CLO               0x00000200 // Data Cache lock overflow
#define CYGARC_REG_L1CSR0_CUL               0x00000400 // Data Cache unable to lock
#define CYGARC_REG_L1CSR0_CSLC              0x00000800 // Data Cache unable to lock
#define CYGARC_REG_L1CSR0_CEDT              0x00003000 // Data cache error detection/correction type
#define CYGARC_REG_L1CSR0_CPI               0x00008000 // Data Cache parity error injection enable
#define CYGARC_REG_L1CSR0_CPE               0x00010000 // Data Cache parity enable
#define CYGARC_REG_L1CSR0_CEA               0x000C0000 // Data cache error action

// L1CSR1
#define CYGARC_REG_L1CSR1_ICE               0x00000001 // Instruction Cache enable
#define CYGARC_REG_L1CSR1_ICFI              0x00000002 // Instruction Cache flash invalidate
#define CYGARC_REG_L1CSR1_ICLFR             0x00000100 // Instruction Cache lock bits flash reset
#define CYGARC_REG_L1CSR1_ICLO              0x00000200 // Instruction Cache lock overflow
#define CYGARC_REG_L1CSR1_ICUL              0x00000400 // Instruction Cache unable to lock
#define CYGARC_REG_L1CSR1_ICSLC             0x00000800 // Instruction Cache snoop lock clear
#define CYGARC_REG_L1CSR1_ICEDT             0x00003000 // Instruction cache error detection type
#define CYGARC_REG_L1CSR1_ICEI              0x00008000 // Instruction Cache parity error injection enable
#define CYGARC_REG_L1CSR1_ICECE             0x00010000 // Instruction error checking enable
#define CYGARC_REG_L1CSR1_ICEA              0x000C0000 // Instruction cache error action

// L1CSR2
#define CYGARC_REG_L1CSR2_DCSTASHID         0x000000FF // Data cache stash ID.
#define CYGARC_REG_L1CSR2_DCWS              0x40000000 // Data cache write-shadow.

//
// L2 cache
//

// L2CFG0
#define CYGARC_REG_L2CFG0_L2CTEHA(_x)       ((_x & 0x60000000) >> 29) // L2 cache tags error handling available
#define CYGARC_REG_L2CFG0_L2CDEHA(_x)       ((_x & 0x18000000) >> 27) // L2 cache data error handling available
#define CYGARC_REG_L2CFG0_L2CIDPA(_x)       ((_x & 0x04000000) >> 26) // Cache instruction and data partitioning available.
#define CYGARC_REG_L2CFG0_L2CBSIZE(_x)      ((_x & 0x03800000) >> 23) // Cache line size.
#define CYGARC_REG_L2CFG0_L2CREPL(_x)       ((_x & 0x00600000) >> 21) // Cache default replacement policy.
#define CYGARC_REG_L2CFG0_L2CLA(_x)         ((_x & 0x00100000) >> 20) // Cache line locking available.
#define CYGARC_REG_L2CFG0_L2CNWAY(_x)       ((_x & 0x0007C000) >> 14) // Number of cache ways
#define CYGARC_REG_L2CFG0_L2CSIZE(_x)       ((_x & 0x00003FFF))       // Cache size as a multiple of 64 Kbytes.

// L2CSR0
#define CYGARC_REG_L2CSR0_L2E               0x80000000                // L2 cache enable.
#define CYGARC_REG_L2CSR0_L2PE              0x40000000                // L2 cache parity/ECC error checking enable.
#define CYGARC_REG_L2CSR0_L2WP(x_)          ((_x & 0x7) << 25)        // L2 Instruction/Data Way Partitioning
#define CYGARC_REG_L2CSR0_L2FI              0x00200000                // L2 cache flash invalidate.
#define CYGARC_REG_L2CSR0_L2IO              0x00100000                // L2 cache instruction only
#define CYGARC_REG_L2CSR0_L2DO              0x00010000                // L2 cache data only.
#define CYGARC_REG_L2CSR0_L2REP             ((_x & 0x3) <<12)         // L2 line replacement algorithm.
#define CYGARC_REG_L2CSR0_L2FL              0x00000800                // L2 cache flush
#define CYGARC_REG_L2CSR0_L2LFC             0x00000400                // L2 cache lock flash clear
#define CYGARC_REG_L2CSR0_L2LOA             0x00000080                // L2 cache lock overflow allocate.
#define CYGARC_REG_L2CSR0_L2LO              0x00000020                // L2 cache lock overflow.

// L2CSR1
#define CYGARC_REG_L2CSR1_L2ILOSSLIMITDIS   0x80000000               // L2 Instruction Loss Limit Disable
#define CYGARC_REG_L2CSR1_L2ILOSSLIMIT(x_)  ((_x & 0x3) << 28)
#define CYGARC_REG_L2CSR1_L2SNPWINLIMITDIS  0x08000000               // L2 Snoop Win Limit Disable
#define CYGARC_REG_L2CSR1_L2SNPWINLIMIT     ((_x & 0x7) << 24)
#define CYGARC_REG_L2CSR1_L2STASHID(_x)     (_x & 0x000000FF)        // L2 cache stash ID.

// HID0
#define CYGARC_REG_HID0_NOPTI                0x00000001 // No-op the data and instruction cache touch instructions
#define CYGARC_REG_HID0_CIGLSO               0x00000010 // Cache-inhibited guarded load/store ordering.
#define CYGARC_REG_HID0_DCFA                 0x00000040 // Data cache flush assist
#define CYGARC_REG_HID0_EMAS7                0x00000080 // Enable MAS7 update
#define CYGARC_REG_HID0_EN_L2MMU_MHD         0x40000000 // Enable L2MMU multiple-hit detection.
#define CYGARC_REG_HID0_EMCP                 0x80000000 // Enable machine check pin

#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
// L1CSR0
#define L1CSR0_CE          CYGARC_REG_L1CSR0_CE
#define L1CSR0_CFI         CYGARC_REG_L1CSR0_CFI
#define L1CSR0_CLFR        CYGARC_REG_L1CSR0_CLFR
#define L1CSR0_CLO         CYGARC_REG_L1CSR0_CLO
#define L1CSR0_CUL         CYGARC_REG_L1CSR0_CUL
#define L1CSR0_CSLC        CYGARC_REG_L1CSR0_CSLC
#define L1CSR0_CPI         CYGARC_REG_L1CSR0_CPI
#define L1CSR0_CPE         CYGARC_REG_L1CSR0_CPE
#define L1CSR0_DCBZ32      CYGARC_REG_L1CSR0_DCBZ32
#define L1CSR0_CEIT        CYGARC_REG_L1CSR0_CEIT
#define L1CSR0_CEDT        CYGARC_REG_L1CSR0_CEDT
#define L1CSR0_CEA         CYGARC_REG_L1CSR0_CEA

// L1CSR1
#define L1CSR1_ICE         CYGARC_REG_L1CSR1_ICE
#define L1CSR1_ICFI        CYGARC_REG_L1CSR1_ICFI
#define L1CSR1_ICLFR       CYGARC_REG_L1CSR1_ICLFR
#define L1CSR1_ICLO        CYGARC_REG_L1CSR1_ICLO
#define L1CSR1_ICUL        CYGARC_REG_L1CSR1_ICUL
#define L1CSR1_ICSLC       CYGARC_REG_L1CSR1_ICSLC
#define L1CSR1_ICEDT       CYGARC_REG_L1CSR1_ICEDT
#define L1CSR1_ICEI        CYGARC_REG_L1CSR1_ICEI
#define L1CSR1_ICECE       CYGARC_REG_L1CSR1_ICECE
#define L1CSR1_ICEA        CYGARC_REG_L1CSR1_ICEA

// L1CSR2
#define L1CSR2_DCWS        CYGARC_REG_L1CSR2_DCWS

// HID0
#define HID0_NOPTI         CYGARC_REG_HID0_NOPTI
#define HID0_DCFA          CYGARC_REG_HID0_DCFA
#define HID0_EMAS7         CYGARC_REG_HID0_EMAS7
#define HID0_CIGLSO        CYGARC_REG_HID0_CIGLSO
#define HID0_EN_L2MMU_MHD  CYGARC_REG_HID0_EN_L2MMU_MHD
#define HID0_EMCP          CYGARC_REG_HID0_EMCP

// L2CSR0
#define L2CSR0_L2E         CYGARC_REG_L2CSR0_L2E
#define L2CSR0_L2PE        CYGARC_REG_L2CSR0_L2PE
#define L2CSR0_L2WP        CYGARC_REG_L2CSR0_L2WP
#define L2CSR0_L2FI        CYGARC_REG_L2CSR0_L2FI
#define L2CSR0_L2IO        CYGARC_REG_L2CSR0_L2IO
#define L2CSR0_L2DO        CYGARC_REG_L2CSR0_L2DO
#define L2CSR0_L2REP       CYGARC_REG_L2CSR0_L2REP
#define L2CSR0_L2FL        CYGARC_REG_L2CSR0_L2FL
#define L2CSR0_L2LFC       CYGARC_REG_L2CSR0_L2LFC
#define L2CSR0_L2LOA       CYGARC_REG_L2CSR0_L2LOA
#define L2CSR0_L2LO        CYGARC_REG_L2CSR0_L2LO

#define CPCCSR0_CPCE       CYGARC_REG_IMM_CPC_CPCCSR0_CPCE
#define CPCCSR0_CPCPE      CYGARC_REG_IMM_CPC_CPCCSR0_CPCPE
#define CPCCSR0_CPCFI      CYGARC_REG_IMM_CPC_CPCCSR0_CPCFI
#define CPCCSR0_CPCWT      CYGARC_REG_IMM_CPC_CPCCSR0_CPCWT
#define CPCCSR0_CPCFL      CYGARC_REG_IMM_CPC_CPCCSR0_CPCFL
#define CPCCSR0_CPCLFC     CYGARC_REG_IMM_CPC_CPCCSR0_CPCLFC
#define CPCCSR0_CPCLOA     CYGARC_REG_IMM_CPC_CPCCSR0_CPCLOA
#define CPCCSR0_CPCLO      CYGARC_REG_IMM_CPC_CPCCSR0_CPCLO

#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#define CYGARC_SPR_SVR_REV_MAJ(_x_)  (((_x_) >>  4) & 0xF)
#define CYGARC_SPR_SVR_REV_MIN(_x_)  (((_x_) >>  0) & 0xF)

#define CYGARC_SPR_SVR_SOC_IDs_MASK  0x0FF7FF00

#define CYGARC_SPR_PVR_REV_MAJ(_x_)  (((_x_) >>  4) & 0xF)
#define CYGARC_SPR_PVR_REV_MIN(_x_)  (((_x_) >>  0) & 0xF)

#endif // ifdef CYGONCE_HAL_VAR_REGS_H
// End of var_regs.h
