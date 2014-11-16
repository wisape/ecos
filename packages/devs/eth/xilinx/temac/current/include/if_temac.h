#ifndef CYGONCE_DEV_ETH_XILINX_TEMAC_IO_H
#define CYGONCE_DEV_ETH_XILINX_TEMAC_IO_H
//==========================================================================
//
//      if_temac.h
//
//      Ethernet driver for Xilinx TEMAC controller
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
// Date:         2012-01-25
// Purpose:
// Description:  Ethernet driver for Xilinx TEMAC controller
//
//
//####DESCRIPTIONEND####
//
//==========================================================================


// Directly Addressable Memory and Soft Registers

// The TEMAC1 registers can be accessed by adding 0x40 to the TEMAC0
// registers. Therefore, all registers in the TEMAC driver are accessed
// with TEMAC0 register name. The offset of 0x40 is part of the TEMAC1
// base address definition-

#define CYGHWR_XILINX_TEMAC0_REG_OFFSET              0x0000   // Offset of TEMAC0 Registers
#define CYGHWR_XILINX_TEMAC1_REG_OFFSET              0x0040   // Offset of TEMAC1 Registers

#define CYGHWR_XILINX_TEMAC0_TBL_OFFSET              0x0000   // Offset of TEMAC0 table and statistics
#define CYGHWR_XILINX_TEMAC1_TBL_OFFSET              0x40000  // Offset of TEMAC1 tables and statistics

#define CYGHWR_XILINX_TEMAC_RAF0                     0x0000   // Reset and Address Filter
#define CYGHWR_XILINX_TEMAC_RAF1                     0x0040
#define CYGHWR_XILINX_TEMAC_TPF0                     0x0004   // Transmit Pause Frame
#define CYGHWR_XILINX_TEMAC_TPF1                     0x0044
#define CYGHWR_XILINX_TEMAC_IFGP0                    0x0008   // Transmit Inter-Frame Gap Adjustment Registers
#define CYGHWR_XILINX_TEMAC_IFGP1                    0x0048
#define CYGHWR_XILINX_TEMAC_IS0                      0x000C   // Interrupt Status Registers
#define CYGHWR_XILINX_TEMAC_IS1                      0x004C
#define CYGHWR_XILINX_TEMAC_IP0                      0x0010   // Interrupt Pending Registers
#define CYGHWR_XILINX_TEMAC_IP1                      0x0050
#define CYGHWR_XILINX_TEMAC_IE0                      0x0014   // Interrupt Enable Registers
#define CYGHWR_XILINX_TEMAC_IE1                      0x0054
#define CYGHWR_XILINX_TEMAC_TTAG0                    0x0018   // Transmit VLAN Tag Register
#define CYGHWR_XILINX_TEMAC_TTAG1                    0x0058
#define CYGHWR_XILINX_TEMAC_RTAG0                    0x001C   // Receive VLAN Tag Register
#define CYGHWR_XILINX_TEMAC_RTAG1                    0x005C
#define CYGHWR_XILINX_TEMAC_UAWL0                    0x0030   // Unicast Address Word Lower Register
#define CYGHWR_XILINX_TEMAC_UAWL1                    0x0070
#define CYGHWR_XILINX_TEMAC_UAWU0                    0x0034   // Unicast Address Word Upper Register
#define CYGHWR_XILINX_TEMAC_UAWU1                    0x0074
#define CYGHWR_XILINX_TEMAC_TPID00                   0x0038   // VLAN TPID Word 0 Register
#define CYGHWR_XILINX_TEMAC_TPID10                   0x0078
#define CYGHWR_XILINX_TEMAC_TPID01                   0x003C   // VLAN TPID Word 1 Register
#define CYGHWR_XILINX_TEMAC_TPID11                   0x007C

#define CYGHWR_XILINX_TEMAC_STATS_CNT0               0x200    // Statistic counter
#define CYGHWR_XILINX_TEMAC_STATS_CNT1               0x40200
#define CYGHWR_XILINX_TEMAC_TX_VLAN_TBL0_S           0x4000   // Transmit VLAN Table entry w/ all fields
#define CYGHWR_XILINX_TEMAC_TX_VLAN_TBL1_S           0x44000
#define CYGHWR_XILINX_TEMAC_RX_VLAN_TBL0_S           0x8000   // Receive VLAN Table entry w/ all fields
#define CYGHWR_XILINX_TEMAC_RX_VLAN_TBL1_S           0x48000
#define CYGHWR_XILINX_TEMAC_MULT_ADDR_TBL0_S         0x20000  // Multicast Address Table entry start
#define CYGHWR_XILINX_TEMAC_MULT_ADDR_TBL1_S         0x60000



