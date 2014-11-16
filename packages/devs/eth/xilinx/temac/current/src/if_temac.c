//==========================================================================
//
//      if_temac.c
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
// Purpose:      Ethernet driver for Xilinx TEMAC controller
// Description:
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#include <pkgconf/system.h>
#include <pkgconf/devs_eth_xilinx_temac.h>

#ifdef CYGDAT_DEVS_XILINX_TEMAC_ETH_CDL
# include CYGDAT_DEVS_XILINX_TEMAC_ETH_CDL
#endif

#include <pkgconf/io_eth_drivers.h>

#ifdef CYGPKG_NET
# include <pkgconf/net.h>
#endif

#include <cyg/infra/cyg_type.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/var_io.h>

#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>

#include <cyg/io/if_temac.h>

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
# include <cyg/hal/var_ll_fifo.h>
#endif

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_MPMC_SDMA) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_MPMC_SDMA)
# include <cyg/hal/var_mpmc.h>
#endif

#ifdef CYGPKG_DEVS_ETH_PHY
# include <cyg/io/eth_phy.h>
#endif

#include <errno.h>
#include <string.h>

static unsigned char enaddr[6];

// Internal data structure
struct temac_eth_info {
#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_MPMC_SDMA) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_MPMC_SDMA)
    volatile struct    mdmc_sdma_bd *txbd,
                      *rxbd;                      // Next Tx,Rx descriptor to use
    volatile struct    mdmc_sdma_bd *tbase,
                      *rbase;                     // First Tx,Rx descriptor
    volatile struct    mdmc_sdma_bd *tnext,
                      *rnext;                     // Next descriptor to check for interrupt
    volatile struct    mdmc_sdma_bd *init_rxring; // Initial base pointer of RX ring
    volatile struct    mdmc_sdma_bd *init_txring; // Initial base pointer of TX ring
#endif
    cyg_uint32         txactive;                  // Count of active Tx buffers
    cyg_uint32         txi, txo;                  // Index of input / output buffer
    unsigned long      txkey[2];
    cyg_uint8          mac_address[6];            // mac (hardware) address
    cyg_uint8          *init_rxbufs;              // Initial base pointer of RX buffers
    cyg_uint8          *init_txbufs;              // Initial base pointer of TX buffers
    cyg_uint32         valid_mac;
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_STATS
    volatile struct    temac_stats *stats;
#endif
    cyg_uint32         base;                      // Ethernet controller base address
    cyg_uint32         tbl_base;                  // Ethernet controller table and statistics base address
    cyg_uint32         llf_base;                  // Local Link FIFO base address, set null if unused
    cyg_uint32         mpmc_base;                 // Multi-port Memory Controller base address, set null if unused
    cyg_uint32         if_config;
    cyg_uint32         if_num;
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    cyg_vector_t       vector;
    cyg_interrupt      interrupt;
    cyg_handle_t       interrupt_handle;
    cyg_vector_t       llf_vector;
    cyg_interrupt      llf_interrupt;
    cyg_handle_t       llf_interrupt_handle;
    cyg_vector_t       llf_irq_masked;
#endif
#ifdef CYGPKG_DEVS_ETH_PHY
    eth_phy_access_t   *phy;
#endif
};

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0
static void temac0_mii_init(void);
static bool temac0_mii_read(cyg_int32, cyg_int32, cyg_uint16 *);
static void temac0_mii_write(cyg_int32, cyg_int32, cyg_uint16);
#endif

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1
static void temac1_mii_init(void);
static bool temac1_mii_read(cyg_int32, cyg_int32, cyg_uint16 *);
static void temac1_mii_write(cyg_int32, cyg_int32, cyg_uint16);
#endif

static bool temac_mii_read(cyg_uint32, cyg_int32, cyg_int32, cyg_uint16 *);
static void temac_mii_write(cyg_uint32, cyg_int32, cyg_int32, cyg_uint16);

