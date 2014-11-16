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
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 
// 2008, 2009, 2010 Free Software Foundation, Inc.
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
// Description:  Variant HAL memory map register definition for Freescale
//               Quicc III target
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_HAL_VAR_REGS_H
#define CYGONCE_HAL_VAR_REGS_H

//memory mapped registers

#define CYGARC_IMM_BASE_STARTUP                 0xFF700000        //Internal Memory Mapped Registers
#define CYGARC_IMM_BASE                         0xD0000000        //Internal Memory Mapped Registers after being moved

/*
 * Base Address
 *
 */
#define CYGARC_REG_IMM_PIC                      0x40000           // Interrupt controller base
#define CYGARC_REG_IMM_I2C                      0x03000           // I2C base
#define CYGARC_REG_IMM_UART                     0x04000           // UART base
#define CYGARC_REG_IMM_GPIO                     0x0F000           // GPIO base
#define CYGARC_REG_IMM_L2CACHE                  0x20000           // L2 Cache base
#define CYGARC_REG_IMM_SYS_MON                  0xE1000           // Performance monitoring

/*
 * Local-address registers (Configuration / Control / Status)
 *
 */
#define CYGARC_REG_IMM_CCSRBAR                  0x0000            // Configuration / control / status
#define CYGARC_REG_IMM_ALTCBAR                  0x0008            // Alternate configuration base address register
#define CYGARC_REG_IMM_ALTCAR                   0x0010            // Alternate configuration attribute register
#define CYGARC_REG_IMM_BPTR                     0x0020            // Boot page translation register

/*
 * Local-address registers (Local Access Window)
 *
 */ 
#define CYGARC_REG_IMM_LBLAWBAR0                0x0C08            //LBLAWBAR0 LBC local access window 0 base address register
#define CYGARC_REG_IMM_LBLAWAR0                 0x0C10            //LBLAWAR0 LBC local access window 0 attribute register
#define CYGARC_REG_IMM_LBLAWBAR1                0x0C28
#define CYGARC_REG_IMM_LBLAWAR1                 0x0C30
#define CYGARC_REG_IMM_LBLAWBAR2                0x0C48
#define CYGARC_REG_IMM_LBLAWAR2                 0x0C50
#define CYGARC_REG_IMM_LBLAWBAR3                0x0C68
#define CYGARC_REG_IMM_LBLAWAR3                 0x0C70
#define CYGARC_REG_IMM_LBLAWBAR4                0x0C88
#define CYGARC_REG_IMM_LBLAWAR4                 0x0C90
#define CYGARC_REG_IMM_LBLAWBAR5                0x0CA8
#define CYGARC_REG_IMM_LBLAWAR5                 0x0CB0
#define CYGARC_REG_IMM_LBLAWBAR6                0x0CC8
#define CYGARC_REG_IMM_LBLAWAR6                 0x0CD0
#define CYGARC_REG_IMM_LBLAWBAR7                0x0CE8
#define CYGARC_REG_IMM_LBLAWAR7                 0x0CF0
#define CYGARC_REG_IMM_LBLAWBAR8                0x0D08
#define CYGARC_REG_IMM_LBLAWAR8                 0x0D10
#define CYGARC_REG_IMM_LBLAWBAR9                0x0D28
#define CYGARC_REG_IMM_LBLAWAR9                 0x0D30
#define CYGARC_REG_IMM_LBLAWBAR10               0x0D48
#define CYGARC_REG_IMM_LBLAWAR10                0x0D50
#define CYGARC_REG_IMM_LBLAWBAR11               0x0D68
#define CYGARC_REG_IMM_LBLAWAR11                0x0D70

/*
 * e500 Coherency Module
 *
 */ 
#define CYGARC_REG_IMM_EEBACR                   0x1000      // CCB address configuration register
#define CYGARC_REG_IMM_EEBPCR                   0x1010      // CCB port configuration register
#define CYGARC_REG_EEBPCR_CPU1_EN               0x02000000  // Enable CPU1
#define CYGARC_REG_EEBPCR_CPU0_EN               0x01000000  // Enable CPU0
#define CYGARC_REG_IMM_ECM_IP1                  0x1BF8      // IP block Revision Register 1
#define CYGARC_REG_IMM_ECM_IP2                  0x1BFC      // IP block Revision Register 2
#define CYGARC_REG_IMM_EEDR                     0x1E00      // ECM error detect register
#define CYGARC_REG_IMM_EEER                     0x1E08      // ECM error enable register
#define CYGARC_REG_IMM_EEATR                    0x1E0C      // ECM error attributes capture register
#define CYGARC_REG_IMM_EELADR                   0x1E10      // ECM error low address capture register
#define CYGARC_REG_IMM_EEHADR                   0x1E14      // ECM error high address capture register

#define CYGARC_REG_EEBPCR_CPU_PRI_LEV0          0x0
#define CYGARC_REG_EEBPCR_CPU_PRI_LEV2          0x1
#define CYGARC_REG_EEBPCR_CPU_PRI_LEV1          0x2
#define CYGARC_REG_EEBPCR_CPU_PRI_RESERVED      0x3
#define CYGARC_REG_EEBPCR_SET_CPU_PRI(cpu, pri) (pri << (cpu << 2))


/*
 * DDR memory controller
 *
 */ 
