//==========================================================================
//
//      mpc8572ds_eth.inl
//
//      Ethernet device driver for Freescale MPC8572DS boards
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
// Date:         2009-11-01
// Purpose:      
// Description:  hardware driver for Freescale MPC8572DS boards
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================


#ifndef CYGONCE_DEVS_MPC8572DS_ETH_INL
#define CYGONCE_DEVS_MPC8572DS_ETH_INL

#include <cyg/hal/hal_intr.h> 
#include <cyg/hal/quicc3.h>

externC
void mpc8572ds_create_phy_irq(struct eth_drv_sc *);

#define RxBUFSIZE CYGNUM_DEVS_ETH_POWERPC_TSEC_RxNUM*CYGNUM_DEVS_ETH_POWERPC_TSEC_BUFSIZE_RX
#define TxBUFSIZE CYGNUM_DEVS_ETH_POWERPC_TSEC_TxNUM*CYGNUM_DEVS_ETH_POWERPC_TSEC_BUFSIZE_TX



#ifdef CYGPKG_DEVS_ETH_MPC8572DS_ETH0

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth0_phy, 
                              tsec_phy_init,
                              NULL,
                              tsec1_phy_put_reg,
                              tsec1_phy_get_reg);
#endif

// Align buffers on a cache boundary
static unsigned char tsec_eth0_rxbufs[RxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static unsigned char tsec_eth0_txbufs[TxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_bd tsec_eth0_rxring[CYGNUM_DEVS_ETH_POWERPC_TSEC_RxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static struct tsec_bd tsec_eth0_txring[CYGNUM_DEVS_ETH_POWERPC_TSEC_TxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_eth_info tsec_eth0_info = { 
#ifdef CYGSEM_DEVS_ETH_MPC8572DS_ETH0_SET_ESA
    .mac_address    = CYGDAT_DEVS_ETH_MPC8572DS_ETH0_ESA,
#endif
    .tsec_if        = CYGDAT_DEVS_ETH_MPC8572DS_ETH0_TSEC_IF,
    .init_rxbufs    = &tsec_eth0_rxbufs[0],
    .init_txbufs    = &tsec_eth0_txbufs[0],
    .init_rxring    = &tsec_eth0_rxring[0],
    .init_txring    = &tsec_eth0_txring[0],
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .tx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC1_Tx,
    .rx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC1_Rx,
    .err_irq_vector = CYGNUM_HAL_INTERRUPT_TSEC1_Err,
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    .phy            = &eth0_phy,
#endif
    .mii            = &tsec1_mii,
    .reset_mii      = true
};

ETH_DRV_SC(tsec_eth0_sc,
        &tsec_eth0_info,
        CYGDAT_DEVS_ETH_MPC8572DS_ETH0_NAME,
        tsec_eth_start,
        tsec_eth_stop,
        tsec_eth_control,
        tsec_eth_can_send,
        tsec_eth_send,
        tsec_eth_recv,
        tsec_eth_deliver,
        tsec_eth_int,
        tsec_eth_int_vector)
;
NETDEVTAB_ENTRY(tsec_netdev0,
        "tsec_" CYGDAT_DEVS_ETH_MPC8572DS_ETH0_NAME,
        tsec_eth_init,
        &tsec_eth0_sc)
;

#endif // CYGPKG_DEVS_ETH_MPC8572DS_ETH0




#ifdef CYGPKG_DEVS_ETH_MPC8572DS_ETH1

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth1_phy, 
                              tsec_phy_init,
                              NULL,
                              tsec1_phy_put_reg,
                              tsec1_phy_get_reg);
#endif

// Align buffers on a cache boundary
static unsigned char tsec_eth1_rxbufs[RxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static unsigned char tsec_eth1_txbufs[TxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_bd tsec_eth1_rxring[CYGNUM_DEVS_ETH_POWERPC_TSEC_RxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static struct tsec_bd tsec_eth1_txring[CYGNUM_DEVS_ETH_POWERPC_TSEC_TxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_eth_info tsec_eth1_info = { 
#ifdef CYGSEM_DEVS_ETH_MPC8572DS_ETH1_SET_ESA
    .mac_address    = CYGDAT_DEVS_ETH_MPC8572DS_ETH1_ESA,
#endif
    .tsec_if        = CYGDAT_DEVS_ETH_MPC8572DS_ETH1_TSEC_IF,
    .init_rxbufs    = &tsec_eth1_rxbufs[0],
    .init_txbufs    = &tsec_eth1_txbufs[0],
    .init_rxring    = &tsec_eth1_rxring[0],
    .init_txring    = &tsec_eth1_txring[0],
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .tx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC2_Tx,
    .rx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC2_Rx,
    .err_irq_vector = CYGNUM_HAL_INTERRUPT_TSEC2_Err,
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    .phy            = &eth1_phy,
#endif
    .mii            = &tsec1_mii,
    .reset_mii      = false
};

ETH_DRV_SC(tsec_eth1_sc,
        &tsec_eth1_info, 
        CYGDAT_DEVS_ETH_MPC8572DS_ETH1_NAME,
        tsec_eth_start,
        tsec_eth_stop,
        tsec_eth_control,
        tsec_eth_can_send,
        tsec_eth_send,
        tsec_eth_recv,
        tsec_eth_deliver,
        tsec_eth_int,
        tsec_eth_int_vector)
;
NETDEVTAB_ENTRY(tsec_netdev1,
        "tsec_" CYGDAT_DEVS_ETH_MPC8572DS_ETH1_NAME,
        tsec_eth_init,
        &tsec_eth1_sc)
;

#endif // CYGPKG_DEVS_ETH_MPC8572DS_ETH1




#ifdef CYGPKG_DEVS_ETH_MPC8572DS_ETH2

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth2_phy, 
                              tsec_phy_init,
                              NULL,
                              tsec1_phy_put_reg,
                              tsec1_phy_get_reg);
#endif

// Align buffers on a cache boundary
static unsigned char tsec_eth2_rxbufs[RxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static unsigned char tsec_eth2_txbufs[TxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_bd tsec_eth2_rxring[CYGNUM_DEVS_ETH_POWERPC_TSEC_RxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static struct tsec_bd tsec_eth2_txring[CYGNUM_DEVS_ETH_POWERPC_TSEC_TxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_eth_info tsec_eth2_info = { 
#ifdef CYGSEM_DEVS_ETH_MPC8572DS_ETH2_SET_ESA
    .mac_address    = CYGDAT_DEVS_ETH_MPC8572DS_ETH2_ESA,
#endif
    .tsec_if        = CYGDAT_DEVS_ETH_MPC8572DS_ETH2_TSEC_IF,
    .init_rxbufs    = &tsec_eth2_rxbufs[0],
    .init_txbufs    = &tsec_eth2_txbufs[0],
    .init_rxring    = &tsec_eth2_rxring[0],
    .init_txring    = &tsec_eth2_txring[0],
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .tx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC3_Tx,
    .rx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC3_Rx,
    .err_irq_vector = CYGNUM_HAL_INTERRUPT_TSEC3_Err,
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    .phy            = &eth2_phy,
#endif
    .mii            = &tsec1_mii,
    .reset_mii      = false
};

ETH_DRV_SC(tsec_eth2_sc,
        &tsec_eth2_info, 
        CYGDAT_DEVS_ETH_MPC8572DS_ETH2_NAME,
        tsec_eth_start,
        tsec_eth_stop,
        tsec_eth_control,
        tsec_eth_can_send,
        tsec_eth_send,
        tsec_eth_recv,
        tsec_eth_deliver,
        tsec_eth_int,
        tsec_eth_int_vector)
;
NETDEVTAB_ENTRY(tsec_netdev2,
		"tsec_" CYGDAT_DEVS_ETH_MPC8572DS_ETH2_NAME,
        tsec_eth_init,
        &tsec_eth2_sc)
;

#endif // CYGPKG_DEVS_ETH_MPC8572DS_ETH2





#ifdef CYGPKG_DEVS_ETH_MPC8572DS_ETH3

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth3_phy, 
                              tsec_phy_init,
                              NULL,
                              tsec1_phy_put_reg,
                              tsec1_phy_get_reg);
#endif

// Align buffers on a cache boundary
static unsigned char tsec_eth3_rxbufs[RxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static unsigned char tsec_eth3_txbufs[TxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_bd tsec_eth3_rxring[CYGNUM_DEVS_ETH_POWERPC_TSEC_RxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static struct tsec_bd tsec_eth3_txring[CYGNUM_DEVS_ETH_POWERPC_TSEC_TxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static struct tsec_eth_info tsec_eth3_info = { 
#ifdef CYGSEM_DEVS_ETH_MPC8572DS_ETH2_SET_ESA
    .mac_address    = CYGDAT_DEVS_ETH_MPC8572DS_ETH3_ESA,
#endif
    .tsec_if        = CYGDAT_DEVS_ETH_MPC8572DS_ETH3_TSEC_IF,
    .init_rxbufs    = &tsec_eth3_rxbufs[0],
    .init_txbufs    = &tsec_eth3_txbufs[0],
    .init_rxring    = &tsec_eth3_rxring[0],
    .init_txring    = &tsec_eth3_txring[0],
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    .tx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC4_Tx,
    .rx_irq_vector  = CYGNUM_HAL_INTERRUPT_TSEC4_Rx,
    .err_irq_vector = CYGNUM_HAL_INTERRUPT_TSEC4_Err,
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    .phy            = &eth3_phy,
#endif
    .mii            = &tsec1_mii,
    .reset_mii      = false
};

ETH_DRV_SC(tsec_eth3_sc,
        &tsec_eth3_info, 
        CYGDAT_DEVS_ETH_MPC8572DS_ETH3_NAME,
        tsec_eth_start,
        tsec_eth_stop,
        tsec_eth_control,
        tsec_eth_can_send,
        tsec_eth_send,
        tsec_eth_recv,
        tsec_eth_deliver,
        tsec_eth_int,
        tsec_eth_int_vector)
;
NETDEVTAB_ENTRY(tsec_netdev3,
        "tsec_" CYGDAT_DEVS_ETH_MPC8572DS_ETH3_NAME,
        tsec_eth_init,
        &tsec_eth3_sc)
;

#endif // CYGPKG_DEVS_ETH_MPC8572DS_ETH3

#endif