static void temac_eth_int(struct eth_drv_sc *data);

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
static void temac_int(struct eth_drv_sc *sc);
#endif

#include <cyg/io/if_temac.inl>
#include <cyg/infra/diag.h>

#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_CHATTER
# define temac_eth_printf(args...)   diag_printf(args)
#else
# define temac_eth_printf(args...)    /* NOOP */
#endif

#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_LOG_ERROR
# define temac_err_printf(args...)   diag_printf(args)
#else
# define temac_err_printf(args...)    /* NOOP */
#endif

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
# include <cyg/io/if_temac_ll_fifo.inl>
#endif

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_MPMC_SDMA) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_MPMC_SDMA)
# include <cyg/io/if_temac_dma.inl>
#endif

// For fetching the ESA from RedBoot
#include <cyg/hal/hal_if.h>
#ifndef CONFIG_ESA
# define CONFIG_ESA 6
#endif


#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_HARD_RESET
cyg_uint8 hard_reset = 1;
#endif

// ----------------------------------------------------------------
// Definition
//
//

#define RESET_FULL_DUPLEX      0x00000001
#define RESET_100MB            0x00000002
#define RESET_1000MB           0x00000004

#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_STATS
# define TEMAC_INC_STATS(_x_)                 (qi->stats._x_++)
#endif


#define TEMAC_WRITE_REG( base, indirect_offset, data)                        \
{                                                                            \
   HAL_WRITE_UINT32( base + CYGHWR_XILINX_TEMAC_LSW0, data );                \
   HAL_WRITE_UINT32( base + CYGHWR_XILINX_TEMAC_CTL0,                        \
                        ( indirect_offset | CYGHWR_XILINX_TEMAC_CTL_WEN ));  \
}

#define TEMAC_READ_REG( base, indirect_offset, data)                         \
{                                                                            \
   HAL_WRITE_UINT32( base + CYGHWR_XILINX_TEMAC_CTL0, indirect_offset );     \
   HAL_READ_UINT32( base + CYGHWR_XILINX_TEMAC_LSW0, data );                 \
}


//
// Setup MII
//

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0
static void
temac0_mii_init(void)
{
    cyg_uint32 base = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+
                                             CYGHWR_XILINX_TEMAC0_REG_OFFSET;
    cyg_uint32 reg  =
       CYGHWR_XILINX_TEMAC_MC_CLK_DIV(CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_MII_CLK_DIV) |
                                              CYGHWR_XILINX_TEMAC_MC_MDIO_EN;
    temac_eth_printf("ETH TEMAC0 - PHY Init\n");
    TEMAC_WRITE_REG( base, CYGHWR_XILINX_TEMAC_MC, reg);
}
#endif

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1
static void
temac1_mii_init(void)
{
    cyg_uint32 base = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+
                                             CYGHWR_XILINX_TEMAC1_REG_OFFSET;
    cyg_uint32 reg  =
       CYGHWR_XILINX_TEMAC_MC_CLK_DIV(CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH1_MII_CLK_DIV) |
                                              CYGHWR_XILINX_TEMAC_MC_MDIO_EN;
    temac_eth_printf("ETH TEMAC1 - PHY Init\n");
    TEMAC_WRITE_REG( base, CYGHWR_XILINX_TEMAC_MC, reg);
}
#endif


//
// MDIO read register
//