// Reset and Address Filter

#define CYGHWR_XILINX_TEMAC_RAF_HARD_RST             BIT_(0)
#define CYGHWR_XILINX_TEMAC_RAF_MCAST_REJ            BIT_(1)
#define CYGHWR_XILINX_TEMAC_RAF_BCAST_REJ            BIT_(2)
#define CYGHWR_XILINX_TEMAC_RAF_TX_VTAG_MODE(_x)     VALUE_(3, _x)
#define CYGHWR_XILINX_TEMAC_RAF_RX_VTAG_MODE(_x)     VALUE_(5, _x)
#define CYGHWR_XILINX_TEMAC_RAF_TX_VTAG_SMODE(_x)    VALUE_(7, _x)
#define CYGHWR_XILINX_TEMAC_RAF_RX_VTAG_SMODE(_x)    VALUE_(9, _x)
#define CYGHWR_XILINX_TEMAC_RAF_NEW_FNC_EN           BIT_(11)
#define CYGHWR_XILINX_TEMAC_RAF_ENHANCED_MCAST_FILT  BIT_(12)
#define CYGHWR_XILINX_TEMAC_RAF_STATS_CNT_RESET      BIT_(13)

// Interrupt Status Registers

#define CYGHWR_XILINX_TEMAC_IS_HARD_ACS_CMPLT        BIT_(0)
#define CYGHWR_XILINX_TEMAC_IS_AUTO_NEG              BIT_(1)
#define CYGHWR_XILINX_TEMAC_IS_RX_CMPLT              BIT_(2)
#define CYGHWR_XILINX_TEMAC_IS_RX_REJECT             BIT_(3)
#define CYGHWR_XILINX_TEMAC_IS_RX_FIFO_OVR           BIT_(4)
#define CYGHWR_XILINX_TEMAC_IS_TX_CMPLT              BIT_(5)
#define CYGHWR_XILINX_TEMAC_IS_RX_DCM_LOCK           BIT_(6)
#define CYGHWR_XILINX_TEMAC_IS_MGT_READY             BIT_(7)

// Interrupt Pending Registers

#define CYGHWR_XILINX_TEMAC_IP_HARD_ACS_CMPLT        BIT_(0)
#define CYGHWR_XILINX_TEMAC_IP_AUTO_NEG              BIT_(1)
#define CYGHWR_XILINX_TEMAC_IP_RX_CMPLT              BIT_(2)
#define CYGHWR_XILINX_TEMAC_IP_RX_REJECT             BIT_(3)
#define CYGHWR_XILINX_TEMAC_IP_RX_FIFO_OVR           BIT_(4)
#define CYGHWR_XILINX_TEMAC_IP_TX_CMPLT              BIT_(5)
#define CYGHWR_XILINX_TEMAC_IP_RX_DCM_LOCK           BIT_(6)
#define CYGHWR_XILINX_TEMAC_IP_MGT_READY             BIT_(7)

// Interrupt Enable Registers

#define CYGHWR_XILINX_TEMAC_IE_HARD_ACS_CMPLT        BIT_(0)
#define CYGHWR_XILINX_TEMAC_IE_AUTO_NEG              BIT_(1)
#define CYGHWR_XILINX_TEMAC_IE_RX_CMPLT              BIT_(2)
#define CYGHWR_XILINX_TEMAC_IE_RX_REJECT             BIT_(3)
#define CYGHWR_XILINX_TEMAC_IE_RX_FIFO_OVR           BIT_(4)
#define CYGHWR_XILINX_TEMAC_IE_TX_CMPLT              BIT_(5)
#define CYGHWR_XILINX_TEMAC_IE_RX_DCM_LOCK           BIT_(6)
#define CYGHWR_XILINX_TEMAC_IE_MGT_READY             BIT_(7)

#define CYGHWR_XILINX_TEMAC_IE_DEFAULT_MASK                                 \
                                     ( CYGHWR_XILINX_TEMAC_IE_RX_CMPLT    | \
                                       CYGHWR_XILINX_TEMAC_IE_RX_REJECT   | \
                                       CYGHWR_XILINX_TEMAC_IE_RX_FIFO_OVR | \
                                       CYGHWR_XILINX_TEMAC_IE_TX_CMPLT )

#define CYGHWR_XILINX_TEMAC_MULT_ADDR_TBL_EN         BIT_(0)