#define CYGARC_REG_IMM_DDRC1                    0x2000            // DDR 1 memory controller offset
#define CYGARC_REG_IMM_DDRC2                    0x6000            // DDR 2 memory controller offset
/* relative to offset */
#define CYGARC_REG_IMM_CS0_BNDS                 0x0000            //CS0_BNDS Chip select 0 memory bounds
#define CYGARC_REG_IMM_CS1_BNDS                 0x0008            //CS1_BNDS Chip select 1 memory bounds
#define CYGARC_REG_IMM_CS2_BNDS                 0x0010            //CS2_BNDS Chip select 2 memory bounds
#define CYGARC_REG_IMM_CS3_BNDS                 0x0018            //CS3_BNDS Chip select 3 memory bounds
#define CYGARC_REG_IMM_CS0_CONFIG               0x0080            //CS0_CONFIG Chip select 0 configuration
#define CYGARC_REG_IMM_CS1_CONFIG               0x0084            //CS1_CONFIG Chip select 1 configuration
#define CYGARC_REG_IMM_CS2_CONFIG               0x0088            //CS2_CONFIG Chip select 2 configuration
#define CYGARC_REG_IMM_CS3_CONFIG               0x008C            //CS3_CONFIG Chip select 3 configuration
#define CYGARC_REG_IMM_CS0_CONFIG_2             0x00C0            //CS0_CONFIG_2 Chip select 0 configuration
#define CYGARC_REG_IMM_CS1_CONFIG_2             0x00C4            //CS1_CONFIG_2 Chip select 1 configuration
#define CYGARC_REG_IMM_CS2_CONFIG_2             0x00C8            //CS2_CONFIG_2 Chip select 2 configuration
#define CYGARC_REG_IMM_CS3_CONFIG_2             0x00CC            //CS3_CONFIG_2 Chip select 3 configuration
#define CYGARC_REG_IMM_TIMING_CFG_3             0x0100            //TIMING_CFG_0 DDR SDRAM timing configuration 3
#define CYGARC_REG_IMM_TIMING_CFG_0             0x0104            //TIMING_CFG_0 DDR SDRAM timing configuration 0
#define CYGARC_REG_IMM_TIMING_CFG_1             0x0108            //TIMING_CFG_0 DDR SDRAM timing configuration 1
#define CYGARC_REG_IMM_TIMING_CFG_2             0x010c            //TIMING_CFG_0 DDR SDRAM timing configuration 2
#define CYGARC_REG_IMM_DDR_SDRAM_CFG            0x0110            //DDR_SDRAM_CFG DDR SDRAM control configuration
#define CYGARC_REG_IMM_DDR_SDRAM_CFG_2          0x0114            //DDR_SDRAM_CFG_2 DDR SDRAM control configuration
#define CYGARC_REG_IMM_DDR_SDRAM_MODE           0x0118            //DDR_SDRAM_MODE DDR SDRAM mode configuration
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_2         0x011c            //DDR_SDRAM_MODE DDR SDRAM mode configuration
#define CYGARC_REG_IMM_DDR_MD_CNTL              0x0120            //SDRAM Mode Control
#define CYGARC_REG_IMM_DDR_SDRAM_INTERVAL       0x0124            //DDR_SDRAM_INTERVAL DDR SDRAM interval configuration
#define CYGARC_REG_IMM_DDR_DATA_INIT            0x0128            //SDRAM data initialization
#define CYGARC_REG_IMM_DDR_SDRAM_CLK_CNTL       0x0130            //DDR_SDRAM_CLK_CNTL DDR SDRAM clock control
#define CYGARC_REG_IMM_DDR_INIT_ADDR            0x0148            //SDRAM Training initialization address
#define CYGARC_REG_IMM_TIMING_CFG_4             0x0160            //TIMING_CFG_0 DDR SDRAM timing configuration 4
#define CYGARC_REG_IMM_TIMING_CFG_5             0x0164            //TIMING_CFG_0 DDR SDRAM timing configuration 5
#define CYGARC_REG_IMM_DDR_ZQ_CNTL              0x0170            //DDR ZQ calibration control
#define CYGARC_REG_IMM_DDR_WRLVL_CNTL           0x0174            //DDR write leveling control
#define CYGARC_REG_IMM_DDR_SDRAM_RCW_1          0x0180            //DDR Register Control Words 1
#define CYGARC_REG_IMM_DDR_SDRAM_RCW_2          0x0184            //DDR Register Control Words 2
#define CYGARC_REG_IMM_DDR_WRLVL_CNTL_2         0x0190            //DDR write leveling control 2
#define CYGARC_REG_IMM_DDR_WRLVL_CNTL_3         0x0194            //DDR write leveling control 3
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_3         0x0200            //DDR SDRAM mode configuration 3
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_4         0x0204            //DDR SDRAM mode configuration 4
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_5         0x0208            //DDR SDRAM mode configuration 5
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_6         0x020c            //DDR SDRAM mode configuration 6
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_7         0x0210            //DDR SDRAM mode configuration 7
#define CYGARC_REG_IMM_DDR_SDRAM_MODE_8         0x0214            //DDR SDRAM mode configuration 8

#define CYGARC_REG_SDRAM_CFG_MEM_EN             0x80000000
#define CYGARC_REG_SDRAM_CFG2_INIT              0x00000010

/*
 * Local bus
 *
 */ 
#if defined(CYGHWR_HAL_POWERPC_QUICC3_P1010)
#define CYGARC_REG_IMM_IFC_CSPR0                0x1E010
#define CYGARC_REG_IMM_IFC_CSPR1                0x1E01C
#define CYGARC_REG_IMM_IFC_CSPR2                0x1E028
#define CYGARC_REG_IMM_IFC_CSPR3                0x1E034
#define CYGARC_REG_IMM_IFC_AMASK0               0x1E0A0
#define CYGARC_REG_IMM_IFC_AMASK1               0x1E0AC
#define CYGARC_REG_IMM_IFC_AMASK2               0x1E0B8
#define CYGARC_REG_IMM_IFC_AMASK3               0x1E0C4
#define CYGARC_REG_IMM_IFC_CSOR0                0x1E130
#define CYGARC_REG_IMM_IFC_CSOR1                0x1E13C
#define CYGARC_REG_IMM_IFC_CSOR2                0x1E148
#define CYGARC_REG_IMM_IFC_CSOR3                0x1E154
#define CYGARC_REG_IMM_IFC_FTIM0_CS0            0x1E1C0
#define CYGARC_REG_IMM_IFC_FTIM1_CS0            0x1E1C4
#define CYGARC_REG_IMM_IFC_FTIM2_CS0            0x1E1C8
#define CYGARC_REG_IMM_IFC_FTIM3_CS0            0x1E1CC
#define CYGARC_REG_IMM_IFC_FTIM0_CS1            0x1E1F0
#define CYGARC_REG_IMM_IFC_FTIM1_CS1            0x1E1F4
#define CYGARC_REG_IMM_IFC_FTIM2_CS1            0x1E1F8
#define CYGARC_REG_IMM_IFC_FTIM3_CS1            0x1E1FC
#define CYGARC_REG_IMM_IFC_FTIM0_CS2            0x1E220
#define CYGARC_REG_IMM_IFC_FTIM1_CS2            0x1E224
#define CYGARC_REG_IMM_IFC_FTIM2_CS2            0x1E228
#define CYGARC_REG_IMM_IFC_FTIM3_CS2            0x1E22C
#define CYGARC_REG_IMM_IFC_FTIM0_CS3            0x1E250
#define CYGARC_REG_IMM_IFC_FTIM1_CS3            0x1E254
#define CYGARC_REG_IMM_IFC_FTIM2_CS3            0x1E258
#define CYGARC_REG_IMM_IFC_FTIM3_CS3            0x1E25C
#else
#define CYGARC_REG_IMM_BR0                      0x5000            //BR0 Options register 0
#define CYGARC_REG_IMM_OR0                      0x5004            //OR0 Options register 0
#define CYGARC_REG_IMM_BR1                      0x5008            //BR1 Options register 1
#define CYGARC_REG_IMM_OR1                      0x500C            //OR1 Options register 1
#define CYGARC_REG_IMM_BR2                      0x5010            //BR2 Options register 2
#define CYGARC_REG_IMM_OR2                      0x5014            //OR2 Options register 2
#define CYGARC_REG_IMM_BR3                      0x5018            //BR3 Options register 3
#define CYGARC_REG_IMM_OR3                      0x501c            //OR3 Options register 3
#define CYGARC_REG_IMM_BR4                      0x5020            //BR0 Options register 4
#define CYGARC_REG_IMM_OR4                      0x5024            //OR0 Options register 4
#define CYGARC_REG_IMM_BR5                      0x5028            //BR1 Options register 5
#define CYGARC_REG_IMM_OR5                      0x502C            //OR1 Options register 5
#define CYGARC_REG_IMM_BR6                      0x5030            //BR2 Options register 6
#define CYGARC_REG_IMM_OR6                      0x5034            //OR2 Options register 6
#define CYGARC_REG_IMM_BR7                      0x5038            //BR3 Options register 7
#define CYGARC_REG_IMM_OR7                      0x503C            //OR3 Options register 7
#define CYGARC_REG_IMM_LCRR                     0x50D4
#endif