static bool
temac_mii_read(cyg_uint32 base, cyg_int32 addr, cyg_int32 phy_addr, cyg_uint16 *data)
{
    bool            res  = true;
    cyg_uint32      reg  = CYGHWR_XILINX_TEMAC_PHY_REG( phy_addr, addr);
    cyg_uint32      rdy  = 0;

    // Start read transaction to MII interface
    HAL_WRITE_UINT32( base + CYGHWR_XILINX_TEMAC_LSW0, reg );
    HAL_WRITE_UINT32( base + CYGHWR_XILINX_TEMAC_CTL0, CYGHWR_XILINX_TEMAC_MIIMAI );

    // Wait read transaction complete
    do{
       HAL_READ_UINT32( base + CYGHWR_XILINX_TEMAC_RDY0, rdy );
    } while( ( rdy & CYGHWR_XILINX_TEMAC_RDY_MIIM_RR ) == 0 );

    // Read data
    HAL_READ_UINT32( base + CYGHWR_XILINX_TEMAC_LSW0, reg );

    *data = (cyg_uint16) reg;

    return res;
}

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0
static bool
temac0_mii_read(cyg_int32 addr, cyg_int32 phy_addr, cyg_uint16 *data)
{
    cyg_uint32      base = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+
                                             CYGHWR_XILINX_TEMAC0_REG_OFFSET;
    return temac_mii_read(base, addr, phy_addr, data);
}
#endif

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1
static bool
temac1_mii_read(cyg_int32 addr, cyg_int32 phy_addr, cyg_uint16 *data)
{
    cyg_uint32      base = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+
                                             CYGHWR_XILINX_TEMAC1_REG_OFFSET;
    return temac_mii_read(base, addr, phy_addr, data);
}
#endif


//
// MDIO write register
//

static void
temac_mii_write(cyg_uint32 base, cyg_int32 addr, cyg_int32 phy_addr, cyg_uint16 data)
{
    cyg_uint32      reg  = CYGHWR_XILINX_TEMAC_PHY_REG( phy_addr, addr);
    cyg_uint32      rdy  = 0;

    // Write to MII interface
    TEMAC_WRITE_REG( base, CYGHWR_XILINX_TEMAC_MIIMWD, data);
    TEMAC_WRITE_REG( base, CYGHWR_XILINX_TEMAC_MIIMAI, reg);

    // Wait transaction complete
    do{
       HAL_READ_UINT32( base + CYGHWR_XILINX_TEMAC_RDY0, rdy );
    } while( ( rdy & CYGHWR_XILINX_TEMAC_RDY_MIIM_WR ) == 0 );
}

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0
static void
temac0_mii_write(cyg_int32 addr, cyg_int32 phy_addr, cyg_uint16 data)
{
    cyg_uint32      base = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+
                                             CYGHWR_XILINX_TEMAC0_REG_OFFSET;
    temac_mii_write(base, addr, phy_addr, data);
}
#endif

#ifdef CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1
static void
temac1_mii_write(cyg_int32 addr, cyg_int32 phy_addr, cyg_uint16 data)
{
    cyg_uint32      base = CYGPKG_DEVS_ETH_XILINX_TEMAC_BLOCK_BASE+
                                             CYGHWR_XILINX_TEMAC1_REG_OFFSET;
    temac_mii_write(base, addr, phy_addr, data);
}
#endif


// ----------------------------------------------------------------
// Ethernet driver
//
//


//
// Deliver function (ex-DSR) handles the Ethernet [logical] processing
//
static void
temac_eth_deliver(struct eth_drv_sc *sc)
{
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    struct temac_eth_info *qi = (struct temac_eth_info *)sc->driver_private;
#endif
    temac_eth_int(sc);
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    // Allow interrupts to happen again
    cyg_drv_interrupt_unmask(qi->vector);
#endif
}

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
//
// This ISR is called when the Ethernet interrupt occurs
//
static int
temac_eth_isr(cyg_vector_t vector, cyg_addrword_t data,
               HAL_SavedRegisters * regs)
{
    temac_eth_printf("ETH TEMAC - ISR %x\n", vector);

    cyg_drv_interrupt_mask(vector);
    cyg_drv_interrupt_acknowledge(vector);

    return (CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);        // Run the DSR
}

