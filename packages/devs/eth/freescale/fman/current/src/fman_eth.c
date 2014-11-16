//==========================================================================
//
//      dev/fman_eth.c
//
//      Ethernet device driver for PowerPC MPC83xx, MPC85xx boards
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
// Date:         2013-02-01
// Purpose:      
// Description:  hardware driver for Freescale Frame Manager
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/devs_eth_freescale_fman.h>
#include <pkgconf/io_eth_drivers.h>

#ifdef CYGPKG_NET
#include <pkgconf/net.h>
#endif

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/ppc_regs.h>

#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>

#ifdef CYGPKG_DEVS_ETH_PHY
#include <cyg/io/eth_phy.h>
#endif

#include "cyg/hal/var_regs.h"
#include "cyg/io/fman.h"
#include "cyg/io/fman_port.h"
#include "cyg/io/dtsec.h"
#include "cyg/io/10gec.h"
#include "cyg/io/eth_fman.h"

#include <string.h> // memset, memcpy

static unsigned char enaddr[6];

#ifdef CYGSEM_DEVS_ETH_FREESCALE_FMAN_CHATTER
#include <cyg/infra/diag.h>
#define feth_printf(args...)   diag_printf(args)
#else
#define feth_printf(args...)   /* NOOP */
#endif

#include <cyg/io/phy.inl>
#include <cyg/io/dtsec.inl>
#include <cyg/io/10gec.inl>

#include CYGDAT_DEVS_ETH_FREESCALE_FMAN_PLF_H
#include CYGDAT_DEVS_ETH_FREESCALE_FMAN_PLF_INL

#ifdef CYGPKG_REDBOOT
#include <pkgconf/redboot.h>

#ifdef CYGSEM_REDBOOT_FLASH_CONFIG
#include <redboot.h>
#include <flash_config.h>
RedBoot_config_option("Network hardware address [MAC]",
        fman_esa,
        ALWAYS_ENABLED, true,
        CONFIG_ESA, 0
);
#endif
#endif

// For fetching the ESA from RedBoot
#include <cyg/hal/hal_if.h>
#ifndef CONFIG_ESA
#define CONFIG_ESA 6
#endif

#ifndef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
#ifdef CYGPKG_KERNEL
#define STACK_SIZE CYGNUM_HAL_STACK_SIZE_MINIMUM
static char tsec_fake_int_stack[DTSEC_MAX_INTERFACE][STACK_SIZE];
static cyg_thread tsec_fake_int_thread_data[DTSEC_MAX_INTERFACE];
static cyg_handle_t tsec_fake_int_thread_handle[DTSEC_MAX_INTERFACE];
static int tsec_fake_int_count = 0;
static void tsec_fake_int(cyg_addrword_t);
#endif
#endif // CYGINT_IO_ETH_INT_SUPPORT_REQUIRED

// Deliver function (ex-DSR) handles the ethernet [logical] processing
static void
fman_eth_deliver(struct eth_drv_sc * sc)
{
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
#endif

  fman_eth_int(sc);

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
  // Allow interrupts to happen again
  cyg_drv_interrupt_unmask(qi->rx_irq_vector);
#endif
}

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
// This ISR is called when the ethernet interrupt occurs
static int
fman_eth_isr(cyg_vector_t vector, cyg_addrword_t data,
       HAL_SavedRegisters *regs)
{
  cyg_drv_interrupt_mask(vector);
  cyg_drv_interrupt_acknowledge(vector);

  return (CYG_ISR_HANDLED | CYG_ISR_CALL_DSR); // Run the DSR
}
#endif


