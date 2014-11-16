//==========================================================================
//
//      if_temac.inl
//
//      Support for Xilinx TEMAC Ethernet controller
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
//
// Date:         2012-02-13
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_DEV_ETH_XILINX_TEMAC_INL
#define CYGONCE_DEV_ETH_XILINX_TEMAC_INL

#include <cyg/hal/hal_intr.h>
#include <cyg/hal/var_io.h>

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1

// Make sure packet size is multiple of words
#define CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_ALIGN_BUFSIZE_RX  \
            ((CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_BUFSIZE_RX + 0x00000003) & (0xfffffffc))

#define CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_ALIGN_BUFSIZE_TX  \
            ((CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_BUFSIZE_TX + 0x00000003) & (0xfffffffc))

// Declare TX / RX buffers
static cyg_uint32 temac_eth1_rxbufs[ CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_ALIGN_BUFSIZE_RX / 4 ];
static cyg_uint32 temac_eth1_txbufs[ CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_ALIGN_BUFSIZE_TX / 4 ];

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth1_phy,
                              temac1_mii_init,
                              NULL, 
                              temac1_mii_write, 
                              temac1_mii_read);
#endif

static struct temac_eth_info temac_eth1_info = {
    .init_rxbufs = (cyg_uint8 *) &temac_eth1_rxbufs[0],
    .init_txbufs = (cyg_uint8 *) &temac_eth1_txbufs[0],
#ifdef CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH1_ESA
    .mac_address = CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH1_ESA,
#endif
    .base        = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+CYGHWR_XILINX_TEMAC1_REG_OFFSET,
    .tbl_base    = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+CYGHWR_XILINX_TEMAC1_TBL_OFFSET,
#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO
    .llf_base    = CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO_BLOCK_BASE,
# ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .llf_vector  = CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO_IRQNUM,
# endif
#else
    .llf_base    = 0,
#endif
    .if_num      = 0,
    .if_config   = 
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_JUMBO
          CYGHWR_XILINX_TEMAC_JUMBO_EN        |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_VLAN
          CYGHWR_XILINX_TEMAC_VLAN_EN         |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_FCS_STRIP
          CYGHWR_XILINX_TEMAC_FCS_STRIP_EN    |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_FCS_INSERT
          CYGHWR_XILINX_TEMAC_FCS_INSERT_EN   |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_LT_CHECK
          CYGHWR_XILINX_TEMAC_LT_CHECK_EN     |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_FLOW_CONTROL
          CYGHWR_XILINX_TEMAC_FLOW_CONTROL_EN |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_PROMISCUOUS
          CYGHWR_XILINX_TEMAC_PROMISCUOUS_EN  |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH1_BROADCAST
          CYGHWR_XILINX_TEMAC_BROADCAST_EN    |
#endif
          0,
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .vector = CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_IRQNUM,
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    .phy = &eth1_phy,
#endif
};

ETH_DRV_SC(temac_eth1_sc,
           &temac_eth1_info,
           CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH1_NAME,
           temac_eth_start,
           temac_eth_stop,
           temac_eth_control,
#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO
           temac_eth_ll_fifo_can_send,
           temac_eth_ll_fifo_send,
           temac_eth_ll_fifo_recv,
#else
           temac_eth_dma_can_send,
           temac_eth_dma_send,
           temac_eth_dma_recv,
#endif
           temac_eth_deliver, 
           temac_eth_poll, 
           temac_eth_int_vector);

NETDEVTAB_ENTRY(temac_netdev1,
                "temac_" CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH1_NAME,
                temac_eth_init,
                &temac_eth1_sc);

#endif// CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0

// Make sure packet size is multiple of words
#define CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_ALIGN_BUFSIZE_RX  \
            ((CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_BUFSIZE_RX + 0x00000003) & (0xfffffffc))

#define CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_ALIGN_BUFSIZE_TX  \
            ((CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_BUFSIZE_TX + 0x00000003) & (0xfffffffc))

// Declare TX / RX buffers
static cyg_uint32 temac_eth0_rxbufs[ CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_ALIGN_BUFSIZE_RX / 4 ];
static cyg_uint32 temac_eth0_txbufs[ CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_ALIGN_BUFSIZE_TX / 4 ];

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth0_phy,
                              temac0_mii_init,
                              NULL, 
                              temac0_mii_write, 
                              temac0_mii_read);
#endif

static struct temac_eth_info temac_eth0_info = {
    .init_rxbufs = (cyg_uint8 *) &temac_eth0_rxbufs[0],
    .init_txbufs = (cyg_uint8 *) &temac_eth0_txbufs[0],
#ifdef CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH0_ESA
    .mac_address = CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH0_ESA,
#endif
    .base        = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+CYGHWR_XILINX_TEMAC0_REG_OFFSET,
    .tbl_base    = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+CYGHWR_XILINX_TEMAC0_TBL_OFFSET,
#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO
    .llf_base    = CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO_BLOCK_BASE,
# ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .llf_vector  = CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO_IRQNUM,
# endif
#else
    .llf_base    = 0,
#endif
    .if_num      = 0,
    .if_config   = 
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_JUMBO
          CYGHWR_XILINX_TEMAC_JUMBO_EN        |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_VLAN
          CYGHWR_XILINX_TEMAC_VLAN_EN         |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_FCS_STRIP
          CYGHWR_XILINX_TEMAC_FCS_STRIP_EN    |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_FCS_INSERT
          CYGHWR_XILINX_TEMAC_FCS_INSERT_EN   |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_LT_CHECK
          CYGHWR_XILINX_TEMAC_LT_CHECK_EN     |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_FLOW_CONTROL
          CYGHWR_XILINX_TEMAC_FLOW_CONTROL_EN |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_PROMISCUOUS
          CYGHWR_XILINX_TEMAC_PROMISCUOUS_EN  |
#endif
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_ETH0_BROADCAST
          CYGHWR_XILINX_TEMAC_BROADCAST_EN    |
#endif
          0,
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .vector = CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_IRQNUM,
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    .phy = &eth0_phy,
#endif
};


ETH_DRV_SC(temac_eth0_sc,
           &temac_eth0_info,
           CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH0_NAME,
           temac_eth_start,
           temac_eth_stop,
           temac_eth_control,
#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO
           temac_eth_ll_fifo_can_send,
           temac_eth_ll_fifo_send,
           temac_eth_ll_fifo_recv,
#else
           temac_eth_dma_can_send,
           temac_eth_dma_send,
           temac_eth_dma_recv,
#endif
           temac_eth_deliver, 
           temac_eth_poll, 
           temac_eth_int_vector);

NETDEVTAB_ENTRY(temac_netdev0,
                "temac_" CYGDAT_DEVS_ETH_XILINX_TEMAC_ETH0_NAME,
                temac_eth_init,
                &temac_eth0_sc);

#endif// CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0

#endif // CYGONCE_DEV_ETH_XILINX_TEMAC_INL