/*
 * GPIO
 *
 */ 
#define CYGARC_REG_IMM_GP1DIR                   0x0C00            //GPIO direction register R/W 0x0000_0000 21.3.1/21-3
#define CYGARC_REG_IMM_GP1DR                    0x0C04            //GPIO open drain register R/W 0x0000_0000 21.3.2/21-3
#define CYGARC_REG_IMM_GP1DAT                   0x0C08            //GPIO data register R/W 0x0000_0000 21.3.3/21-4
#define CYGARC_REG_IMM_GP1IER                   0x0C0C            //GPIO interrupt event register R/W Undefined 21.3.4/21-4
#define CYGARC_REG_IMM_GP1IMR                   0x0C10            //GPIO interrupt mask register R/W 0x0000_0000 21.3.5/21-4
#define CYGARC_REG_IMM_GP1ICR                   0x0C14            //GPIO external interrupt control register R/W 0x0000_0000 21.3.6/21-5

#define CYGARC_REG_GPIO0                        0x80000000
#define CYGARC_REG_GPIO1                        0x40000000
#define CYGARC_REG_GPIO2                        0x20000000
#define CYGARC_REG_GPIO3                        0x10000000
#define CYGARC_REG_GPIO4                        0x08000000
#define CYGARC_REG_GPIO5                        0x04000000
#define CYGARC_REG_GPIO6                        0x02000000
#define CYGARC_REG_GPIO7                        0x01000000
 
#define CYGARC_REG_GPPORCR                      0xE0020


/* 
 *
 * PIC 
 *
 */
#define CYGARC_REG_IMM_PIC_GR_OFFSET            0x40000           // Offset of Global register section
// Feature reporting register
#define CYGARC_REG_IMM_FRR                      0x1000
// Global Configuration register
#define CYGARC_REG_IMM_GCR                      0x1020
// Global Configuration register field
#define CYGARC_REG_IMM_GCR_RST                  0x80000000       // Reset PIC
#define CYGARC_REG_IMM_GCR_M                    0x20000000       // PIC Mode
// Processor Core initialisation register
#define CYGARC_REG_IMM_PIR                      0x1090
// Interrupt vector registers
#define CYGARC_REG_IMM_IPIVPR0                  0x10A0
#define CYGARC_REG_IMM_IPIVPR1                  0x10B0
#define CYGARC_REG_IMM_IPIVPR2                  0x10C0  
#define CYGARC_REG_IMM_IPIVPR3                  0x10D0
#define CYGARC_REG_IMM_IPIVPR_MSK               0x80000000        // Mask interrupts source
#define CYGARC_REG_IMM_IPIVPR_A                 0x40000000        // Activity 
#define CYGARC_REG_IMM_IPIVPR_PRIORITY          0x000F0000        // Priority number mask
#define CYGARC_REG_IMM_IPIVPR_VECTOR            0x0000FFFF        // Vector number mask
#define CYGARC_REG_IMM_EXT_IRQ_HIGH_OR_UP       0x00800000        // Active high or rising edge
#define CYGARC_REG_IMM_EXT_IRQ_SENSE_LEVEL      0x00400000        // Level sensitive
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
#define CYGARC_REG_IMM_MER0                     0x1500
#define CYGARC_REG_IMM_MER1                     0x2500
// Shared message signaled interrupt
#define CYGARC_REG_IMM_MSIR0                    0x1600
#define CYGARC_REG_IMM_MSIR1                    0x1610
#define CYGARC_REG_IMM_MSIR2                    0x1620
#define CYGARC_REG_IMM_MSIR3                    0x1630
#define CYGARC_REG_IMM_MSIR4                    0x1640
#define CYGARC_REG_IMM_MSIR5                    0x1650
#define CYGARC_REG_IMM_MSIR6                    0x1660
#define CYGARC_REG_IMM_MSIR7                    0x1670
// Global timer vector/priority
#define CYGARC_REG_IMM_GTVPRA0                  0x1120
#define CYGARC_REG_IMM_GTVPRA1                  0x1160
#define CYGARC_REG_IMM_GTVPRA2                  0x11A0
#define CYGARC_REG_IMM_GTVPRA3                  0x11E0
#define CYGARC_REG_IMM_GTVPRB0                  0x2120
#define CYGARC_REG_IMM_GTVPRB1                  0x2160
#define CYGARC_REG_IMM_GTVPRB2                  0x21A0
#define CYGARC_REG_IMM_GTVPRB3                  0x21E0
// Global timer destination register
#define CYGARC_REG_IMM_GTDRA0                   0x1130
#define CYGARC_REG_IMM_GTDRA1                   0x1170
#define CYGARC_REG_IMM_GTDRA2                   0x11B0
#define CYGARC_REG_IMM_GTDRA3                   0x11F0
#define CYGARC_REG_IMM_GTDRB0                   0x2130
#define CYGARC_REG_IMM_GTDRB1                   0x2170
#define CYGARC_REG_IMM_GTDRB2                   0x21B0
#define CYGARC_REG_IMM_GTDRB3                   0x21F0
// Offset of interrupt source configuration register
#define CYGARC_REG_IMM_IRQ_SRC_CONF_OFFSET      0x50000
#define CYGARC_REG_IMM_IRQ_SRC_EXT_OFFSET_VPR   0x0000
#define CYGARC_REG_IMM_IRQ_SRC_EXT_OFFSET_IDR   0x0010
#define CYGARC_REG_IMM_IRQ_SRC_INT_OFFSET_VPR   0x0200
#define CYGARC_REG_IMM_IRQ_SRC_INT_OFFSET_IDR   0x0210
#define CYGARC_REG_IMM_IRQ_SRC_MSG_OFFSET_VPR   0x1600
#define CYGARC_REG_IMM_IRQ_SRC_MSG_OFFSET_IDR   0x1610
#define CYGARC_REG_IMM_IRQ_SRC_SHA_OFFSET_VPR   0x1C00
#define CYGARC_REG_IMM_IRQ_SRC_SHA_OFFSET_IDR   0x1C10