//
// Initialize the Independent Mode Global Parameter for the RX and TX
// ports.
static bool
fman_init_global_param(struct eth_fman_t *qi)
{
  cyg_uint32 rx_imgp_base, rx_imgp_offset;
  cyg_uint8 *RxBUF, *TxBUF, i;
  volatile struct fman_bd *rxbd, *txbd;

  // Allocate memory from FMAN internal memory
  rx_imgp_base = FMAN_IMEM_ETH_ALLOC( qi->fman->num, sizeof(struct fman_imgp_ram),
                                                       FMAN_FMBM_BUFFER_ALIGN);
  rx_imgp_offset = rx_imgp_base - FMAN_IMEM_BASE(  qi->fman->num );
  qi->fman_imgp_ram = (volatile struct fman_imgp_ram *) rx_imgp_base;

  if( qi->fman_imgp_ram == NULL )
    return false;

  // Clear BDs memory
  rxbd = qi->init_rxring;
  txbd = qi->init_txring;
  qi->rbase = qi->rxbd = qi->rnext = rxbd;
  qi->tbase = qi->txbd = qi->tnext = txbd;
  memset((void *)rxbd, 0, sizeof(struct fman_bd) * qi->rxnum);
  memset((void *)txbd, 0, sizeof(struct fman_bd) * qi->txnum);
  qi->rtop = qi->rbase + qi->rxnum;
  qi->ttop = qi->tbase + qi->txnum;

  RxBUF = qi->init_rxbufs;
  TxBUF = qi->init_txbufs;

  // Initialize BDs
  for (i = 0; i < qi->rxnum; i++)
  {
    rxbd->ctrl   = FMAN_BD_Rx_Empty;
    rxbd->buf_lo = (cyg_uint32) RxBUF;
    RxBUF += CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_RX;
    rxbd++;
  }
  for (i = 0; i < qi->txnum; i++)
  {
    txbd->ctrl   = FMAN_BD_Tx_Last;
    txbd->buf_lo = (cyg_uint32) TxBUF;
    TxBUF += CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_TX;
    txbd++;
  }

  //
  // Now we fill the structure
  qi->fman_imgp_ram->imr       = FMAN_IMGP_IMR_GBL;
  qi->fman_imgp_ram->rx_qd_ptr = rx_imgp_offset + 0x20;
  qi->fman_imgp_ram->tx_qd_ptr = rx_imgp_offset + 0x40;
  qi->fman_imgp_ram->mrblr     = CYGNUM_DEVS_ETH_FREESCALE_FMAN_BUFSIZE_RX_LOG2;
  qi->fman_imgp_ram->rx_qd_bsy_cnt = 0;

  // Fill the RX Queue descriptor
  memset((void *)&qi->fman_imgp_ram->rx_qd.ctrl, 0, sizeof(struct fman_qd));
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
  qi->fpm_event = fman_allocate_event(qi->fman->num);
  if ( qi->fpm_event < 0 ){
    diag_printf("FMAN%d - Error allocating FMAN event\n", qi->fman->num);
    return false;
  }
  qi->fman_imgp_ram->rx_qd.ctrl         = FMAN_QD_RX_FRAME_IM |
                                          FMAN_QD_BUSY_IM |
                                          FMAN_QD_BUSY_SET_FPMEVT_SEL(qi->fpm_event);
#endif
  qi->fman_imgp_ram->rx_qd.ring_base_lo = (cyg_uint32) qi->init_rxring;
  qi->fman_imgp_ram->rx_qd.ring_size    =
            sizeof(struct fman_bd) * qi->rxnum;

  // Fill the TX Queue descriptor
  memset((void *)&qi->fman_imgp_ram->tx_qd.ctrl, 0, sizeof(struct fman_qd));
  qi->fman_imgp_ram->tx_qd.ring_base_lo = (cyg_uint32) qi->init_txring;
  qi->fman_imgp_ram->tx_qd.ring_size    =
            sizeof(struct fman_bd) * qi->txnum;

  qi->fman->rx_port.dfqid  = rx_imgp_offset;
  qi->fman->tx_port.tcfqid = rx_imgp_offset;

  return true;
}


