#ifndef CYGONCE_DEVS_ETHERNET_FREESCALE_FMAN_H
#define CYGONCE_DEVS_ETHERNET_FREESCALE_FMAN_H
//==========================================================================
//
//      eth_fman.h
//
//      Freescale Ethernet layer : Frame Manager
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
// Date:         2013-01-20
// Purpose:
// Description:  Ethernet driver using DPAA Frame Manager
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

//
// The following enumeration defines the Media Access
// Controller type

enum fman_mac_type {
  FMAN_MAC_DTSEC = 0x01,
  FMAN_MAC_10GEC,
};

enum mac_if_type {
  MODE_MII = 0x01,
  MODE_RMII,
  MODE_RGMII,
  MODE_TBI,
  MODE_RTBI,
  MODE_SGMII,
  MODE_QSGMII
};

enum mac_ctl_type {
  MAC_CTL_RX = 0x01,
  MAC_CTL_TX,
  MAC_CTL_ALL
};

//
// The following structure defines the API allowing access
// to the MAC layer from the FMAN Ethernet layer

struct mac_drv_funs {
  // MAC layer - initialization
  cyg_uint32 (*init)(struct eth_drv_sc *sc, cyg_uint8 *enaddr, cyg_uint32 speed,
                     cyg_uint32 dplx, cyg_uint32 mode);
  // Allow transmission/reception
  cyg_uint32 (*start)(struct eth_drv_sc *sc, cyg_uint32 ctl);
  // Shut down hardware
  cyg_uint32 (*stop)(struct eth_drv_sc *sc, cyg_uint32 ctl);
};

struct mac_drv_sc {
  struct mac_drv_funs *funs;
};

#define FMAN_MAC_ENTRIES(sc,init,start,stop)                              \
static cyg_uint32 start(struct eth_drv_sc *sc, cyg_uint32 ctl);           \
static cyg_uint32 stop(struct eth_drv_sc *sc, cyg_uint32 ctl);            \
static cyg_uint32 init(struct eth_drv_sc *sc, cyg_uint8 *enaddr,          \
                     cyg_uint32 speed, cyg_uint32 dplx, cyg_uint32 mode); \
static struct mac_drv_funs sc##_funs = {                                  \
  init,                                                                   \
  start,                                                                  \
  stop};                                                                  \