// Default interrupt setting
#define CYGARC_REG_PIC_VECPRI_MASK              0x80000000
#define CYGARC_REG_PIC_PRI_DEFAULT              8
#define CYGARC_REG_PIC_DEST_CPU0                0x00000001
#define CYGARC_REG_PIC_DEST_CPU1                0x00000002
#define CYGARC_REG_PIC_DEST_CPU_MASK            0x00000003
// Number of interrupt source
#define CYGARC_REG_PIC_INT_IRQ_NUM              64   // Internal interrupts
#define CYGARC_REG_PIC_EXT_IRQ_NUM              12   // External interrupts
#define CYGARC_REG_PIC_MSG_IRQ_NUM              8    // Message interrupts
#define CYGARC_REG_PIC_SHA_IRQ_NUM              8    // Shared message interrupts
#define CYGARC_REG_PIC_GTA_IRQ_NUM              4    // Global timer A
#define CYGARC_REG_PIC_GTB_IRQ_NUM              4    // Global timer B
#define CYGARC_REG_PIC_IPI_IRQ_NUM              4    // Interprocessor interrupts

// Per CPU registers
#define CYGARC_REG_IMM_PIC_PER_CPU_OFFSET       0x60000
// Interrupt acknowledge register
#define CYGARC_REG_IMM_IACK0                    0x00A0 
#define CYGARC_REG_IMM_IACK1                    0x10A0
#define CYGARC_REG_IMM_IACK_CPU0                CYGARC_REG_IMM_IACK0
#define CYGARC_REG_IMM_IACK_CPU1                CYGARC_REG_IMM_IACK1
// End of Interrupt
#define CYGARC_REG_IMM_IEO0                     0x00B0 
#define CYGARC_REG_IMM_IEO1                     0x10B0
#define CYGARC_REG_IMM_IEO_CPU0                 CYGARC_REG_IMM_IEO0
#define CYGARC_REG_IMM_IEO_CPU1                 CYGARC_REG_IMM_IEO1
// Current task priority register
#define CYGARC_REG_IMM_CTPR0                    0x0080 
#define CYGARC_REG_IMM_CTPR1                    0x1080
#define CYGARC_REG_IMM_CTPR_CPU0                CYGARC_REG_IMM_CTPR0
#define CYGARC_REG_IMM_CTPR_CPU1                CYGARC_REG_IMM_CTPR1
// Interprocessor dispatch register
#define CYGARC_REG_IMM_IPIDR0_0                 0x0040
#define CYGARC_REG_IMM_IPIDR1_0                 0x0050
#define CYGARC_REG_IMM_IPIDR2_0                 0x0060
#define CYGARC_REG_IMM_IPIDR3_0                 0x0070
#define CYGARC_REG_IMM_IPIDR0_1                 0x1040
#define CYGARC_REG_IMM_IPIDR1_1                 0x1050
#define CYGARC_REG_IMM_IPIDR2_1                 0x1060
#define CYGARC_REG_IMM_IPIDR3_1                 0x1070
#define CYGARC_REG_IMM_IPIDR0_CPU0              CYGARC_REG_IMM_IPIDR0_0
#define CYGARC_REG_IMM_IPIDR1_CPU0              CYGARC_REG_IMM_IPIDR1_0
#define CYGARC_REG_IMM_IPIDR2_CPU0              CYGARC_REG_IMM_IPIDR2_0
#define CYGARC_REG_IMM_IPIDR3_CPU0              CYGARC_REG_IMM_IPIDR3_0
#define CYGARC_REG_IMM_IPIDR0_CPU1              CYGARC_REG_IMM_IPIDR0_1
#define CYGARC_REG_IMM_IPIDR1_CPU1              CYGARC_REG_IMM_IPIDR1_1
#define CYGARC_REG_IMM_IPIDR2_CPU1              CYGARC_REG_IMM_IPIDR2_1
#define CYGARC_REG_IMM_IPIDR3_CPU1              CYGARC_REG_IMM_IPIDR3_1
// who I am register
#define CYGARC_REG_IMM_WHOAMI0                  0x0090 
#define CYGARC_REG_IMM_WHOAMI1                  0x1090
#define CYGARC_REG_IMM_WHOAMI_CPU0              CYGARC_REG_IMM_WHOAMI0
#define CYGARC_REG_IMM_WHOAMI_CPU1              CYGARC_REG_IMM_WHOAMI1

/* 
 *
 * L2/SRAM
 *
 */
#define CYGARC_REG_IMM_L2CTL                    0x0000             // L2 control register
#define CYGARC_REG_IMM_L2WAP                    0x0004             // L2 cache way allocation
#define CYGARC_REG_IMM_L2CEWAR0                 0x0010             // L2 cache external write address register 0
#define CYGARC_REG_IMM_L2CEWCR0                 0x0018             // L2 cache external write control register 0
#define CYGARC_REG_IMM_L2CEWAR1                 0x0020             // L2 cache external write address register 1
#define CYGARC_REG_IMM_L2CEWCR1                 0x0028             // L2 cache external write control register 1
#define CYGARC_REG_IMM_L2CEWAR2                 0x0030             // L2 cache external write address register 2
#define CYGARC_REG_IMM_L2CEWCR2                 0x0038             // L2 cache external write control register 2
#define CYGARC_REG_IMM_L2CEWAR3                 0x0040             // L2 cache external write address register 3
#define CYGARC_REG_IMM_L2CEWCR3                 0x0048             // L2 cache external write control register 4
#define CYGARC_REG_IMM_L2SRBAR0                 0x0100             // L2 cache memory mapped SRAM base address register 0
#define CYGARC_REG_IMM_L2SRBAREA0               0x0104
#define CYGARC_REG_IMM_L2SRBAR1                 0x0108             // L2 cache memory mapped SRAM base address register 1
#define CYGARC_REG_IMM_L2SRBAREA1               0x010C
#define CYGARC_REG_IMM_L2ERRINJHI               0x0E00             // L2 error injection mask high
#define CYGARC_REG_IMM_L2ERRINJLO               0x0E04             // L2 error injection mask low
#define CYGARC_REG_IMM_L2RRINJCTL               0x0E08             // L2 error injection mask control
#define CYGARC_REG_IMM_L2CAPTDATAHI             0x0E20             // L2 error data high capture
#define CYGARC_REG_IMM_L2CAPTDATALO             0x0E24             // L2 error data low capture
#define CYGARC_REG_IMM_L2CAPTECC                0x0E28             // L2 error syndrome
#define CYGARC_REG_IMM_L2ERRDET                 0x0E40             // L2 error detect
#define CYGARC_REG_IMM_L2ERRDIS                 0x0E44             // L2 error disable
#define CYGARC_REG_IMM_L2ERRINTEN               0x0E48             // L2 error interrupt enable
#define CYGARC_REG_IMM_L2ERRATTR                0x0E4C             // L2 error attribute capture
#define CYGARC_REG_IMM_L2ERRADDRL               0x0E50             // L2 error address capture low
#define CYGARC_REG_IMM_L2ERRADDRH               0x0E54             // L2 error address capture high
#define CYGARC_REG_IMM_L2ERRCTL                 0x0E58             // L2 error control

/* 
 *
 * Timers
 *
 */
#define CYGARC_REG_IMM_TFRRA                    0x10F0             // Time frequency Reporting
#define CYGARC_REG_IMM_TFRRB                    0x20F0             // Time frequency Reporting
#define CYGARC_REG_IMM_TCRA                     0x1300             // Time Control Registers
#define CYGARC_REG_IMM_TCRB                     0x2300             // Time Control Registers