//
// This function is called to shut down the interface.
//
static void
fman_eth_stop(struct eth_drv_sc *sc)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  struct mac_drv_sc *mac_sc = qi->mac->sc;
  cyg_uint32 rx_port  = qi->fman->rx_port.port;
  cyg_uint32 tx_port  = qi->fman->tx_port.port;
  cyg_uint32 fman_num = qi->fman->num;
  cyg_uint32 im       = qi->fman->im;

  feth_printf("FMAN%d-%s - Stop\n", qi->fman->num, sc->dev_name);

  // Stop MAC layer (RX)
  mac_sc->funs->stop(sc, MAC_CTL_RX);
  // Stop FMAN port
  FMAN_ETH_RX_PORT_ONOFF( fman_num, rx_port, qi->fman_imgp_ram, FMAN_PORT_OFF, im);

  // Stop MAC layer (TX)
  mac_sc->funs->stop(sc, MAC_CTL_TX);
  // Stop FMAN port
  FMAN_ETH_TX_PORT_ONOFF( fman_num, tx_port, qi->fman_imgp_ram, FMAN_PORT_OFF, im);
}


//
// Initialize the interface - performed at system startup
// This function must set up the interface, including arranging to
// handle interrupts, etc, so that it may be "started" cheaply later.
//
static bool
fman_eth_init(struct cyg_netdevtab_entry *tab)
{
  struct eth_drv_sc *sc = (struct eth_drv_sc *) tab->device_instance;
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  struct mac_drv_sc *mac_sc = NULL;
  cyg_uint8  f = (cyg_uint8) qi->fman->num;
  cyg_uint32 istate, speed = 10, full_duplex = 0;
  bool ret = false, esa_ok;
#ifdef CYGPKG_DEVS_ETH_PHY
  cyg_uint16 phy_state = 0;
#endif

  feth_printf("FMAN%d-%s - Initialization\n", f, sc->dev_name);

  HAL_DISABLE_INTERRUPTS(istate);

  //
  // Some sanity check of the user data
  if( qi->mac == NULL ){
    diag_printf("FMAN%d-%s - Missing MAC layer info\n", f, sc->dev_name);
    goto exit;
  }
  if( qi->fman == NULL ){
    diag_printf("FMAN%d-%s - Missing FMAN info\n", f, sc->dev_name);
    goto exit;
  }
  mac_sc = qi->mac->sc;
  if( mac_sc == NULL ){
    diag_printf("FMAN%d-%s - Missing MAC layer call function\n", f, sc->dev_name);
    goto exit;
  }

  //
  // Map MAC layer registers
  if( qi->mac->type == FMAN_MAC_DTSEC ) {
    dtsec_eth_set_if( sc, f, qi->mac->num );
    if( qi->dtsec == NULL ) {
       diag_printf("FMAN%d-%s - MAC mapping failed\n", f, sc->dev_name);
       goto exit;
    }
  }
  else if( qi->mac->type == FMAN_MAC_10GEC ) {
    gec_eth_set_if( sc, f, qi->mac->num );
    if( qi->dtsec == NULL ) {
       diag_printf("FMAN%d-%s - MAC mapping failed\n", f, sc->dev_name);
       goto exit;
    }
  }
  else {
    diag_printf("FMAN%d-%s - MAC layer unknown\n", f, sc->dev_name);
    goto exit;
  }

#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    if( f == 1 ) {
      qi->rx_irq_vector = CYGNUM_HAL_INTERRUPT_FMAN1;
    }
# ifdef CYGNUM_HAL_INTERRUPT_FMAN2
    else {
      qi->rx_irq_vector = CYGNUM_HAL_INTERRUPT_FMAN2;
    }
# endif
    cyg_drv_interrupt_create(qi->rx_irq_vector,
                            8,
                            (cyg_addrword_t) sc, //  Data item passed to interrupt handler
                            (cyg_ISR_t *) fman_eth_isr,
                            (cyg_DSR_t *) eth_drv_dsr,
                            &qi->eth_interrupt_handle_rx,
                            &qi->eth_interrupt_rx);
    cyg_drv_interrupt_attach(qi->eth_interrupt_handle_rx);
    cyg_drv_interrupt_unmask(qi->rx_irq_vector);
#endif

  //
  // FMAN layer Initialization
  if((ret = fman_init_global_param( qi )) == false)
    goto exit;

  esa_ok = CYGACC_CALL_IF_FLASH_CFG_OP(CYGNUM_CALL_IF_FLASH_CFG_GET,
              "fman_esa", enaddr, CONFIG_ESA);

  if (!esa_ok)
  {
    feth_printf("FMAN%d-%s - Warning! ESA unknown\n",
        f, sc->dev_name);
    memcpy(&enaddr, &qi->mac->address[0], sizeof(enaddr));
  }