void
temac_eth_dsr(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
{
    struct eth_drv_sc *sc = (struct eth_drv_sc *) data;

    temac_eth_printf("ETH TEMAC - DSR %x\n", vector);

    temac_int( sc );

    // Allow interrupts to happen again
    cyg_drv_interrupt_unmask(vector);
}
#endif


//
// [re]Initialize the Ethernet controller
//   Done separately since shutting down the device requires a
//   full reconfiguration when re-enabling.
//   when
static bool
temac_eth_setup(struct eth_drv_sc *sc, unsigned char *enaddr, int flags)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    cyg_uint32 tc = 0, rcw1 = 0xFFEE;
    cyg_uint32 uaw0 = 0, uaw1 = 0;
    cyg_uint32 emmc = 0;
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_STATS
    cyg_uint32 raf0 = 0;
#endif

    qi->txactive = 0;
    qi->txi = qi->txo = 0;

    // Read and clear link speed in controller
    TEMAC_READ_REG( qi->base, CYGHWR_XILINX_TEMAC_EMMC, emmc );
    emmc &= ~CYGHWR_XILINX_TEMAC_EMMC_LINK_SPEED(0x3);

    // Setup actual speed
    if( flags & RESET_1000MB )
       emmc |= CYGHWR_XILINX_TEMAC_EMMC_LINK_SPEED(0x2);
    else if( flags & RESET_100MB )
       emmc |= CYGHWR_XILINX_TEMAC_EMMC_LINK_SPEED(0x1);
    else
       emmc |= CYGHWR_XILINX_TEMAC_EMMC_LINK_SPEED(0x0);

#if   defined(CYGSEM_DEVS_ETH_XILINX_TEMAC_PHY_INTERFACE_RGMII)
    emmc |= CYGHWR_XILINX_TEMAC_EMMC_RGMII;
#elif defined(CYGSEM_DEVS_ETH_XILINX_TEMAC_PHY_INTERFACE_SGMII)
    emmc |= CYGHWR_XILINX_TEMAC_EMMC_SGMII;
#elif defined(CYGSEM_DEVS_ETH_XILINX_TEMAC_PHY_INTERFACE_1000Base_X)
    emmc |= CYGHWR_XILINX_TEMAC_EMMC_GPCS;
    emmc |= CYGHWR_XILINX_TEMAC_EMMC_TX16;
    emmc |= CYGHWR_XILINX_TEMAC_EMMC_RX16;
#endif

    TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_EMMC, emmc );

    if( qi->if_config & CYGHWR_XILINX_TEMAC_FLOW_CONTROL_EN ) {
       TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_FCC,
           ( CYGHWR_XILINX_TEMAC_FCC_FCTX | CYGHWR_XILINX_TEMAC_FCC_FCRX ) );
    } else {
       TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_FCC, 0x0 );
    }

    if( qi->if_config & CYGHWR_XILINX_TEMAC_VLAN_EN ) {
       tc   |= CYGHWR_XILINX_TEMAC_TC_VLAN_EN;
       rcw1 |= CYGHWR_XILINX_TEMAC_RCW1_VLAN_EN;
    }

    if( ! ( qi->if_config & CYGHWR_XILINX_TEMAC_FCS_STRIP_EN ) ) {
       rcw1 |= CYGHWR_XILINX_TEMAC_RCW1_FCS;
    }

    if( qi->if_config & CYGHWR_XILINX_TEMAC_JUMBO_EN ) {
       tc   |= CYGHWR_XILINX_TEMAC_TC_JUM_EN;
       rcw1 |= CYGHWR_XILINX_TEMAC_RCW1_JUM_EN;
    }

    if( ! ( qi->if_config & CYGHWR_XILINX_TEMAC_FCS_INSERT_EN ) ) {
       tc   |= CYGHWR_XILINX_TEMAC_TC_FCS;
    }

    if( ! ( qi->if_config & CYGHWR_XILINX_TEMAC_LT_CHECK_EN ) ) {
       rcw1 |= CYGHWR_XILINX_TEMAC_RCW1_LT_EN;
    }

    if( qi->if_config & CYGHWR_XILINX_TEMAC_PROMISCUOUS_EN ) {
       TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_AFM,
                                                CYGHWR_XILINX_TEMAC_AFM_PM );
    } else {
       TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_AFM, 0x0 );
    }

