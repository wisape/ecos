#ifndef CYGONCE_DEVS_ETH_FREESCALE_DTSEC_H
#define CYGONCE_DEVS_ETH_FREESCALE_DTSEC_H
//==========================================================================
//
//      dev/dtsec.h
//
//      Freescale Datapath Three Speed Ethernet Controller (dTSEC)
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
// Author(s):    ccoutand
// Contributors: 
// Date:         2013-01-30
// Purpose:      
// Description:  Hardware driver for Freescale dTSEC
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define FEC_BD_Tx_STATS  0x03FF  // Status mask

typedef struct
{
   cyg_uint32 miimcfg;
   cyg_uint32 miimcom;
   cyg_uint32 miimadd;
   cyg_uint32 miimcon;
   cyg_uint32 miimstat;
   cyg_uint32 miimind;
} dtsec_mii_t;

// MIB specific registers
struct rmon_dtsec
{
    cyg_uint32 tr64;
    cyg_uint32 tr127;
    cyg_uint32 tr255;
    cyg_uint32 tr511;
    cyg_uint32 tr1k;
    cyg_uint32 trmax;
    cyg_uint32 trmgv;
    cyg_uint32 rbyt;
    cyg_uint32 rpkt;
    cyg_uint32 rfcs;
    cyg_uint32 rmca;
    cyg_uint32 rbca;
    cyg_uint32 rxcf;
    cyg_uint32 rxpf;
    cyg_uint32 rxuo;
    cyg_uint32 raln;
    cyg_uint32 rflr;
    cyg_uint32 rcde;
    cyg_uint32 rcse;
    cyg_uint32 rund;
    cyg_uint32 rovr;
    cyg_uint32 rfrg;
    cyg_uint32 rjbr;
    cyg_uint32 rdrp;
    cyg_uint32 tbyt;
    cyg_uint32 tpkt;
    cyg_uint32 tmca;
    cyg_uint32 tbca;
    cyg_uint32 txpf;
    cyg_uint32 tdfr;
    cyg_uint32 tedf;
    cyg_uint32 tscl;
    cyg_uint32 tmcl;
    cyg_uint32 tlcl;
    cyg_uint32 txcl;
    cyg_uint32 tncl;
    cyg_uint8 _reserved20[0x04];
    cyg_uint32 tdrp;
    cyg_uint32 tjbr;
    cyg_uint32 tfcs;
    cyg_uint32 txcf;
    cyg_uint32 tovr;
    cyg_uint32 tund;
    cyg_uint32 tfrg;
    cyg_uint32 car1;
    cyg_uint32 car2;
    cyg_uint32 cam1;
    cyg_uint32 cam2;
};

// DTSEC
struct dtsec_reg
{
    cyg_uint32 id;
    cyg_uint32 id2;
    cyg_uint32 ievent;
    cyg_uint32 imask;
    cyg_uint8 _reserved1[0x04];
    cyg_uint32 ecntrl;
    cyg_uint32 ptv;
    cyg_uint32 tbipa;
    cyg_uint32 tmr_ctrl;
    cyg_uint32 tmr_pevent;
    cyg_uint32 tmr_pemask;
    cyg_uint8 _reserved2[0x14];
    cyg_uint32 tctrl;
    cyg_uint8 _reserved3[0x0C];
    cyg_uint32 rctrl;
    cyg_uint8 _reserved4[0x2C];
    cyg_uint32 igaddr[8];
    cyg_uint32 gaddr[8];
    cyg_uint8 _reserved5[0x40];
    cyg_uint32 maccfg1;
    cyg_uint32 maccfg2;
    cyg_uint32 ipgifg;
    cyg_uint32 hafdup;
    cyg_uint32 maxfrm;
    cyg_uint8 _reserved6[0x28];
    cyg_uint32 ifstat;
    cyg_uint32 macstnaddr1;
    cyg_uint32 macstnaddr2;
    cyg_uint32 macxaddr[15][2];
    cyg_uint8 _reserved7[0x40];
    struct rmon_dtsec rmon;
} ;

#define IEEE_8023_MAX_FRAME     1518    // Largest possible ethernet frame
#define IEEE_8023_MIN_FRAME     60    // Smallest possible ethernet frame

// MII Management Configuration Register (MIIMCFG)
#define MIIMCFG_RESET           0x80000000
#define MIIMCFG_CLOCK_DIV_4     0x00000001
#define MIIMCFG_CLOCK_DIV_6     0x00000002
#define MIIMCFG_CLOCK_DIV_8     0x00000003
#define MIIMCFG_CLOCK_DIV_10    0x00000004
#define MIIMCFG_CLOCK_DIV_14    0x00000005
#define MIIMCFG_CLOCK_DIV_20    0x00000006
#define MIIMCFG_CLOCK_DIV_28    0x00000007