#ifdef CYGPKG_DEVS_ETH_PHY
  //
  // Reset and setup the MII interface
  if(qi->reset_mii == true)
     dtsec_reset_phy(qi->mii);

  //
  // This is not really a PHY initialization, just looking up the PHY
  // device table for any match found from the MII bus
  if (!_eth_phy_init(qi->phy)) {
     feth_printf("FMAN%d-%s - Failed to initialize PHY\n",
        f, sc->dev_name);
     ret = false;
     goto exit;
  }

  //
  // Apply specific PHY register initialization required for the platform
  if(qi->plf_phy_init != NULL)
    qi->plf_phy_init(qi->phy);

  //
  // Get result from the auto-negotiation process
  phy_state = _eth_phy_state(qi->phy);

  if(phy_state & ETH_PHY_STAT_1000MB)
    speed = 1000;
  if(phy_state & ETH_PHY_STAT_100MB)
    speed = 100;
  if(phy_state & ETH_PHY_STAT_FDX)
     full_duplex  = 1;
  if(phy_state & ETH_PHY_STAT_LINK){
     feth_printf("Ethernet Mode (%s): %dMbps/%s\n",
         sc->dev_name,
         speed,
         (full_duplex ? "Full" : "Half"));
  }
  else {
      feth_printf("FMAN%d-%s - NO LINK\n", \
      f, sc->dev_name);
  }
#else
  feth_printf("FMAN%d-%s - No PHY interface specified\n",
      f, sc->dev_name);
  ret = false;
  goto exit;
#endif

  //
  // MAC layer Initialization
  mac_sc->funs->init(sc, &qi->mac->address[0], speed, full_duplex, qi->mac->mac2phy);

  //
  // Setup port
  fman_bmi_cfg_rx_port( &qi->fman->rx_port, f, qi->fman->im );
  fman_bmi_cfg_tx_port( &qi->fman->tx_port, f, qi->fman->im );

  // Call upper layer
  (sc->funs->eth_drv->init)(sc, (unsigned char *) &enaddr);

  fman_umask_event( qi->fman->num, qi->fpm_event, (FMAN_FPM_FMCEV_RXF | FMAN_FPM_FMCEV_BSY) );

exit:
#ifdef CYGPKG_KERNEL_SMP_SUPPORT
  cyg_drv_spinlock_clear ( &qi->tx_spin_lock );
  cyg_drv_spinlock_clear ( &qi->rx_spin_lock );
#endif

  HAL_RESTORE_INTERRUPTS(istate);

  return ret;
}

//
// This function is called to "start up" the interface.  It may be called
// multiple times, even when the hardware is already running.  It will be
// called whenever something "hardware oriented" changes and should leave
// the hardware ready to send/receive packets.
//
static void
fman_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr,
        int flags)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  struct mac_drv_sc *mac_sc = qi->mac->sc;
  cyg_uint32 rx_port  = qi->fman->rx_port.port;
  cyg_uint32 tx_port  = qi->fman->tx_port.port;
  cyg_uint32 fman_num = qi->fman->num;
  cyg_uint32 im       = qi->fman->im;

  feth_printf("FMAN%d-%s - Start\n", qi->fman->num, sc->dev_name);

  // Start MAC layer (RX)
  mac_sc->funs->start(sc, MAC_CTL_RX);
  // Start FMAN port
  FMAN_ETH_RX_PORT_ONOFF( fman_num, rx_port, qi->fman_imgp_ram, FMAN_PORT_ON, im );

  // Start MAC layer (TX)
  mac_sc->funs->start(sc, MAC_CTL_TX);
  // Start FMAN port
  FMAN_ETH_TX_PORT_ONOFF( fman_num, tx_port, qi->fman_imgp_ram, FMAN_PORT_ON, im );
}