#define CYGARC_REG_IMM_GTCCRA0                  0x1100             // Global Timer Current Count
#define CYGARC_REG_IMM_GTCCRA1                  0x1140    
#define CYGARC_REG_IMM_GTCCRA2                  0x1180    
#define CYGARC_REG_IMM_GTCCRA3                  0x11C0    
#define CYGARC_REG_IMM_GTCCRB0                  0x2100    
#define CYGARC_REG_IMM_GTCCRB1                  0x2140    
#define CYGARC_REG_IMM_GTCCRB2                  0x2180    
#define CYGARC_REG_IMM_GTCCRB3                  0x21C0    

#define CYGARC_REG_IMM_GTCCR_CI                 0x80000000         // Count inhibited

#define CYGARC_REG_IMM_GTBCRA0                  0x1110             // Global Timer Base Count
#define CYGARC_REG_IMM_GTBCRA1                  0x1150    
#define CYGARC_REG_IMM_GTBCRA2                  0x1190    
#define CYGARC_REG_IMM_GTBCRA3                  0x11D0    
#define CYGARC_REG_IMM_GTBCRB0                  0x2110    
#define CYGARC_REG_IMM_GTBCRB1                  0x2150    
#define CYGARC_REG_IMM_GTBCRB2                  0x2190    
#define CYGARC_REG_IMM_GTBCRB3                  0x21D0  

/* 
 *
 * TSEC memory mapping
 *
 */
#if defined(CYGHWR_HAL_POWERPC_QUICC3_P1010)
# define CYGARC_REG_IMM_TSEC1           0xB0000 //Base address for TSEC1
# define CYGARC_REG_IMM_TSEC2           0xB1000 //Base address for TSEC2
# define CYGARC_REG_IMM_TSEC3           0xB2000 //Base address for TSEC3
#else
# define CYGARC_REG_IMM_TSEC1           0x24000 //Base address for TSEC1
# define CYGARC_REG_IMM_TSEC2           0x25000 //Base address for TSEC2
# define CYGARC_REG_IMM_TSEC3           0x26000 //Base address for TSEC3
# define CYGARC_REG_IMM_TSEC4           0x27000 //Base address for TSEC4
#endif

#define CYGARC_REG_IMM_TSEC1_MIIMCFG    0x24520 //MII management configuration R/W 0x0000_0007 15.5.3.5.6/15-71
#define CYGARC_REG_IMM_TSEC1_MIIMCOM    0x24524 //MII management command R/W 0x0000_0000 15.5.3.5.7/15-72
#define CYGARC_REG_IMM_TSEC1_MIIMADD    0x24528 //MII management address R/W 0x0000_0000 15.5.3.5.8/15-73
#define CYGARC_REG_IMM_TSEC1_MIIMCON    0x2452C //MII management control WO 0x0000_0000 15.5.3.5.9/15-74
#define CYGARC_REG_IMM_TSEC1_MIIMSTAT   0x24530 //MII management status R 0x0000_0000 15.5.3.5.10/15-74
#define CYGARC_REG_IMM_TSEC1_MIIMIND    0x24534 //MII management indicator

#define CYGARC_REG_IMM_TSEC2_MIIMCFG    0x25520 //MII management configuration R/W 0x0000_0007 15.5.3.5.6/15-71
#define CYGARC_REG_IMM_TSEC2_MIIMCOM    0x25524 //MII management command R/W 0x0000_0000 15.5.3.5.7/15-72
#define CYGARC_REG_IMM_TSEC2_MIIMADD    0x25528 //MII management address R/W 0x0000_0000 15.5.3.5.8/15-73
#define CYGARC_REG_IMM_TSEC2_MIIMCON    0x2552C //MII management control WO 0x0000_0000 15.5.3.5.9/15-74
#define CYGARC_REG_IMM_TSEC2_MIIMSTAT   0x25530 //MII management status R 0x0000_0000 15.5.3.5.10/15-74
#define CYGARC_REG_IMM_TSEC2_MIIMIND    0x25534 //MII management indicator

#define CYGARC_REG_IMM_TSEC3_MIIMCFG    0x26520 //MII management configuration R/W 0x0000_0007 15.5.3.5.6/15-71
#define CYGARC_REG_IMM_TSEC3_MIIMCOM    0x26524 //MII management command R/W 0x0000_0000 15.5.3.5.7/15-72
#define CYGARC_REG_IMM_TSEC3_MIIMADD    0x26528 //MII management address R/W 0x0000_0000 15.5.3.5.8/15-73
#define CYGARC_REG_IMM_TSEC3_MIIMCON    0x2652C //MII management control WO 0x0000_0000 15.5.3.5.9/15-74
#define CYGARC_REG_IMM_TSEC3_MIIMSTAT   0x26530 //MII management status R 0x0000_0000 15.5.3.5.10/15-74
#define CYGARC_REG_IMM_TSEC3_MIIMIND    0x26534 //MII management indicator

#define CYGARC_REG_IMM_TSEC4_MIIMCFG    0x27520 //MII management configuration R/W 0x0000_0007 15.5.3.5.6/15-71
#define CYGARC_REG_IMM_TSEC4_MIIMCOM    0x27524 //MII management command R/W 0x0000_0000 15.5.3.5.7/15-72
#define CYGARC_REG_IMM_TSEC4_MIIMADD    0x27528 //MII management address R/W 0x0000_0000 15.5.3.5.8/15-73
#define CYGARC_REG_IMM_TSEC4_MIIMCON    0x2752C //MII management control WO 0x0000_0000 15.5.3.5.9/15-74
#define CYGARC_REG_IMM_TSEC4_MIIMSTAT   0x27530 //MII management status R 0x0000_0000 15.5.3.5.10/15-74
#define CYGARC_REG_IMM_TSEC4_MIIMIND    0x27534 //MII management indicator


/* 
 *
 * TSEC FIFO
 *
 */
#define CYGARC_REG_IMM_TSEC1_FIFOCFG           0x024A00 //Base address for TSEC1 FIFOCFG
#define CYGARC_REG_IMM_TSEC2_FIFOCFG           0x025A00 //Base address for TSEC2 FIFOCFG