#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_STATS
    qi->stats = (volatile struct temac_stats *)
              ((unsigned char *)(qi->base + CYGHWR_XILINX_TEMAC_STATS_CNT0));

    // Reset statistics
    HAL_READ_UINT32 ( qi->base + CYGHWR_XILINX_TEMAC_RAF0 , raf0);
    raf0 |= CYGHWR_XILINX_TEMAC_RAF_STATS_CNT_RESET;
    HAL_WRITE_UINT32( qi->base + CYGHWR_XILINX_TEMAC_RAF0 , raf0);
#endif

    uaw0 =   (cyg_uint32) enaddr[3] << 24
           | (cyg_uint32) enaddr[2] << 16
           | (cyg_uint32) enaddr[1] << 8
           | (cyg_uint32) enaddr[0];

    uaw1 =   (cyg_uint32) enaddr[5] << 8
           | (cyg_uint32) enaddr[4];

    TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_UAW0, uaw0 );
    TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_UAW1, uaw1 );

    TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_TC, tc );
    TEMAC_WRITE_REG( qi->base, CYGHWR_XILINX_TEMAC_RCW1, rcw1 );

    temac_eth_printf("TC register %x\n", tc);
    temac_eth_printf("RCW1 register %x\n", rcw1);
    temac_eth_printf("EMMC register %x\n", emmc);

    return true;
}

//
// This function is called to shut down the interface.
//
static void
temac_eth_stop(struct eth_drv_sc *sc)
{
    struct temac_eth_info *qi = (struct temac_eth_info *)sc->driver_private;
    cyg_uint32 reg = 0;

    temac_eth_printf("ETH TEMAC - Stop\n");

    // Disable interrupt
    HAL_WRITE_UINT32( qi->base + CYGHWR_XILINX_TEMAC_IE0, 0 );

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
    if ( qi->llf_base ) {
      HAL_WRITE_UINT32( qi->llf_base + CYGHWR_HAL_MB_XPS_LL_FIFO_IER , 0);
    }
#endif

    // Stop RX
    TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1, reg);
    reg &= ~CYGHWR_XILINX_TEMAC_RCW1_RX_EN;
    TEMAC_WRITE_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1, reg);

    // Stop TX
    TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_TC, reg);
    reg &= ~CYGHWR_XILINX_TEMAC_TC_TX_EN;
    TEMAC_WRITE_REG( qi->base , CYGHWR_XILINX_TEMAC_TC, reg);
}