//
// This function is called for low level "control" operations
//
static int
fman_eth_control(struct eth_drv_sc *sc, unsigned long key,
           void *data, int length)
{
#ifdef ETH_DRV_SET_MC_ALL
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
#endif

  switch (key)
  {
  case ETH_DRV_SET_MAC_ADDRESS:
      return 0;
      break;
#ifdef ETH_DRV_SET_MC_ALL
  case ETH_DRV_SET_MC_ALL:
  case ETH_DRV_SET_MC_LIST:
      if( qi->mac->type == FMAN_MAC_DTSEC ) {
        qi->dtsec->gaddr[0] = 0xFFFFFFFF;
        qi->dtsec->gaddr[1] = 0xFFFFFFFF;
      } else {
        // GEC TBD
      }
      return 0;
      break;
#endif
  default:
      return 1;
      break;
  }
}

//
// This function is called to see if another packet can be sent.
// It should return the number of packets which can be handled.
// Zero should be returned if the interface is busy and can not send any more.
//
static int
fman_eth_can_send(struct eth_drv_sc *sc)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;

  return (qi->txactive < qi->txnum);
}


static void
fman_eth_TxEvent(struct eth_drv_sc *sc)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct fman_bd *txbd;
  cyg_uint32 key, txindex;

#ifdef CYGPKG_KERNEL_SMP_SUPPORT
  qi->smp_stats[HAL_SMP_CPU_THIS()].tx_pkt++;
#endif

  txbd = qi->tnext;

  // Note: length field is used to indicate the buffer has/had data in it
  while ( (txbd->length != 0) && ((txbd->ctrl & FMAN_BD_Tx_Ready) == 0) )
  {
    txbd->length = 0;
    txindex = ((unsigned long) txbd - (unsigned long) qi->tbase)
                / sizeof(*txbd);
    if ( (key = qi->txkey[txindex]) != 0 ) {
      qi->txkey[txindex] = 0;
      (sc->funs->eth_drv->tx_done)(sc, key, 0);
    }
    qi->txactive--;
    txbd++;
    if (txbd >= qi->ttop) {
      txbd = qi->tbase;
    }
  }
  // Remember where we left off
  qi->tnext = (struct fman_bd *) txbd;
}

//
// This function is called when a packet has been received.  It's job is
// to prepare to unload the packet from the hardware.  Once the length of
// the packet is known, the upper layer of the driver can be told.  When
// the upper layer is ready to unload the packet, the internal function
// 'fec_eth_recv' will be called to actually fetch it from the hardware.
//
static void
fman_eth_RxEvent(struct eth_drv_sc *sc)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct fman_bd *rxbd, *rxfirst;
  volatile cyg_uint16 offset_out;
  cyg_int16 cnt_bd = 0;

#ifdef CYGPKG_KERNEL_SMP_SUPPORT
  qi->rx_irq_in_progress = 1;
  qi->smp_stats[HAL_SMP_CPU_THIS()].rx_pkt++;
#endif

  rxbd = rxfirst = qi->rnext;
  offset_out = qi->fman_imgp_ram->rx_qd.offset_out;
  while ((rxbd->ctrl & FMAN_BD_Rx_Empty) == 0) {
    cnt_bd++;
    qi->rxbd = rxbd; // Save for callback
    (sc->funs->eth_drv->recv)(sc, rxbd->length - 4);
    rxbd++;
    offset_out += sizeof(struct fman_bd);
    if (rxbd >= qi->rtop) {
      rxbd = qi->rbase;
    }
  }

  // Update the IM global parameters
  if ( offset_out >= qi->fman_imgp_ram->rx_qd.ring_size )
    qi->fman_imgp_ram->rx_qd.offset_out = 0;
  else
    qi->fman_imgp_ram->rx_qd.offset_out = offset_out;

  // Remember where we left off
  qi->rnext = (struct fman_bd *) rxbd;

  if(cnt_bd > qi->rxmax)
     qi->rxmax = cnt_bd;

#ifdef CYGPKG_KERNEL_SMP_SUPPORT
  qi->rx_irq_in_progress = 0;
#endif
}