#define CYGARC_REG_IMM_TSECx_FIFOCFG_IPG       0x00000008 // Minimum inter packet gap
#define CYGARC_REG_IMM_TSECx_FIFOCFG_RRX       0x00000010 // Enable reset of FIFO receive function
#define CYGARC_REG_IMM_TSECx_FIFOCFG_RTX       0x00000011 // Enable reset of FIFO transmit function
#define CYGARC_REG_IMM_TSECx_FIFOCFG_RXE       0x00000012 // Enable FIFO receive function
#define CYGARC_REG_IMM_TSECx_FIFOCFG_TXE       0x00000013 // Enable FIFO transmit function
#define CYGARC_REG_IMM_TSECx_FIFOCFG_LPB       0x00000017 // Loopback enable
#define CYGARC_REG_IMM_TSECx_FIFOCFG_RFC       0x00000018 // Enable receive flow control. Setting FFC overrides this bit
#define CYGARC_REG_IMM_TSECx_FIFOCFG_TFC       0x00000019 // Enable transmit flow control
#define CYGARC_REG_IMM_TSECx_FIFOCFG_FFC       0x0000001A // Force flow control. This can be used by software to stop reception on the FIFO interface
#define CYGARC_REG_IMM_TSECx_FIFOCFG_CRCAPP    0x0000001B // Append a CRC (CRC-32 algorithm, as per IEEE 802.3) to the end of every transmitted frame
#define CYGARC_REG_IMM_TSECx_FIFOCFG_CRCCHK    0x0000001C // Check the CRC (CRC-32 algorithm, as per IEEE 802.3) at the end of every frame
#define CYGARC_REG_IMM_TSECx_FIFOCFG_SIGM      0x0000001E // FIFO signaling mode. Determines how the GMII signals are interpreted as framing signals


#define CYGARC_REG_IMM_TSEC1_MACCFG2           0x024504 // Base address for TSEC1 MACCFG2
#define CYGARC_REG_IMM_TSEC2_MACCFG2           0x025504 // Base address for TSEC2 MACCFG2
#define CYGARC_REG_IMM_TSEC1_ECNTRL            0x024020 // Base address for TSEC1 ECNTRL
#define CYGARC_REG_IMM_TSEC2_ECNTRL            0x025020 // Base address for TSEC2 ECNTRL
#define CYGARC_REG_IMM_TSEC1_IMASK             0x024014 // Base address for TSEC1 IMASK
#define CYGARC_REG_IMM_TSEC2_IMASK             0x025014 // Base address for TSEC2 IMASK
#define CYGARC_REG_IMM_TSEC1_IEVENT            0x024010 // Base address for TSEC1 IEVENT
#define CYGARC_REG_IMM_TSEC2_IEVENT            0x025010 // Base address for TSEC2 IEVENT
#define CYGARC_REG_IMM_TSEC1_RCTRL             0x024300 // Base address for TSEC1 RCTRL
#define CYGARC_REG_IMM_TSEC2_RCTRL             0x024300 // Base address for TSEC2 RCTRL
#define CYGARC_REG_IMM_TSEC1_DMACTRL           0x02402C // Base address for TSEC1 DMACTRL
#define CYGARC_REG_IMM_TSEC2_DMACTRL           0x02502C // Base address for TSEC2 DMACTRL
#define CYGARC_REG_IMM_TSEC1_TQUEUE            0x024114 // Base address for TSEC1 TQUEUE
#define CYGARC_REG_IMM_TSEC2_TQUEUE            0x025114 // Base address for TSEC2 TQUEUE
#define CYGARC_REG_IMM_TSEC1_RQUEUE            0x024314 // Base address for TSEC1 RQUEUE
#define CYGARC_REG_IMM_TSEC2_RQUEUE            0x025314 // Base address for TSEC2 RQUEUE

// Adresses of TBASE0-TBASE7 registers for TSEC1 and TSEC2
#define CYGARC_REG_IMM_TSEC1_TBASE0            0x24204
#define CYGARC_REG_IMM_TSEC1_TBASE1            0x2420C
#define CYGARC_REG_IMM_TSEC1_TBASE2            0x24214
#define CYGARC_REG_IMM_TSEC1_TBASE3            0x2421C
#define CYGARC_REG_IMM_TSEC1_TBASE4            0x24224
#define CYGARC_REG_IMM_TSEC1_TBASE5            0x2422C
#define CYGARC_REG_IMM_TSEC1_TBASE6            0x24234
#define CYGARC_REG_IMM_TSEC1_TBASE7            0x2423C

#define CYGARC_REG_IMM_TSEC2_TBASE0            0x25204
#define CYGARC_REG_IMM_TSEC2_TBASE1            0x2520C
#define CYGARC_REG_IMM_TSEC2_TBASE2            0x25214
#define CYGARC_REG_IMM_TSEC2_TBASE3            0x2521C
#define CYGARC_REG_IMM_TSEC2_TBASE4            0x25224
#define CYGARC_REG_IMM_TSEC2_TBASE5            0x2522C
#define CYGARC_REG_IMM_TSEC2_TBASE6            0x25234
#define CYGARC_REG_IMM_TSEC2_TBASE7            0x2523C

// Adresses of RBASE0-RBASE7 registers for TSEC1 and TSEC2
#define CYGARC_REG_IMM_TSEC1_RBASE0            0x24404
#define CYGARC_REG_IMM_TSEC1_RBASE1            0x2440C
#define CYGARC_REG_IMM_TSEC1_RBASE2            0x24414
#define CYGARC_REG_IMM_TSEC1_RBASE3            0x2441C
#define CYGARC_REG_IMM_TSEC1_RBASE4            0x24424
#define CYGARC_REG_IMM_TSEC1_RBASE5            0x2442C
#define CYGARC_REG_IMM_TSEC1_RBASE6            0x24434
#define CYGARC_REG_IMM_TSEC1_RBASE7            0x2443C

#define CYGARC_REG_IMM_TSEC2_RBASE0            0x25404
#define CYGARC_REG_IMM_TSEC2_RBASE1            0x2540C
#define CYGARC_REG_IMM_TSEC2_RBASE2            0x25414
#define CYGARC_REG_IMM_TSEC2_RBASE3            0x2541C
#define CYGARC_REG_IMM_TSEC2_RBASE4            0x25424
#define CYGARC_REG_IMM_TSEC2_RBASE5            0x2542C
#define CYGARC_REG_IMM_TSEC2_RBASE6            0x25434
#define CYGARC_REG_IMM_TSEC2_RBASE7            0x2543C

// Addresses of RBPTR0-RBPTR7 registers for TSEC1 and TSEC2
#define CYGARC_REG_IMM_TSEC1_RBPTR0            0x24384
#define CYGARC_REG_IMM_TSEC1_RBPTR1            0x2438C
#define CYGARC_REG_IMM_TSEC1_RBPTR2            0x24394
#define CYGARC_REG_IMM_TSEC1_RBPTR3            0x2439C
#define CYGARC_REG_IMM_TSEC1_RBPTR4            0x243A4
#define CYGARC_REG_IMM_TSEC1_RBPTR5            0x243AC
#define CYGARC_REG_IMM_TSEC1_RBPTR6            0x243B4
#define CYGARC_REG_IMM_TSEC1_RBPTR7            0x243BC

#define CYGARC_REG_IMM_TSEC2_RBPTR0            0x25384
#define CYGARC_REG_IMM_TSEC2_RBPTR1            0x2538C
#define CYGARC_REG_IMM_TSEC2_RBPTR2            0x25394
#define CYGARC_REG_IMM_TSEC2_RBPTR3            0x2539C
#define CYGARC_REG_IMM_TSEC2_RBPTR4            0x253A4
#define CYGARC_REG_IMM_TSEC2_RBPTR5            0x253AC
#define CYGARC_REG_IMM_TSEC2_RBPTR6            0x253B4
#define CYGARC_REG_IMM_TSEC2_RBPTR7            0x253BC