//
// Initialize the interface - performed at system startup
// This function must set up the interface, including arranging to
// handle interrupts, etc, so that it may be "started" cheaply later.
//
static bool
temac_eth_init(struct cyg_netdevtab_entry *tab)
{
    struct eth_drv_sc *sc = (struct eth_drv_sc *)tab->device_instance;
    struct temac_eth_info *qi = (struct temac_eth_info *)sc->driver_private;
    cyg_uint32      speed100 = 0, full_duplex = 0, speed1000 = 0;
    bool            esa_ok;
    cyg_uint16      phy_state = 0;
    cyg_uint32      istate, tc = 0, rcw1 = 0;
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_HARD_RESET
    cyg_uint32      raf0 = 0, is0 = 0;
#endif
    bool            ret = true;

    temac_eth_printf("ETH TEMAC - Initialization for %s\n", sc->dev_name);

    HAL_DISABLE_INTERRUPTS(istate);

#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_HARD_RESET
    if( hard_reset ) {
      // Apply hard reset
      HAL_READ_UINT32 ( qi->base + CYGHWR_XILINX_TEMAC_RAF0 , raf0);
      raf0 |= CYGHWR_XILINX_TEMAC_RAF_HARD_RST;
      HAL_WRITE_UINT32( qi->base + CYGHWR_XILINX_TEMAC_RAF0 , raf0);


      // Wait reset completed
      do{
         HAL_DELAY_US(10);
         HAL_READ_UINT32 ( qi->base + CYGHWR_XILINX_TEMAC_RDY0 , raf0);
         HAL_READ_UINT32 ( qi->base + CYGHWR_XILINX_TEMAC_IS0  , is0);
      } while( ! ( raf0 & CYGHWR_XILINX_TEMAC_RDY_CFG_HARD_ACS_RDY ) ||
               ! ( is0 & CYGHWR_XILINX_TEMAC_IS_MGT_READY )    );
      hard_reset = 0;
    }
#endif

    // Reset TX
    TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_TC , tc);
    tc |= CYGHWR_XILINX_TEMAC_TC_RST;
    TEMAC_WRITE_REG( qi->base , CYGHWR_XILINX_TEMAC_TC , tc);

    // Reset RX
    TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1 , rcw1);
    rcw1 |= CYGHWR_XILINX_TEMAC_RCW1_RST;
    TEMAC_WRITE_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1 , rcw1);

    // Wait reset completed
    do{
        HAL_DELAY_US(10);
        TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_TC , tc);
        TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1 , rcw1);
    } while( ( tc & CYGHWR_XILINX_TEMAC_TC_RST ) || ( rcw1 & CYGHWR_XILINX_TEMAC_RCW1_RST ));

    // Make sure TX / RX are halted
    temac_eth_stop(sc);

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
    // Clear interrupts
    if ( qi->llf_base ) {
      HAL_WRITE_UINT32( qi->llf_base + CYGHWR_XILINX_TEMAC_IS0 , 0xff);
      HAL_WRITE_UINT32( qi->llf_base + CYGHWR_HAL_MB_XPS_LL_FIFO_ISR , CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_MASK);
    }
#endif

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
# if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
    if ( qi->llf_base ) {
      cyg_drv_interrupt_create(qi->llf_vector,
                               0x0,
                               (cyg_addrword_t)sc,
                               (cyg_ISR_t *)temac_eth_isr,
                               (cyg_DSR_t *)eth_drv_dsr,
                               &qi->llf_interrupt_handle, &qi->llf_interrupt);
      cyg_drv_interrupt_attach(qi->llf_interrupt_handle);
      qi->llf_irq_masked = 0;
      cyg_drv_interrupt_unmask(qi->llf_vector);
      cyg_drv_interrupt_create(qi->vector,
                             0x0,
                             (cyg_addrword_t)sc,
                             (cyg_ISR_t *)temac_eth_isr,
                             (cyg_DSR_t *)temac_eth_dsr,
                             &qi->interrupt_handle, &qi->interrupt);
      cyg_drv_interrupt_attach(qi->interrupt_handle);
      cyg_drv_interrupt_unmask(qi->vector);
    }
    else
# endif
    {
      cyg_drv_interrupt_create(qi->vector,
                             0x0,
                             (cyg_addrword_t)sc,
                             (cyg_ISR_t *)temac_eth_isr,
                             (cyg_DSR_t *)eth_drv_dsr,
                             &qi->interrupt_handle, &qi->interrupt);
      cyg_drv_interrupt_attach(qi->interrupt_handle);
      cyg_drv_interrupt_unmask(qi->vector);
    }
#endif

    esa_ok = CYGACC_CALL_IF_FLASH_CFG_OP(CYGNUM_CALL_IF_FLASH_CFG_GET,
                                         "temac_esa", enaddr, CONFIG_ESA);

    if (!esa_ok) {
        temac_eth_printf("ETH TEMAC - Warning! ESA unknown for %s\n", sc->dev_name);
        memcpy(&enaddr, &qi->mac_address, sizeof(enaddr));
    }