// MII Management Indicator Register (MIIMIND)
#define MIIMIND_NOT_VALID       0x00000004
#define MIIMIND_SCAN            0x00000002
#define MIIMIND_BUSY            0x00000001

// MII Management Status Register (MIIMSTAT)
#define MIIMSTAT_PHY_STATUS     0x0000FFFF

// MII Management Command Register (MIIMCOM)
#define MIIMCOM_READ            0x00000001

// Interrupt Mask Register (IMASK)
#define IMASK_BREN              0x80000000
#define IMASK_RXCEN             0x40000000
#define IMASK_MSROEN            0x04000000
#define IMASK_GTSCEN            0x02000000
#define IMASK_BTEN              0x01000000
#define IMASK_TXCEN             0x00800000
#define IMASK_TXEEN             0x00400000
#define IMASK_LCEN              0x00040000
#define IMASK_CRLEN             0x00020000
#define IMASK_XFUNEN            0x00010000
#define IMASK_MAGEN             0x00000800
#define IMASK_MMRDEN            0x00000400
#define IMASK_MMWREN            0x00000200
#define IMASK_GRSCEN            0x00000100
#define IMASK_TDPEEN            0x00000002
#define IMASK_RDPEEN            0x00000001
#define IMASK_DEFAULT (IMASK_BREN  | IMASK_RXCEN  | IMASK_GTSCEN | IMASK_BTEN | \
           IMASK_TXCEN | IMASK_TXEEN | IMASK_LCEN | IMASK_CRLEN | IMASK_XFUNEN | IMASK_MAGEN | \
           IMASK_GRSCEN | IMASK_TDPEEN | IMASK_RDPEEN )

// Interrupt Event Register (IEVENT)
#define IEVENT_BABR             0x80000000 // Babbling receive error.
#define IEVENT_RXC              0x40000000 // Receive control (pause frame) interrupt.
#define IEVENT_MSRO             0x04000000 // MIB counter overflow.
#define IEVENT_GTSC             0x02000000 // Graceful transmit stop complete.
#define IEVENT_BABT             0x01000000 // Babbling transmit error.
#define IEVENT_TXC              0x00800000 // Transmit control (pause frame) interrupt.
#define IEVENT_TXE              0x00400000 // Transmit Error
#define IEVENT_LC               0x00040000 // Late collision
#define IEVENT_CRL              0x00020000 // Collision retry limit.
#define IEVENT_XFUN             0x00010000 // Transmit FIFO underrun.
#define IEVENT_MAG              0x00000800 // Magic Packet detected
#define IEVENT_MMRD             0x00000400 // MII management read completion
#define IEVENT_MMWR             0x00000200 // MII management write completion
#define IEVENT_GRSC             0x00000100 // Graceful receive stop complete
#define IEVENT_TDPE             0x00000002 // Internal data error on transmit
#define IEVENT_RDPE             0x00000001 // Internal data error on receive

// Ethernet Control Register (ECNTRL)
#define ECNTRL_CFG_RO           0x80000000 // Configuration read-only
#define ECNTRL_CLRCNT           0x00004000 // Clear all statistics counters
#define ECNTRL_AUTOZ            0x00002000 // Automatically zero MIB counter values
#define ECNTRL_STEN             0x00001000 // MIB counter statistics enabled
#define ECNTRL_GMIIM            0x00000040 // GMII interface mode
#define ECNTRL_TBIM             0x00000020 // Ten-bit interface mode
#define ECNTRL_RPM              0x00000010 // Reduced-pin mode for Gigabit interfaces
#define ECNTRL_R100M            0x00000008 // RGMII/RMII 100 mode
#define ECNTRL_RMM              0x00000004 // Reduced-pin mode for 10/100 interfaces
#define ECNTRL_SGMIIM           0x00000002 // Serial GMII mode

// MAC Configuration Register #1 (MACCFG1)
#define MACCFG1_SOFT_RESET      0x80000000
#define MACCFG1_RESET_RX_MC     0x00080000
#define MACCFG1_RESET_TX_MC     0x00040000
#define MACCFG1_RESET_RX_FUN    0x00020000
#define MACCFG1_RESET_TX_FUN    0x00010000
#define MACCFG1_LOOPBACK        0x00000100
#define MACCFG1_RXFLOW          0x00000020
#define MACCFG1_TXFLOW          0x00000010
#define MACCFG1_SYNC_RXEN       0x00000008
#define MACCFG1_RXEN            0x00000004
#define MACCFG1_SYNC_TXEN       0x00000002
#define MACCFG1_TXEN            0x00000001