// Addresses of TBPTR0-TBPTR7 registers for TSEC1 and TSEC2
#define CYGARC_REG_IMM_TSEC1_TBPTR0            0x24184
#define CYGARC_REG_IMM_TSEC1_TBPTR1            0x2418C
#define CYGARC_REG_IMM_TSEC1_TBPTR2            0x24194
#define CYGARC_REG_IMM_TSEC1_TBPTR3            0x2419C
#define CYGARC_REG_IMM_TSEC1_TBPTR4            0x241A4
#define CYGARC_REG_IMM_TSEC1_TBPTR5            0x241AC
#define CYGARC_REG_IMM_TSEC1_TBPTR6            0x241B4
#define CYGARC_REG_IMM_TSEC1_TBPTR7            0x241BC

#define CYGARC_REG_IMM_TSEC2_TBPTR0            0x25184
#define CYGARC_REG_IMM_TSEC2_TBPTR1            0x2518C
#define CYGARC_REG_IMM_TSEC2_TBPTR2            0x25194
#define CYGARC_REG_IMM_TSEC2_TBPTR3            0x2519C
#define CYGARC_REG_IMM_TSEC2_TBPTR4            0x251A4
#define CYGARC_REG_IMM_TSEC2_TBPTR5            0x251AC
#define CYGARC_REG_IMM_TSEC2_TBPTR6            0x251B4
#define CYGARC_REG_IMM_TSEC2_TBPTR7            0x251BC

/* 
 *
 * SEC
 *
 */
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
    

/*
 *
 * PCI Express
 *
 */
#if defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8572E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8569E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8568E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8548E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8547E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8545E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8543E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8540)  || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8536E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8535E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_MPC8533E) || \
       defined(CYGHWR_HAL_POWERPC_QUICC3_P1010)

#define CYGARC_PCIE_1_BASE_ADDR                0x0000A000
#define CYGARC_PCIE_2_BASE_ADDR                0x00009000
#define CYGARC_PCIE_3_BASE_ADDR                0x00008000

#define CYGARC_PCIE_CONFIG_ADDR                0x000
#define CYGARC_PCIE_CONFIG_DATA                0x004
#define CYGARC_PCIE_RESERVED                   0x008
#define CYGARC_PCIE_OTB_CPL_TOR                0x00C
#define CYGARC_PCIE_RTY_TOR                    0x010

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

#endif

/*  
 *
 * DMA channels
 *
 */
#define CYGARC_REG_IMM_DMA1_CH1             0x00021100
#define CYGARC_REG_IMM_DMA1_CH2             0x00021180
#define CYGARC_REG_IMM_DMA1_CH3             0x00021200
#define CYGARC_REG_IMM_DMA1_CH4             0x00021280

/*
 *
 * General
 *
 */
#define CYGARC_REG_IMM_PORPLLSR             0x000E0000
#define CYGARC_REG_IMM_PORBMSR              0x000E0004
#define CYGARC_REG_IMM_PORIMPSCR            0x000E0008
#define CYGARC_REG_IMM_PORDEVSR             0x000E000C
#define CYGARC_REG_IMM_PORDBGMSR            0x000E0010
#define CYGARC_REG_IMM_PORDEVSR2            0x000E0014
#define CYGARC_REG_IMM_DEVDISR              0x000E0070
#define CYGARC_REG_IMM_MCPSUMR              0x000E0090
#define CYGARC_REG_IMM_RSTRSCR              0x000E0094
#define CYGARC_REG_IMM_ECTRSTCR             0x000E0098
#define CYGARC_REG_IMM_PVR                  0x000E00A0
#define CYGARC_REG_IMM_SVR                  0x000E00A4
#define CYGARC_REG_IMM_RSTCR                0x000E00B0
#define CYGARC_REG_IMM_SRDS1CR              0x000E0F04
#define CYGARC_REG_IMM_SRDS2CR              0x000E0F10
#define CYGARC_REG_IMM_RSTCR_HRESET_REQ     0x00000002

/*  
 *
 * MMU
 *
 */
// L1 cache
// L1CSR0
#define CYGARC_REG_L1CSR0_CE       0x00000001 // Data Cache enable
#define CYGARC_REG_L1CSR0_CFI      0x00000002 // Data Cache flash invalidate
#define CYGARC_REG_L1CSR0_CLFR     0x00000100 // Data Cache lock bits flash reset
#define CYGARC_REG_L1CSR0_CLO      0x00000200 // Data Cache lock overflow
#define CYGARC_REG_L1CSR0_CUL      0x00000400 // Data Cache unable to lock
#define CYGARC_REG_L1CSR0_CSLC     0x00000800 // Data Cache unable to lock
#define CYGARC_REG_L1CSR0_CPI      0x00008000 // Data Cache parity error injection enable
#define CYGARC_REG_L1CSR0_CPE      0x00010000 // Data Cache parity enable

// L1CSR1
#define CYGARC_REG_L1CSR1_ICE      0x00000001 // Instruction Cache enable
#define CYGARC_REG_L1CSR1_ICFI     0x00000002 // Instruction Cache flash invalidate
#define CYGARC_REG_L1CSR1_ICLFR    0x00000100 // Instruction Cache lock bits flash reset
#define CYGARC_REG_L1CSR1_ICLO     0x00000200 // Instruction Cache lock overflow
#define CYGARC_REG_L1CSR1_ICUL     0x00000400 // Instruction Cache unable to lock
#define CYGARC_REG_L1CSR1_ICSLC    0x00000800 // Instruction Cache snoop lock clear
#define CYGARC_REG_L1CSR1_ICPI     0x00008000 // Instruction Cache parity error injection enable
#define CYGARC_REG_L1CSR1_ICPE     0x00010000 // Instruction Cache parity enable

// L2 cache
#define CYGARC_REG_L2CTL           0x0

#define CYGARC_REG_L2CTL_STACHCTL  0x00000003 // L2 stash configuration
#define CYGARC_REG_L2CTL_STASHDIS  0x00000008 // L2 stach allocate disable
#define CYGARC_REG_L2CTL_LFRID     0x00000300 // L2 cache lock bits flash reset instruction or data
#define CYGARC_REG_L2CTL_LFR       0x00000400 // L2 cache lock bits flash reset
#define CYGARC_REG_L2CTL_SLC       0x00001000 // L2 snoop lock clear
#define CYGARC_REG_L2CTL_L2LO      0x00002000 // L2 cache lock overflow
#define CYGARC_REG_L2CTL_SRAM      0x00070000 // L2 SRAM configuration
#define CYGARC_REG_L2CTL_INTDIS    0x00080000 // Cache read intervention disable
#define CYGARC_REG_L2CTL_IO        0x00200000 // L2 Instruction only
#define CYGARC_REG_L2CTL_DO        0x00400000 // L2 Data only
#define CYGARC_REG_L2CTL_SIZ       0x30000000 // L2 SRAM size
#define CYGARC_REG_L2CTL_I         0x40000000 // L2 flash invalidate
#define CYGARC_REG_L2CTL_E         0x80000000 // L2 Enable