#ifdef CYGPKG_DEVS_ETH_PHY
    if (!_eth_phy_init(qi->phy)) {
        temac_eth_printf("ETH TEMAC - Failed to initialize PHY of %s\n",
                          sc->dev_name);
        ret = false;
        goto exit;
    }
    phy_state = _eth_phy_state(qi->phy);

    if(phy_state & ETH_PHY_STAT_1000MB)
      speed1000 = 1;
    if(phy_state & ETH_PHY_STAT_100MB)
      speed100 = 1;
    if(phy_state & ETH_PHY_STAT_FDX)
       full_duplex  = 1;
    if(phy_state & ETH_PHY_STAT_LINK){
        temac_eth_printf("Ethernet Mode (%s): %sMbps/%s\n",
                          sc->dev_name,
                          (speed1000 ? "1000" : speed100 ? "100" : "10"),
                          (full_duplex ? "Full" : "Half"));
    } else {
        temac_eth_printf("ETH TEMAC - NO LINK on %s\n", sc->dev_name);
    }
#else
    temac_eth_printf("ETH TEMAC - No PHY interface specified for %s\n",
                      sc->dev_name);
    ret = false;
    goto exit;
#endif

    if (!temac_eth_setup(sc, enaddr,  (full_duplex ? RESET_FULL_DUPLEX
                     : 0x00000000) | (speed100  ? RESET_100MB : 0x00000000) |
                                     (speed1000 ? RESET_1000MB : 0x00000000)))
    {
      ret = false;
      goto exit;
    }

    // restore interrupt before upper layer initialization
    HAL_RESTORE_INTERRUPTS(istate);

    // Call upper layer initialization
    (sc->funs->eth_drv->init)(sc, (unsigned char *) &enaddr);

    return ret;;

exit:
    HAL_RESTORE_INTERRUPTS(istate);
    return ret;
}

//
// This function is called to "start up" the interface.  It may be called
// multiple times, even when the hardware is already running.  It will be
// called whenever something "hardware oriented" changes and should leave
// the hardware ready to send/receive packets.
//
void
temac_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr, int flags)
{
    struct temac_eth_info *qi = (struct temac_eth_info *)sc->driver_private;
    cyg_uint32 reg = 0;

    temac_eth_printf("ETH TEMAC - Start\n");

    // Start TX
    TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_TC, reg);
    reg |= CYGHWR_XILINX_TEMAC_TC_TX_EN;
    TEMAC_WRITE_REG( qi->base , CYGHWR_XILINX_TEMAC_TC, reg);

    // Start RX
    TEMAC_READ_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1, reg);
    reg |= CYGHWR_XILINX_TEMAC_RCW1_RX_EN;
    TEMAC_WRITE_REG( qi->base , CYGHWR_XILINX_TEMAC_RCW1, reg);

    // Enable interrupts
    HAL_WRITE_UINT32( qi->base + CYGHWR_XILINX_TEMAC_IE0 ,
                                       CYGHWR_XILINX_TEMAC_IE_DEFAULT_MASK );

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
    if ( qi->llf_base ) {
      HAL_WRITE_UINT32( qi->llf_base + CYGHWR_HAL_MB_XPS_LL_FIFO_IER ,
                                         CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_MASK);
    }
#endif
}

//
// This function is called for low level "control" operations
//
int
temac_eth_control(struct eth_drv_sc *sc, unsigned long key,
                   void *data, int length)
{
    switch (key)
    {
    case ETH_DRV_SET_MAC_ADDRESS:
        return 0;
        break;
#ifdef ETH_DRV_SET_MC_ALL
    case ETH_DRV_SET_MC_ALL:
    case ETH_DRV_SET_MC_LIST:
        // TODO
        return 0;
        break;
#endif
    default:
        return 1;
        break;
    }
}


//
// Interrupt vector
//
static int
temac_eth_int_vector(struct eth_drv_sc *sc)
{
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    return (qi->vector);
#else
    return 0;
#endif
}


//
// Interrupt processing
//
static void
temac_eth_int(struct eth_drv_sc *sc)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    cyg_uint32 isr = 0;