// Transmit VLAN Table entry w/ all fields

#define CYGHWR_XILINX_TEMAC_TX_VLAN_TBL_TAG_EN       BIT_(0)
#define CYGHWR_XILINX_TEMAC_TX_VLAN_TBL_STRP_EN      BIT_(1)
#define CYGHWR_XILINX_TEMAC_TX_VLAN_ID(_x)           VALUE_(2, _x)

// Receive VLAN Table entry w/ all fields

#define CYGHWR_XILINX_TEMAC_RX_VLAN_TBL_TAG_EN       BIT_(0)
#define CYGHWR_XILINX_TEMAC_RX_VLAN_TBL_STRP_EN      BIT_(1)
#define CYGHWR_XILINX_TEMAC_RX_VLAN_ID(_x)           VALUE_(2, _x)

// Statistics specific registers

struct temac_stats
{
    cyg_uint32 txbl;
    cyg_uint32 txbu;
    cyg_uint32 rxbl;
    cyg_uint32 rxbu;
    cyg_uint32 rxundrl;
    cyg_uint32 rxundru;
    cyg_uint32 rxfragl;
    cyg_uint32 rxfragu;
    cyg_uint32 rx64bl;
    cyg_uint32 rx64bu;
    cyg_uint32 rx65b127l;
    cyg_uint32 rx65b127u;
    cyg_uint32 rx128b255l;
    cyg_uint32 rx128b255u;
    cyg_uint32 rx256b511l;
    cyg_uint32 rx256b511u;
    cyg_uint32 rx512b1023l;
    cyg_uint32 rx512b1023u;
    cyg_uint32 rx1024bl;
    cyg_uint32 rx1024bu;
    cyg_uint32 rxovrl;
    cyg_uint32 rxovru;
    cyg_uint32 tx64bl;
    cyg_uint32 tx64bu;
    cyg_uint32 tx65b127l;
    cyg_uint32 tx65b127u;
    cyg_uint32 tx128b255l;
    cyg_uint32 tx128b255u;
    cyg_uint32 tx256b511l;
    cyg_uint32 tx256b511u;
    cyg_uint32 tx512b1023l;
    cyg_uint32 tx512b1023u;
    cyg_uint32 tx1024bl;
    cyg_uint32 tx1024bu;
    cyg_uint32 txovrl;
    cyg_uint32 txovru;
    cyg_uint32 rxfl;
    cyg_uint32 rxfu;
    cyg_uint32 rxfcserl;
    cyg_uint32 rxfcseru;
    cyg_uint32 rxbcstfl;
    cyg_uint32 rxbcstfu;
    cyg_uint32 rxmcstfl;
    cyg_uint32 rxmcstfu;
    cyg_uint32 rxctrlfl;
    cyg_uint32 rxctrlfu;
    cyg_uint32 rxlterl;
    cyg_uint32 rxlteru;
    cyg_uint32 rxvlanfl;
    cyg_uint32 rxvlanfu;
    cyg_uint32 rxpfl;
    cyg_uint32 rxpfu;
    cyg_uint32 rxuopfl;
    cyg_uint32 rxuopfu;
    cyg_uint32 txfl;
    cyg_uint32 txfu;
    cyg_uint32 txbcstfl;
    cyg_uint32 txbcstfu;
    cyg_uint32 txmcstfl;
    cyg_uint32 txmcstfu;
    cyg_uint32 txundrerl;
    cyg_uint32 txundreru;
    cyg_uint32 txctrfl;
    cyg_uint32 txctrfu;
    cyg_uint32 txvlanfl;
    cyg_uint32 txvlanfu;
    cyg_uint32 txpfl;
    cyg_uint32 txpfu;
};


// Directly Addressable TEMAC Registers

#define CYGHWR_XILINX_TEMAC_MSW0                     0x0020   // Most Significant Word Data (shared) Register (MSW)
#define CYGHWR_XILINX_TEMAC_MSW1                     0x0060
#define CYGHWR_XILINX_TEMAC_LSW0                     0x0024   // Least Significant Word Data (shared) Register (LSW)
#define CYGHWR_XILINX_TEMAC_LSW1                     0x0064
#define CYGHWR_XILINX_TEMAC_CTL0                     0x0028   // Control (shared) Register (CTL)
#define CYGHWR_XILINX_TEMAC_CTL1                     0x0068
#define CYGHWR_XILINX_TEMAC_RDY0                     0x002C   // Ready Status Register Ethernet Interface (RDY)
#define CYGHWR_XILINX_TEMAC_RDY1                     0x006C