//  MAC Configuration Register #2 (MACCFG2)
#define MACCFG2_IF_MODE_NIBBLE  0x00000100
#define MACCFG2_IF_MODE_BYTE    0x00000200
#define MACCFG2_PreAM_RxEN      0x00000080
#define MACCFG2_PreAM_TxEN      0x00000040
#define MACCFG2_HUGE_FRAME      0x00000020
#define MACCFG2_LENGTH_CHECK    0x00000010
#define MACCFG2_MPEN            0x00000008
#define MACCFG2_PAD_CRC         0x00000004
#define MACCFG2_CRC_EN          0x00000002
#define MACCFG2_FULL_DUPLEX     0x00000001


// Attribute Register (ATTR)
#define ATTR_RDSEN 0x00000080   /*Rx data snoop enable.
                                 * 0 Disables snooping of all receive frames data to memory.
                                 * 1 Enables snooping of all receive frames data to memory.
                                 */
#define ATTR_RBDSEN 0x00000040  /*RxBD snoop enable.
                                 * 0 Disables snooping of all receive BD memory accesses.
                                 * 1 Enables snooping of all receive BD memory accesses.
                                 */

//  Receive Control Register (RCTRL)
#define RCTRL_CFA               0x00008000
#define RCTRL_GHTX              0x00000400
#define RCTRL_RTSE              0x00000040
#define RCTRL_GSR               0x00000020
#define RCTRL_BC_REJ            0x00000010
#define RCTRL_MPROM             0x00000008
#define RCTRL_RSF               0x00000004
#define RCTRL_EMEN              0x00000002
#define RCTRL_UPROM             0x00000001

//  Transmit Control Register (TCTRL)
#define TCTRL_THDF              0x00000800
#define TCTRL_TTSE              0x00000040
#define TCTRL_GTS               0x00000020
#define TCTRL_RFC_PAUSE         0x00000010

#define IEVENT_CLEAR_ALL        0xFFFFFFFF
#define IMASK_CLEAR_ALL         0x00000000

#define RESET_FULL_DUPLEX       0x00000001
#define RESET_100MB             0x00000002
#define RESET_1000MB            0x00000004

#ifdef CYGPKG_DEVS_ETH_PHY
static void dtsec_phy_init(void);
static void dtsec_reset_phy(volatile dtsec_mii_t *);
static void dtsec_phy_put_reg(dtsec_mii_t *, int , int , unsigned short );
static bool dtsec_phy_get_reg(dtsec_mii_t *, int , int , unsigned short *);
#endif

#define DTSEC_MAX_INTERFACE 4

#define DTSEC_ERROR_BABR(qi) \
   diag_printf("DTSEC%d - Babbling receive error.\n", qi->dtsec_if)
#define DTSEC_ERROR_BSY(qi) \
   diag_printf("DTSEC%d - Busy condition interrupt.\n", qi->dtsec_if)
#define DTSEC_ERROR_EBERR(qi) \
   diag_printf("DTSEC%d - Internal bus error.\n", qi->dtsec_if)
#define DTSEC_ERROR_MSRO(qi) \
      diag_printf("DTSEC%d - MIB counter overflow.\n", qi->dtsec_if)
#define DTSEC_ERROR_BABT(qi) \
      diag_printf("DTSEC%d - Babbling transmit error.\n", qi->dtsec_if)
#define DTSEC_ERROR_TXE(qi) \
      diag_printf("DTSEC%d - Transmit error.\n", qi->dtsec_if)
#define DTSEC_ERROR_LC(qi) \
      diag_printf("DTSEC%d - Late collision.\n", qi->dtsec_if)
#define DTSEC_ERROR_CRL(qi) \
      diag_printf("DTSEC%d - Collision retry limit.\n", qi->dtsec_if)
#define DTSEC_ERROR_XFUN(qi) \
      diag_printf("DTSEC%d - Transmit FIFO underrun.\n", qi->dtsec_if)
#define DTSEC_ERROR_DPE(qi) \
      diag_printf("DTSEC%d - Internal data parity error.\n", qi->dtsec_if)

#define CYGARC_REG_IMM_DTSECx_MIIMCFG     0x0120
#define CYGARC_REG_IMM_DTSECx_MIIMCOM     0x0124
#define CYGARC_REG_IMM_DTSECx_MIIMADD     0x0128
#define CYGARC_REG_IMM_DTSECx_MIIMCON     0x012C
#define CYGARC_REG_IMM_DTSECx_MIIMSTAT    0x0130
#define CYGARC_REG_IMM_DTSECx_MIIMIND     0x0134

#define CYGNUM_REG_IMM_DPAA_FMAN1_DTSEC_MII1_BASE(_x)     \
   (volatile struct dtsec_mii_t *) ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 + _x )

#endif