#if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)

     if ( qi->llf_base ) {
         // Check and clear interrupt
         HAL_READ_UINT32 ( (qi->llf_base + CYGHWR_HAL_MB_XPS_LL_FIFO_ISR), isr);
         HAL_WRITE_UINT32( (qi->llf_base + CYGHWR_HAL_MB_XPS_LL_FIFO_ISR), isr);
         if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TC ) {
             temac_eth_printf("%s - Transmit packet irq\n", sc->dev_name);
             temac_eth_ll_fifo_TxEvent(sc);
             isr &= ~CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TC;
         }
         if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RC ) {
             temac_eth_printf("%s - Receive packet irq\n", sc->dev_name);
             temac_eth_ll_fifo_RxEvent(sc);
             isr &= ~CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RC;
         }
         if ( isr ) {
           temac_eth_ll_fifo_Error( sc, isr );
         }
     }

#endif

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
# if defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH0_LL_FIFO) || \
        defined(CYGPKG_DEVS_ETH_XILINX_TEMAC_ETH1_LL_FIFO)
     cyg_drv_interrupt_unmask( qi->llf_vector );
# endif
#endif
}


#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
//
// TEMAC controller interrupt
//

static void
temac_int(struct eth_drv_sc *sc)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    cyg_uint32 isr = 0;

    // Check interrupts
    HAL_READ_UINT32 ( (qi->base + CYGHWR_XILINX_TEMAC_IP0), isr);

    if( isr & CYGHWR_XILINX_TEMAC_IS_HARD_ACS_CMPLT ) {
        temac_eth_printf("%s - TEMAC, hard reset completed\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IS_AUTO_NEG ) {
        temac_eth_printf("%s - TEMAC, Auto negotiation completed\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IS_RX_CMPLT ) {
        temac_eth_printf("%s - TEMAC, RX completed\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IS_RX_REJECT ) {
        temac_err_printf("%s - TEMAC, RX rejected\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IS_RX_FIFO_OVR ) {
        temac_err_printf("%s - TEMAC, RX FIFO overrun\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IP_TX_CMPLT ) {
        temac_eth_printf("%s - TEMAC, TX completed\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IS_RX_DCM_LOCK ) {
        temac_eth_printf("%s - TEMAC, DCM lock\n",
             sc->dev_name);
    }
    if( isr & CYGHWR_XILINX_TEMAC_IS_MGT_READY ) {
        temac_eth_printf("%s - TEMAC, MGMT ready\n",
             sc->dev_name);
    }

    // Clear interrupts
    HAL_WRITE_UINT32( (qi->base + CYGHWR_XILINX_TEMAC_IS0), isr);
}
#endif


//
// Polling function
//
static void
temac_eth_poll(struct eth_drv_sc *sc)
{
     temac_eth_int(sc);
     CYGACC_CALL_IF_DELAY_US(500);
}


//
// Display ETH statistics
//
#ifdef CYGSEM_DEVS_ETH_XILINX_TEMAC_STATS
externC void
temac_disp_stats(struct eth_drv_sc *sc , char flag )
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;

    diag_printf("Ethernet Statistics\n");

    diag_printf("%17s %10d\n",
        "TX BYTES (UPPER)",
        qi->stats.txbu);
    diag_printf("%17s %10d\n",
        "TX BYTES (LOWER)",
        qi->stats.txbl);
    diag_printf("%17s %10d\n",
        "TX FRAMES OK (UPPER)",
        qi->stats.txfu);
    diag_printf("%17s %10d\n",
        "TX FRAMES OK (LOWER)",
        qi->stats.txfl);
    diag_printf("%17s %10d\n",
        "RX BYTES (UPPER)",
        qi->stats.rxbu);
    diag_printf("%17s %10d\n",
        "RX BYTES (LOWER)",
        qi->stats.rxbl);
    diag_printf("%17s %10d\n",
        "RX FRAMES OK (UPPER)",
        qi->stats.rxfu);
    diag_printf("%17s %10d\n",
        "RX FRAMES OK (LOWER)",
        qi->stats.rxfl);

    // TODO...
#endif

// EOF if_temac.c