// Control (shared) Register (CTL)

#define CYGHWR_XILINX_TEMAC_CTL_ADDR_CODE(_x)        VALUE_(0, _x)
#define CYGHWR_XILINX_TEMAC_CTL_WEN                  BIT_(15)

// Ready Status Register Ethernet Interface (RDY)

#define CYGHWR_XILINX_TEMAC_RDY_FABR_RR              BIT_(0)
#define CYGHWR_XILINX_TEMAC_RDY_MIIM_RR              BIT_(1)
#define CYGHWR_XILINX_TEMAC_RDY_MIIM_WR              BIT_(2)
#define CYGHWR_XILINX_TEMAC_RDY_AF_RR                BIT_(3)
#define CYGHWR_XILINX_TEMAC_RDY_AF_WR                BIT_(4)
#define CYGHWR_XILINX_TEMAC_RDY_CFG_RR               BIT_(5)
#define CYGHWR_XILINX_TEMAC_RDY_CFG_WR               BIT_(6)
#define CYGHWR_XILINX_TEMAC_RDY_CFG_HARD_ACS_RDY     BIT_(16)


// Indirectly Addressable TEMAC Registers

#define CYGHWR_XILINX_TEMAC_RCW0                     0x200    // Receive Configuration Word 0
#define CYGHWR_XILINX_TEMAC_RCW1                     0x240    // Receive Configuration Word 1
#define CYGHWR_XILINX_TEMAC_TC                       0x280    // Transmit Configuration (TC)
#define CYGHWR_XILINX_TEMAC_FCC                      0x2C0    // Flow Control Configuration (FCC)
#define CYGHWR_XILINX_TEMAC_EMMC                     0x300    // Ethernet MAC Mode Configuration (EMMC)
#define CYGHWR_XILINX_TEMAC_PHYC                     0x320    // RGMII/SGMII Configuration (PHYC)
#define CYGHWR_XILINX_TEMAC_MC                       0x340    // Management Configuration (MC)
#define CYGHWR_XILINX_TEMAC_UAW0                     0x380    // Unicast Address Word 0 (UAW0)
#define CYGHWR_XILINX_TEMAC_UAW1                     0x384    // Unicast Address Word 1 (UAW1)
#define CYGHWR_XILINX_TEMAC_MAW0                     0x388    // Multicast Address Table Access Word 0 (MAW0)
#define CYGHWR_XILINX_TEMAC_MAW1                     0x38C    // Multicast Address Table Access Word 1 (MAW1)
#define CYGHWR_XILINX_TEMAC_AFM                      0x390    // Address Filter Mode (AFM)
#define CYGHWR_XILINX_TEMAC_TIS                      0x3A0    // Interrupt Status (TIS)
#define CYGHWR_XILINX_TEMAC_TIE                      0x3A4    // Interrupt Enable (TIE)
#define CYGHWR_XILINX_TEMAC_MIIMWD                   0x3B0    // MII Management Write Data (MIIMWD)
#define CYGHWR_XILINX_TEMAC_MIIMAI                   0x3B4    // MII Management Access Initiate

// Receive Configuration Word 1

#define CYGHWR_XILINX_TEMAC_RCW1_RST                 BIT_(31)
#define CYGHWR_XILINX_TEMAC_RCW1_JUM_EN              BIT_(30)
#define CYGHWR_XILINX_TEMAC_RCW1_FCS                 BIT_(29)
#define CYGHWR_XILINX_TEMAC_RCW1_RX_EN               BIT_(28)
#define CYGHWR_XILINX_TEMAC_RCW1_VLAN_EN             BIT_(27)
#define CYGHWR_XILINX_TEMAC_RCW1_HD                  BIT_(26)
#define CYGHWR_XILINX_TEMAC_RCW1_LT_EN               BIT_(25)
#define CYGHWR_XILINX_TEMAC_RCW1_PAUSE_ADDR(_x)     VALUE_(0, _x)

// Transmit Configuration (TC)

#define CYGHWR_XILINX_TEMAC_TC_RST                   BIT_(31)
#define CYGHWR_XILINX_TEMAC_TC_JUM_EN                BIT_(30)
#define CYGHWR_XILINX_TEMAC_TC_FCS                   BIT_(29)
#define CYGHWR_XILINX_TEMAC_TC_TX_EN                 BIT_(28)
#define CYGHWR_XILINX_TEMAC_TC_VLAN_EN               BIT_(27)
#define CYGHWR_XILINX_TEMAC_TC_HD                    BIT_(26)
#define CYGHWR_XILINX_TEMAC_TC_IFG_EN                BIT_(25)