#define CYGARC_L2CACHE_256K        1
#define CYGARC_L2CACHE_512K        2
#define CYGARC_L2CACHE_1024K       3

#define CYGARC_L2CACHE_SRAM_NO     0            // No SRAM
#define CYGARC_L2CACHE_SRAM        1            // Entire cache is single SRAM
#define CYGARC_L2CACHE_SRAM_HALF   2            // Half of the cache is SRAM
#define CYGARC_L2CACHE_SRAM_BOTH   3            // Both half of the cache is SRAM
// etc...

#define CYGARC_REG_L2CTL_GET_SIZ(x)  ((x & CYGARC_REG_L2CTL_SIZ) >> 28)
#define CYGARC_REG_L2CTL_SET_SRAM(x) ((x << 16) & CYGARC_REG_L2CTL_SRAM) 

// HID0
#define CYGARC_REG_HID0_NOPTI      0x00000001 // No-op the data and instruction cache touch instructions
#define CYGARC_REG_HID0_DCFA       0x00000040 // Data cache flush assist
#define CYGARC_REG_HID0_EMAS7      0x00000080 // Enable MAS7 update
#define CYGARC_REG_HID0_SEL_TBCLK  0x00002000 // Select time based clock
#define CYGARC_REG_HID0_TBEN       0x00004000 // Time base enable
#define CYGARC_REG_HID0_NAP        0x00400000 // Nap power management mode
#define CYGARC_REG_HID0_DOZE       0x00800000 // Doze power management mode
#define CYGARC_REG_HID0_EMCP       0x80000000 // Enable machine check pin

// HID1
#define CYGARC_REG_HID1_MID        0x0000000f // Unique core identifier
#define CYGARC_REG_HID1_ABE        0x00001000 // Address broadcast enable
#define CYGARC_REG_HID1_ASTME      0x00002000 // Address bus streaming mode enable
#define CYGARC_REG_HID1_RFXE       0x00020000 // Read fault exception table
#define CYGARC_REG_HID1_PLL_CFG    0x3f000000 // From config pin (PLL ratio)
#define CYGARC_REG_HID1_PLL_MODE   0xc0000000 // From config pin (PLL mode)


#ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
// L1CSR0
#define L1CSR0_CE       CYGARC_REG_L1CSR0_CE
#define L1CSR0_CFI      CYGARC_REG_L1CSR0_CFI
#define L1CSR0_CLFR     CYGARC_REG_L1CSR0_CLFR
#define L1CSR0_CLO      CYGARC_REG_L1CSR0_CLO
#define L1CSR0_CUL      CYGARC_REG_L1CSR0_CUL
#define L1CSR0_CSLC     CYGARC_REG_L1CSR0_CSLC
#define L1CSR0_CPI      CYGARC_REG_L1CSR0_CPI
#define L1CSR0_CPE      CYGARC_REG_L1CSR0_CPE

// L1CSR1
#define L1CSR1_ICE      CYGARC_REG_L1CSR1_ICE
#define L1CSR1_ICFI     CYGARC_REG_L1CSR1_ICFI
#define L1CSR1_ICLFR    CYGARC_REG_L1CSR1_ICLFR
#define L1CSR1_ICLO     CYGARC_REG_L1CSR1_ICLO
#define L1CSR1_ICUL     CYGARC_REG_L1CSR1_ICUL
#define L1CSR1_ICSLC    CYGARC_REG_L1CSR1_ICSLC
#define L1CSR1_ICPI     CYGARC_REG_L1CSR1_ICPI
#define L1CSR1_ICPE     CYGARC_REG_L1CSR1_ICPE

// HID0 & HID1
#define HID0_NOPTI      CYGARC_REG_HID0_NOPTI
#define HID0_DCFA       CYGARC_REG_HID0_DCFA
#define HID0_EMAS7      CYGARC_REG_HID0_EMAS7
#define HID0_SEL_TBCLK  CYGARC_REG_HID0_SEL_TBCLK
#define HID0_TBEN       CYGARC_REG_HID0_TBEN
#define HID0_NAP        CYGARC_REG_HID0_NAP
#define HID0_DOZE       CYGARC_REG_HID0_DOZE
#define HID0_EMCP       CYGARC_REG_HID0_EMCP

#define HID1_MID        CYGARC_REG_HID1_MID
#define HID1_ABE        CYGARC_REG_HID1_ABE
#define HID1_ASTME      CYGARC_REG_HID1_ASTME
#define HID1_RFXE       CYGARC_REG_HID1_RFXE
#define HID1_PLL_CFG    CYGARC_REG_HID1_PLL_CFG
#define HID1_PLL_MODE   CYGARC_REG_HID1_PLL_MODE

// L2 cache
#define L2CTL_STACHCTL  CYGARC_REG_L2CTL_STACHCTL
#define L2CTL_STASHDIS  CYGARC_REG_L2CTL_STASHDIS
#define L2CTL_LFRID     CYGARC_REG_L2CTL_LFRID
#define L2CTL_LFR       CYGARC_REG_L2CTL_LFR
#define L2CTL_SLC       CYGARC_REG_L2CTL_SLC
#define L2CTL_L2LO      CYGARC_REG_L2CTL_L2LO
#define L2CTL_SRAM      CYGARC_REG_L2CTL_SRAM
#define L2CTL_INTDIS    CYGARC_REG_L2CTL_INTDIS
#define L2CTL_IO        CYGARC_REG_L2CTL_IO
#define L2CTL_DO        CYGARC_REG_L2CTL_DO
#define L2CTL_SIZ       CYGARC_REG_L2CTL_SIZ
#define L2CTL_I         CYGARC_REG_L2CTL_I
#define L2CTL_E         CYGARC_REG_L2CTL_E

#define L2CTL_SET_SIZ      CYGARC_REG_L2CTL_SET_SIZ
#define L2CACHE_256K       CYGARC_L2CACHE_256K
#define L2CACHE_512K       CYGARC_L2CACHE_512K
#define L2CACHE_1024K      CYGARC_L2CACHE_1024K
#define L2CTL_SET_SRAM     CYGARC_REG_L2CTL_SET_SRAM
#define L2CACHE_SRAM_NO    CYGARC_L2CACHE_SRAM_NO
#define L2CACHE_SRAM       CYGARC_L2CACHE_SRAM
#define L2CACHE_SRAM_HALF  CYGARC_L2CACHE_SRAM_HALF
#define L2CACHE_SRAM_BOTH  CYGARC_L2CACHE_SRAM_BOTH

#endif // ifdef CYGARC_HAL_COMMON_EXPORT_CPU_MACROS

#endif // ifdef CYGONCE_HAL_VAR_REGS_H
// End of var_regs.h
