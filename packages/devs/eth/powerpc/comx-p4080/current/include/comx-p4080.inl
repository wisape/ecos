#ifndef CYGONCE_DEVS_COMX_P4080_ETH_INL
#define CYGONCE_DEVS_COMX_P4080_ETH_INL
//==========================================================================
//
//      comx-p4080.inl
//
//      Ethernet device driver for Emerson COMX-P4080 board
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 2013 Free Software Foundation, Inc. 
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
// Date:         2013-01-29
// Purpose:      
// Description:  Hardware driver for Emerson COMX-P4080 board
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/hal/hal_intr.h> 
#include <cyg/hal/qoriq.h>

#define RxBUFSIZE CYGNUM_DEVS_ETH_FREESCALE_FMAN_ETH0_RxNUM*CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_RX
#define TxBUFSIZE CYGNUM_DEVS_ETH_FREESCALE_FMAN_ETH0_TxNUM*CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_TX

#ifdef CYGPKG_DEVS_ETH_PHY

# define BCM54xx_MII_CTRL_REG             0x00
# define BCM54xx_MII_CTRL_AN_RESTART      0x0200
# define BCM54xx_MII_CTRL_AN_ENABLE       0x1000
# define BCM54xx_MII_CTRL_RESET           0x8000

void
_eth_phy_plf_init(eth_phy_access_t *f){
  cyg_uint16 phy_reg;

  // Reset PHY
  _eth_phy_read(f, BCM54xx_MII_CTRL_REG, f->phy_addr, &phy_reg);
  phy_reg |= BCM54xx_MII_CTRL_RESET;
  _eth_phy_write(f, BCM54xx_MII_CTRL_REG, f->phy_addr, phy_reg);

  _eth_phy_write(f, 4, f->phy_addr, 0x01E1);
  _eth_phy_write(f, 5, f->phy_addr, 0x45E1);

  // Allow auto-negociation
  _eth_phy_read(f, BCM54xx_MII_CTRL_REG, f->phy_addr, &phy_reg);
  phy_reg |= (BCM54xx_MII_CTRL_AN_RESTART | BCM54xx_MII_CTRL_AN_ENABLE);
  _eth_phy_write(f, BCM54xx_MII_CTRL_REG, f->phy_addr, phy_reg);
}

#endif // CYGPKG_DEVS_ETH_PHY

#ifdef CYGPKG_DEVS_ETH_COMX_P4080_ETH0

#ifdef CYGPKG_DEVS_ETH_PHY
ETH_PHY_REG_LEVEL_ACCESS_FUNS(eth0_phy, 
                              dtsec_phy_init,
                              NULL,
                              dtsec1_phy_put_reg,
                              dtsec1_phy_get_reg);
#endif

// Align buffers on a cache boundary
static unsigned char eth0_rxbufs[RxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static unsigned char eth0_txbufs[TxBUFSIZE] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

static volatile struct fman_bd eth0_rxring[CYGNUM_DEVS_ETH_FREESCALE_FMAN_ETH0_RxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));
static volatile struct fman_bd eth0_txring[CYGNUM_DEVS_ETH_FREESCALE_FMAN_ETH0_TxNUM] __attribute__((aligned(HAL_DCACHE_LINE_SIZE)));

//
// Fill Frame Manager Data structure
// Most of this data could be added to the CDL in the future

static struct fman_eth_cfg fman_eth0_cfg = {
#ifdef CYGDAT_DEVS_ETH_COMX_P4080_ETH0_FMAN_MODE_IM
    .im      = 1,
#else
    .im      = 0,
#endif
    .num     = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_FMAN_NUM,
    .tx_port = {
      .port      = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_TX_PORTID,
      .lport     = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_TX_LOGICAL_PORTID,
      .fifo_mfl  = FMAN_BMI_DEFAULT_TX_FIFO_MIN_FILL_LEV,
      .fifo_dpde = FMAN_BMI_DEFAULT_TX_FIFO_DQ_PDEPTH_1G,
      .fifo_lcl  = FMAN_BMI_DEFAULT_TX_FIFO_LOW_COMFORT_LEV,
    },
    .rx_port = {
      .port      = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_RX_PORTID,
      .lport     = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_RX_LOGICAL_PORTID,
      .fifo_pel  = FMAN_BMI_DEFAULT_RX_FIFO_PRI_ELEV,
      .fifo_thr  = FMAN_BMI_DEFAULT_RX_FIFO_THR,
    },
};

//
// Fill MAC layer Data structure

static struct mac_eth_cfg mac_eth0_cfg = {
#ifdef CYGSEM_DEVS_ETH_COMX_P4080_ETH0_SET_ESA
    .address    = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_ESA,
#endif
#ifdef CYGDAT_DEVS_ETH_COMX_P4080_ETH0_MAC_DTSEC
    .type       = FMAN_MAC_DTSEC,
#else
    .type       = FMAN_MAC_10GEC,
#endif
    .num        = CYGDAT_DEVS_ETH_COMX_P4080_ETH0_MAC_NUM,
    .mac2phy    = MODE_RGMII,
    .sc         = &dtsec_sc
};

//
// Fill Ethernet Data structure

static struct eth_fman_t fman_eth0 = { 
    .mac            = &mac_eth0_cfg,
    .fman           = &fman_eth0_cfg,
    .init_rxbufs    = &eth0_rxbufs[0],
    .init_txbufs    = &eth0_txbufs[0],
    .init_rxring    = &eth0_rxring[0],
    .init_txring    = &eth0_txring[0],
    .rxnum          = CYGNUM_DEVS_ETH_FREESCALE_FMAN_ETH0_RxNUM,
    .txnum          = CYGNUM_DEVS_ETH_FREESCALE_FMAN_ETH0_TxNUM,
#ifdef CYGPKG_DEVS_ETH_PHY
    .reset_mii      = true,
    .phy            = &eth0_phy,
    .create_phy_irq = NULL,
    .plf_phy_init   = _eth_phy_plf_init,
#endif
    .mii            = &dtsec1_mii,
};

ETH_DRV_SC(fman_eth0_sc,
        &fman_eth0,
        CYGDAT_DEVS_ETH_COMX_P4080_ETH0_NAME,
        fman_eth_start,
        fman_eth_stop,
        fman_eth_control,
        fman_eth_can_send,
        fman_eth_send,
        fman_eth_recv,
        fman_eth_deliver,
        fman_eth_int,
        fman_eth_int_vector)
;
NETDEVTAB_ENTRY(fman_netdev0,
        "fman_dtsec_" CYGDAT_DEVS_ETH_COMX_P4080_ETH0_NAME,
        fman_eth_init,
        &fman_eth0_sc)
;

#endif // CYGPKG_DEVS_ETH_COMX_P4080_ETH0

#ifdef CYGPKG_DEVS_ETH_COMX_P4080_ETH_DEBUG_CODE

externC void
comx_p4080_eth_stats(cyg_uint8 eth, char flag)
{
#ifdef CYGPKG_DEVS_ETH_COMX_P4080_ETH0
   if(eth == 0)
   {
     fman_disp_tx_bd_stats( &fman_eth0_sc , flag );
     fman_disp_rx_bd_stats( &fman_eth0_sc , flag );
     fman_disp_imgp( &fman_eth0_sc );
     dtsec_disp_stats( &fman_eth0_sc , flag );
   }
#endif // CYGPKG_DEVS_ETH_COMX_P4080_ETH0
}

#endif // CYGPKG_DEVS_ETH_COMX_P4080_ETH_DEBUG_CODE

#endif