// Flow Control Configuration (FCC)

#define CYGHWR_XILINX_TEMAC_FCC_FCTX                 BIT_(30)
#define CYGHWR_XILINX_TEMAC_FCC_FCRX                 BIT_(29)

// Ethernet MAC Mode Configuration (EMMC)

#define CYGHWR_XILINX_TEMAC_EMMC_LINK_SPEED(_x)      VALUE_(30, _x)
#define CYGHWR_XILINX_TEMAC_EMMC_RGMII               BIT_(29)
#define CYGHWR_XILINX_TEMAC_EMMC_SGMII               BIT_(28)
#define CYGHWR_XILINX_TEMAC_EMMC_GPCS                BIT_(27)
#define CYGHWR_XILINX_TEMAC_EMMC_HOST                BIT_(26)
#define CYGHWR_XILINX_TEMAC_EMMC_TX16                BIT_(25)
#define CYGHWR_XILINX_TEMAC_EMMC_RX16                BIT_(24)

// RGMII/SGMII Configuration (PHYC)

#define CYGHWR_XILINX_TEMAC_PHYC_RGMII_LNK_SPEED(_x) VALUE_(2, _x)
#define CYGHWR_XILINX_TEMAC_PHYC_RGMII_HD            BIT_(1)
#define CYGHWR_XILINX_TEMAC_PHYC_RGMII_LINK          BIT_(0)

// Management Configuration (MC)

#define CYGHWR_XILINX_TEMAC_MC_CLK_DIV(_x)           VALUE_(0, _x)
#define CYGHWR_XILINX_TEMAC_MC_MDIO_EN               BIT_(6)

// Address Filter Mode (AFM)

#define CYGHWR_XILINX_TEMAC_AFM_PM                   BIT_(31)

// Interrupt Status (TIS)

#define CYGHWR_XILINX_TEMAC_TIS_FABR_RR              BIT_(0)
#define CYGHWR_XILINX_TEMAC_TIS_MIIM_RR              BIT_(1)
#define CYGHWR_XILINX_TEMAC_TIS_MIIM_WR              BIT_(2)
#define CYGHWR_XILINX_TEMAC_TIS_AF_RR                BIT_(3)
#define CYGHWR_XILINX_TEMAC_TIS_AF_WR                BIT_(4)
#define CYGHWR_XILINX_TEMAC_TIS_CFG_RR               BIT_(5)
#define CYGHWR_XILINX_TEMAC_TIS_CFG_WR               BIT_(6)

// Interrupt Enable (TIE)

#define CYGHWR_XILINX_TEMAC_TIE_FABR_RR              BIT_(0)
#define CYGHWR_XILINX_TEMAC_TIE_MIIM_RR              BIT_(1)
#define CYGHWR_XILINX_TEMAC_TIE_MIIM_WR              BIT_(2)
#define CYGHWR_XILINX_TEMAC_TIE_AF_RR                BIT_(3)
#define CYGHWR_XILINX_TEMAC_TIE_AF_WR                BIT_(4)
#define CYGHWR_XILINX_TEMAC_TIE_CFG_RR               BIT_(5)
#define CYGHWR_XILINX_TEMAC_TIE_CFG_WR               BIT

// MII Management, concatenate PHY address and register address
#define CYGHWR_XILINX_TEMAC_PHY_REG( addr, reg  )    \
       ( ( reg & 0x1F ) | ( ( addr << 5 ) & 0x3E0 ) )

// Interface to CDL
#define CYGHWR_XILINX_TEMAC_JUMBO_EN                 BIT_(0)
#define CYGHWR_XILINX_TEMAC_VLAN_EN                  BIT_(1)
#define CYGHWR_XILINX_TEMAC_FCS_STRIP_EN             BIT_(2)
#define CYGHWR_XILINX_TEMAC_FCS_INSERT_EN            BIT_(3)
#define CYGHWR_XILINX_TEMAC_LT_CHECK_EN              BIT_(4)
#define CYGHWR_XILINX_TEMAC_FLOW_CONTROL_EN          BIT_(5)
#define CYGHWR_XILINX_TEMAC_PROMISCUOUS_EN           BIT_(6)
#define CYGHWR_XILINX_TEMAC_BROADCAST_EN             BIT_(7)


//
//-----------------------------------------------------------------------------
// end of if_temac.h
#endif // CYGONCE_DEV_ETH_XILINX_TEMAC_IO_H