struct mac_drv_sc sc = {&sc##_funs};

//
// MAC layer data structure

struct mac_eth_cfg {
  cyg_uint8          address[6];  // MAC (hardware) address
  cyg_uint16         num;         // 1 = DTSEC1 or GEC1, 2= DTSEC2 etc...
  cyg_uint32         type;        // DTSEC or GEC
  cyg_uint32         mac2phy;     // MODE_MII, MODE_RGMII etc..
  struct mac_drv_sc *sc;
};

//
// Frame Manager data structure

struct fman_eth_cfg {
  cyg_uint32         num;         // FMAN1, FMAN2 etc..
  cyg_uint32         im;          // Independent Mode
  struct fman_tx_port_cfg  tx_port;
  struct fman_rx_port_cfg  rx_port;
};

#define VSTRUCT volatile struct

//
// The following structure contains the Frame Manager Ethernet driver
// internal data.

#ifdef CYGPKG_KERNEL_SMP_SUPPORT
typedef struct {
  cyg_uint32            rx_pkt;             // Number of packet transmitted / sent
  cyg_uint32            tx_pkt;             // by a specific core
 } dtsec_smp_stats_t;
#endif

struct eth_fman_t {
  VSTRUCT fman_imgp_ram *fman_imgp_ram;     // Global parameter
  VSTRUCT dtsec_reg     *dtsec;             // DTSEC memory map, Applicable only for DTSEC MAC
  VSTRUCT gec_reg       *gec;               // Applicable only for 10GEC MAC
  VSTRUCT fman_bd       *txbd, *rxbd;       // Next Tx,Rx descriptor to use
  VSTRUCT fman_bd       *tbase, *rbase;     // First Tx,Rx descriptor
  VSTRUCT fman_bd       *ttop, *rtop;       // Last Tx,Rx descriptor
  VSTRUCT fman_bd       *tnext, *rnext;     // Next descriptor to check for interrupt
  cyg_uint32             txsize, rxsize;    // Length of individual buffers
  cyg_uint32             txactive;          // Count of active Tx buffers
  cyg_uint32             rxmax;             // Debug RX counter
#ifdef CYGPKG_KERNEL_SMP_SUPPORT
  cyg_uint32             rx_irq_in_progress;
#endif
  cyg_uint32             txkey[1024];
  cyg_uint8             *init_rxbufs;       // Initial base pointer of RX buffers
  cyg_uint8             *init_txbufs;       // Initial base pointer of TX buffers
  VSTRUCT fman_bd       *init_rxring;       // Initial base pointer of RX ring
  VSTRUCT fman_bd       *init_txring;       // Initial base pointer of TX ring
  cyg_uint32             rxnum;             // Number of RX Buffer
  cyg_uint32             txnum;             // Number of TX Buffer
  struct mac_eth_cfg    *mac;
  struct fman_eth_cfg   *fman;
  cyg_int8               fpm_event;
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
  cyg_uint8              tx_irq_vector;     // Tx interrupt Vector
  cyg_uint8              rx_irq_vector;     // Rx interrupt Vector
  cyg_uint8              err_irq_vector;    // Error interrupt Vector
  cyg_interrupt          eth_interrupt_tx;
  cyg_interrupt          eth_interrupt_rx;
  cyg_interrupt          eth_interrupt_err;
  cyg_handle_t           eth_interrupt_handle_tx;
  cyg_handle_t           eth_interrupt_handle_rx;
  cyg_handle_t           eth_interrupt_handle_err;
  bool                   rx_mask;           // This 3 flags are use to cope with the Network driver not
  bool                   tx_mask;           // supplying the IRQ vector when the deliver function is called
  bool                   err_mask;
#endif
  volatile dtsec_mii_t  *mii;               // Select MII interface
  bool                   reset_mii;
#ifdef CYGPKG_DEVS_ETH_PHY
  eth_phy_access_t      *phy;
  void (*plf_phy_init)(eth_phy_access_t *);      // Platform specific PHY init
  void (*create_phy_irq)(struct eth_drv_sc *sc); // HW specific if PHY irq is wanted
#endif
  cyg_uint8              state;
#ifdef CYGPKG_KERNEL_SMP_SUPPORT
  cyg_spinlock_t         rx_spin_lock;           // Support for SMP processor
  cyg_spinlock_t         tx_spin_lock;
  dtsec_smp_stats_t      smp_stats[CYGPKG_HAL_SMP_CPU_MAX];
#endif
};

#define FMAN_MAC_SUCCESS                0x0000
#define FMAN_MAC_FAILURE                0x0001

#ifdef CYGPKG_DEVS_ETH_PHY
static void dtsec1_phy_put_reg(int , int , unsigned short );
static bool dtsec1_phy_get_reg(int , int , unsigned short *);
#endif

#define CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_RX_LOG2 (11)
#define CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_TX_LOG2 (11)
#define CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_RX (1<<CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_RX_LOG2)
#define CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_TX (1<<CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_TX_LOG2)

#define FMAN_STAT_ALL_BDS                0x01
#define FMAN_STAT_CURRENT_BD             0x02
#define FMAN_STAT_MAC_RMON               0x04
#define FMAN_STAT_SMP                    0x08

#ifdef CYGSEM_DEVS_ETH_FREESCALE_FMAN_DEBUG_CODE
externC void fman_disp_rx_bd_stats( struct eth_drv_sc * , char );
externC void fman_disp_tx_bd_stats( struct eth_drv_sc * , char );
externC void fman_disp_imgp( struct eth_drv_sc * );
#endif

#endif