//
// This routine is called to send data to the hardware.
static void
fman_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
         int sg_len, int total_len, unsigned long key)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct fman_bd *txbd, *txfirst;
  volatile cyg_uint8 *bp;
  volatile cyg_uint16 offset_in;
  cyg_uint32 i, txindex;

  // Find a free buffer
  txbd = txfirst = qi->txbd;
  if( txbd->ctrl & FMAN_BD_Tx_Ready ){
    feth_printf("FMAN%d-%s - No free xmit buffers\n",
        qi->fman->num, sc->dev_name);
    return;
  }

  // Set up buffer
  bp = (cyg_uint8 *) txbd->buf_lo;
  for (i = 0; i < sg_len; i++) {
    memcpy((void *) bp, (void *) sg_list[i].buf, sg_list[i].len);
    bp += sg_list[i].len;
  }

  txbd->length = total_len;
  txindex = ((unsigned long) txbd - (unsigned long) qi->tbase)
            / sizeof(*txbd);
  qi->txkey[txindex] = key;

   // Set BDs has ready, single buffer for BD
  HAL_MEMORY_BARRIER();
  txbd->ctrl = FMAN_BD_Tx_Ready | FMAN_BD_Tx_Last;
  HAL_MEMORY_BARRIER();

  qi->txactive++;

  // Update the IM global parameters
  offset_in  =
            qi->fman_imgp_ram->tx_qd.offset_in + sizeof(struct fman_bd);
  if ( offset_in >= qi->fman_imgp_ram->tx_qd.ring_size )
    qi->fman_imgp_ram->tx_qd.offset_in = 0;
  else
    qi->fman_imgp_ram->tx_qd.offset_in = offset_in;

  HAL_MEMORY_BARRIER();

  // Remember the next buffer to try
  qi->txbd = txbd + 1;
  if (qi->txbd >= qi->ttop) {
    qi->txbd = qi->tbase;
  }
}

//
// This function is called as a result of the "eth_drv_recv()" call above.
// It's job is to actually fetch data for a packet from the hardware once
// memory buffers have been allocated for the packet.  Note that the buffers
// may come in pieces, using a scatter-gather list.  This allows for more
// efficient processing in the upper layers of the stack.
//
static void
fman_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  cyg_uint8  *bp;
  cyg_uint16   i;

  bp = (cyg_uint8 *) qi->rxbd->buf_lo;

  for (i = 0; i < sg_len; i++) {
    if (sg_list[i].buf != 0) {
      memcpy((void *) sg_list[i].buf, bp, sg_list[i].len);
      bp += sg_list[i].len;
    }
  }
  qi->rxbd->length = 0;
  qi->rxbd->ctrl |= FMAN_BD_Rx_Empty;
  HAL_MEMORY_BARRIER();
}

//
// Interrupt processing
//
static void
fman_eth_int(struct eth_drv_sc *sc)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  cyg_uint32 event = fman_get_event( qi->fman->num, qi->fpm_event );

  if(event & FMAN_FPM_FMCEV_RXF){
    fman_eth_RxEvent( sc );
    // Temporary solution, we also process TX since we do not have
    // TX specific interrupt
    fman_eth_TxEvent( sc );
  }

  if(event & FMAN_FPM_FMCEV_BSY){
    diag_printf("ETH - RX Busy condition detected\n");
  }

  fman_clear_event( qi->fman->num, qi->fpm_event, event );
}

//
// Interrupt vector
//
static int
fman_eth_int_vector(struct eth_drv_sc *sc)
{
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  return (qi->rx_irq_vector);
#else
  return 0;
#endif
}

#if !defined(CYGINT_IO_ETH_INT_SUPPORT_REQUIRED) && defined(CYGPKG_KERNEL)
void
tsec_fake_int(cyg_addrword_t param)
{
    struct eth_drv_sc *sc = (struct eth_drv_sc *) param;
    cyg_uint32 int_state;

    while (true)
    {
       cyg_thread_delay(1); // 10ms
       HAL_DISABLE_INTERRUPTS(int_state);
       fman_eth_int(sc);
       HAL_RESTORE_INTERRUPTS(int_state);
    }
}
#endif

#ifdef CYGSEM_DEVS_ETH_FREESCALE_FMAN_DEBUG_CODE

externC void
fman_disp_rx_bd_stats( struct eth_drv_sc *sc , char flag )
{
  int i, current_bd = -1;
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct fman_bd *rxbd;

  rxbd = qi->rbase;

  diag_printf("RX BDs\n");
  diag_printf("------\n");

  if( flag & FMAN_STAT_ALL_BDS )
  {
    diag_printf("Idx - Empty - Last -  Len - Buffer\n");
  }

  for (i = 0; i < qi->rxnum; i++)
  {
    if( flag & FMAN_STAT_ALL_BDS )
    {
      diag_printf("%3d - %5d - %4d - %4d - 0x%08x\n",
         i, \
         ((rxbd->ctrl & FMAN_BD_Rx_Empty) ? 1 : 0),
         ((rxbd->ctrl & FMAN_BD_Rx_Last) ? 1 : 0),
         rxbd->length,
         rxbd->buf_lo);
    }
    if(rxbd == qi->rnext)
       current_bd = i;
    rxbd++;
  }

  if( flag & FMAN_STAT_CURRENT_BD )
  {
     rxbd--;
     diag_printf("Max BDs %d, Current DB %d ( @%p @b%p / @e%p )\n",
     qi->rxmax,
     current_bd,
     qi->rnext,
     qi->rbase,
     rxbd);
  }
}

externC void
fman_disp_tx_bd_stats( struct eth_drv_sc *sc , char flag )
{
  int i, current_bd = -1;
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct fman_bd *txbd;

  txbd = qi->tbase;

  diag_printf("TX BDs\n");
  diag_printf("------\n");

  if( flag & FMAN_STAT_ALL_BDS )
  {
    diag_printf("Idx - Ready - Last  -  Len - Buffer\n");
  }

  for (i = 0; i < qi->txnum; i++)
  {
    if( flag & FMAN_STAT_ALL_BDS )
    {
      diag_printf("%3d - %5d - %4d - %4d - 0x%08x\n",
         i,
         ((txbd->ctrl & FMAN_BD_Tx_Ready) ? 1 : 0),
         ((txbd->ctrl & FMAN_BD_Tx_Last) ? 1 : 0),
         txbd->length,
         txbd->buf_lo);
    }
    if(txbd == qi->tnext)
      current_bd = i;
    txbd++;
  }

  if( flag & FMAN_STAT_CURRENT_BD )
  {
     txbd--;
     diag_printf("Current DB %d ( @%p @b%p / @e%p )\n",
     current_bd,
     qi->tnext,
     qi->tbase,
     txbd);
  }
}

externC void
fman_disp_imgp( struct eth_drv_sc *sc )
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;

  diag_printf("IM Global Parameters\n");
  diag_printf("--------------------\n");

  diag_printf("IM Register: %x\n",
                 qi->fman_imgp_ram->imr);
  diag_printf("RX QD @0x%x\n",
                 qi->fman_imgp_ram->rx_qd_ptr);
  diag_printf("TX QD @0x%x\n",
                 qi->fman_imgp_ram->tx_qd_ptr);
  diag_printf("Max Size   : %d\n",
                 qi->fman_imgp_ram->mrblr);
  diag_printf("RX Busy Cnt: %d\n",
                 qi->fman_imgp_ram->rx_qd_bsy_cnt);
  diag_printf("--\n");
  diag_printf("  RX QD Offset In : %d\n",
                 qi->fman_imgp_ram->rx_qd.offset_in);
  diag_printf("  RX QD Offset Out: %d\n",
                 qi->fman_imgp_ram->rx_qd.offset_out);
  diag_printf("--\n");
  diag_printf("  TX QD Offset In : %d\n",
                 qi->fman_imgp_ram->tx_qd.offset_in);
  diag_printf("  TX QD Offset Out: %d\n",
                 qi->fman_imgp_ram->tx_qd.offset_out);
}

#endif // CYGSEM_DEVS_ETH_FREESCALE_FMAN_DEBUG_CODE
