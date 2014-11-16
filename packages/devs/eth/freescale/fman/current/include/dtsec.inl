//==========================================================================
//
//      dev/dtsec.inl
//
//      Ethernet device driver for the DPAA
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
// Date:         2013-02-05
// Purpose:      
// Description:  Hardware driver for Freescale DTSEC
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================


//
// Set DTSEC register pointer
//
static void
dtsec_eth_set_if(struct eth_drv_sc *sc, cyg_uint32 fman_id, cyg_uint8 mac_id)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  CYG_ADDRESS fman_base = 0;

  if( (fman_base = CYGNUM_DPAA_FMAN_PORT_BASE_ADDR(fman_id)) == 0)
    return;

  switch (mac_id)
  {
    case 1:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC1);
      break;
    case 2:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC2);
      break;
    case 3:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC3);
      break;
    case 4:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC4);
      break;
    case 5:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC5);
      break;
    case 6:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC6);
      break;
    case 7:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC7);
      break;
    case 8:
      qi->dtsec = (volatile struct dtsec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_DTSEC8);
      break;
    default:
      qi->dtsec = NULL;
      break;
  }
}

//
// Initialize the Media Access Controller layer
//
static cyg_uint32
dtsec_eth_init(struct eth_drv_sc *sc, cyg_uint8 *enaddr, cyg_uint32 speed, 
                                              cyg_uint32 dplx, cyg_uint32 mode){
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct dtsec_reg *dtsec = (volatile struct dtsec_reg *) qi->dtsec;
  cyg_uint32 ecntrl = (ECNTRL_STEN | ECNTRL_AUTOZ | ECNTRL_CLRCNT);

  feth_printf("DTSEC MAC layer initialization\n");

  // Reset MAC for at least 3 TX clock cycle
  dtsec->maccfg1 = MACCFG1_SOFT_RESET;
  HAL_DELAY_US(1000);

  // Clear soft reset
  dtsec->maccfg1 = 0; // (MACCFG1_RXFLOW| MACCFG1_TXFLOW);

  // Stop RX/TX
  dtsec->rctrl = RCTRL_GSR;
  dtsec->tctrl = TCTRL_GTS;
  HAL_DELAY_US(1000);

  //
  // Configure MAC layer

  // Group address hash
  dtsec->gaddr[0] = 0;
  dtsec->gaddr[1] = 0;
  dtsec->gaddr[2] = 0;
  dtsec->gaddr[3] = 0;
  dtsec->gaddr[4] = 0;
  dtsec->gaddr[5] = 0;
  dtsec->gaddr[6] = 0;
  dtsec->gaddr[7] = 0;

  dtsec->igaddr[0] = 0;
  dtsec->igaddr[1] = 0;
  dtsec->igaddr[2] = 0;
  dtsec->igaddr[3] = 0;
  dtsec->igaddr[4] = 0;
  dtsec->igaddr[5] = 0;
  dtsec->igaddr[6] = 0;
  dtsec->igaddr[7] = 0;

  // Device physical address
  dtsec->macstnaddr1 = \
              (cyg_uint32) ((cyg_uint32) enaddr[5] << 24 | \
                            (cyg_uint32) enaddr[4] << 16 | \
                            (cyg_uint32) enaddr[3] <<  8 | \
                            (cyg_uint32) enaddr[2]);
  dtsec->macstnaddr2 = \
              (cyg_uint32) ((cyg_uint32) enaddr[1] << 24 | \
                            (cyg_uint32) enaddr[0] << 16);

  // Max frame size
  dtsec->maxfrm = IEEE_8023_MAX_FRAME;

  // Init MIB
  memset((void *) &(dtsec->rmon), 0, sizeof(struct rmon_dtsec));
  dtsec->rmon.cam1 = 0xFFFFFFFF;
  dtsec->rmon.cam2 = 0xFFFFFFFF;

  // Clear interrupts
  dtsec->imask  = IMASK_CLEAR_ALL;
  dtsec->ievent = IEVENT_CLEAR_ALL;

  if( (dplx == 0) && (speed == 1000) )
    diag_printf("DTEC MAC layer do not support half duplex in 1Gbps mode\n");

  if( mode == MODE_RGMII )
    ecntrl |= ( ECNTRL_GMIIM | ECNTRL_RPM );

  if( mode == MODE_RMII )
    ecntrl |= ( ECNTRL_RMM );

  if( (mode == MODE_SGMII) || (mode == MODE_QSGMII) )
    ecntrl |= ( ECNTRL_SGMIIM );

  if( (mode == MODE_TBI) || (mode == MODE_RTBI) ||
      (mode == MODE_SGMII) || (mode == MODE_QSGMII) )
    ecntrl |= ECNTRL_TBIM;

  if( (speed == 100) && ((mode == MODE_RMII) ||
      (mode == MODE_RGMII) || (mode == MODE_SGMII)) )
    ecntrl |= ECNTRL_R100M;

   feth_printf("DTSEC MAC layer ECNTRL: 0x%08x\n", ecntrl);

   dtsec->ecntrl  = ecntrl;

   dtsec->maccfg2 =  0x00007000  | 
                   ((speed==1000) ? MACCFG2_IF_MODE_BYTE : MACCFG2_IF_MODE_NIBBLE) |
                     MACCFG2_PAD_CRC | 
                   ((dplx==1) ? MACCFG2_FULL_DUPLEX : 0x0);

  return FMAN_MAC_SUCCESS;
}

//
// This function is called to enable the RX/TX
//
static cyg_uint32
dtsec_eth_start(struct eth_drv_sc *sc, cyg_uint32 ctl)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct dtsec_reg *dtsec = (volatile struct dtsec_reg *) qi->dtsec;

  feth_printf("DTSEC MAC layer start - %d\n", ctl);

  // Start RX/TX
  switch(ctl){
    case MAC_CTL_ALL:
    case MAC_CTL_RX:
      dtsec->rctrl &= ~RCTRL_GSR;
      dtsec->maccfg1 |= (MACCFG1_RXEN);
      if(ctl==MAC_CTL_RX)
        break;
      // intended fall-through for MAC_CTL_ALL
    case MAC_CTL_TX:
      dtsec->tctrl &= ~TCTRL_GTS;
      dtsec->maccfg1 |= (MACCFG1_TXEN);
      break;
    default:
      return FMAN_MAC_FAILURE;
  }

  return FMAN_MAC_SUCCESS;
}

//
// This function is called to shut down the interface.
//
static cyg_uint32
dtsec_eth_stop(struct eth_drv_sc *sc, cyg_uint32 ctl)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  volatile struct dtsec_reg *dtsec = (volatile struct dtsec_reg *) qi->dtsec;

  feth_printf("DTSEC MAC layer stop - %d\n", ctl);

  // Stop RX/TX
  switch(ctl){
    case MAC_CTL_ALL:
    case MAC_CTL_RX:
      dtsec->rctrl |= RCTRL_GSR;
      dtsec->maccfg1 &= ~(MACCFG1_RXEN);
      if(ctl==MAC_CTL_RX)
        break;
      // intended fall-through for MAC_CTL_ALL
    case MAC_CTL_TX:
      dtsec->tctrl |= TCTRL_GTS;
      dtsec->maccfg1 &= ~(MACCFG1_TXEN);
      break;
    default:
      return FMAN_MAC_FAILURE;
  }

  return FMAN_MAC_SUCCESS;
}

FMAN_MAC_ENTRIES(dtsec_sc,
    dtsec_eth_init,
    dtsec_eth_start,
    dtsec_eth_stop);

//
// Display DTSEC RMON statistics
//
//
#ifdef CYGSEM_DEVS_ETH_FREESCALE_FMAN_DEBUG_CODE

static
void dtsec_disp_stats(struct eth_drv_sc *sc , char flag )
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;

  diag_printf("Ethernet Stats from DTSEC @ 0x%p\n", qi->dtsec);

  if( flag & FMAN_STAT_MAC_RMON )
  {
    diag_printf("%6s %10d\n\r", \
    "tr64", qi->dtsec->rmon.tr64);
    diag_printf("%6s %10d\n\r", \
    "tr127", qi->dtsec->rmon.tr127);
    diag_printf("%6s %10d\n\r", \
    "tr255", qi->dtsec->rmon.tr255);
    diag_printf("%6s %10d\n\r", \
    "tr511", qi->dtsec->rmon.tr511);
    diag_printf("%6s %10d\n\r", \
    "tr1k", qi->dtsec->rmon.tr1k);
    diag_printf("%6s %10d\n\r", \
    "trmax", qi->dtsec->rmon.trmax);
    diag_printf("%6s %10d\n\r", \
    "trmgv", qi->dtsec->rmon.trmgv);
    diag_printf("%6s %10d\n\r", \
    "rbyt", qi->dtsec->rmon.rbyt);
    diag_printf("%6s %10d\n\r", \
    "rpkt", qi->dtsec->rmon.rpkt);
    diag_printf("%6s %10d\n\r", \
    "rfcs", qi->dtsec->rmon.rfcs);
    diag_printf("%6s %10d\n\r", \
    "rmca", qi->dtsec->rmon.rmca);
    diag_printf("%6s %10d\n\r", \
    "rbca", qi->dtsec->rmon.rbca);
    diag_printf("%6s %10d\n", \
    "rxcf", qi->dtsec->rmon.rxcf);
    diag_printf("%6s %10d\n\r", \
    "rxpf", qi->dtsec->rmon.rxpf);
    diag_printf("%6s %10d\n\r", \
    "rxuo", qi->dtsec->rmon.rxuo);
    diag_printf("%6s %10d\n\r", \
    "raln", qi->dtsec->rmon.raln);
    diag_printf("%6s %10d\n\r", \
    "rflr", qi->dtsec->rmon.rflr);
    diag_printf("%6s %10d\n\r", \
    "rcde", qi->dtsec->rmon.rcde);
    diag_printf("%6s %10d\n\r", \
    "rcse", qi->dtsec->rmon.rcse);
    diag_printf("%6s %10d\n\r", \
    "rund", qi->dtsec->rmon.rund);
    diag_printf("%6s %10d\n\r", \
    "rovr", qi->dtsec->rmon.rovr);
    diag_printf("%6s %10d\n", \
    "rfrg", qi->dtsec->rmon.rfrg);
    diag_printf("%6s %10d\n\r", \
    "rjbr", qi->dtsec->rmon.rjbr);
    diag_printf("%6s %10d\n\r", \
    "rdrp", qi->dtsec->rmon.rdrp);
    diag_printf("%6s %10d\n\r", \
    "tbyt", qi->dtsec->rmon.tbyt);
    diag_printf("%6s %10d\n\r", \
    "tpkt", qi->dtsec->rmon.tpkt);
    diag_printf("%6s %10d\n\r", \
    "tmca", qi->dtsec->rmon.tmca);
    diag_printf("%6s %10d\n\r", \
    "tbca", qi->dtsec->rmon.tbca);
    diag_printf("%6s %10d\n\r", \
    "txpf", qi->dtsec->rmon.txpf);
    diag_printf("%6s %10d\n\r", \
    "tdfr", qi->dtsec->rmon.tdfr);
    diag_printf("%6s %10d\n\r", \
    "tedf", qi->dtsec->rmon.tedf);
    diag_printf("%6s %10d\n\r", \
    "tscl", qi->dtsec->rmon.tscl);
    diag_printf("%6s %10d\n\r", \
    "tmcl", qi->dtsec->rmon.tmcl);
    diag_printf("%6s %10d\n\r", \
    "tlcl", qi->dtsec->rmon.tlcl);
    diag_printf("%6s %10d\n\r", \
    "txcl", qi->dtsec->rmon.txcl);
    diag_printf("%6s %10d\n\r", \
    "tncl", qi->dtsec->rmon.tncl);
    diag_printf("%6s %10d\n\r", \
    "tdrp", qi->dtsec->rmon.tdrp);
    diag_printf("%6s %10d\n\r", \
    "tjbr", qi->dtsec->rmon.tjbr);
    diag_printf("%6s %10d\n\r", \
    "tfcs", qi->dtsec->rmon.tfcs);
    diag_printf("%6s %10d\n\r", \
    "txcf", qi->dtsec->rmon.txcf);
    diag_printf("%6s %10d\n\r", \
    "tovr", qi->dtsec->rmon.tovr);
    diag_printf("%6s %10d\n\r", \
    "tund", qi->dtsec->rmon.tund);
  }
}

#endif // CYGSEM_DEVS_ETH_FREESCALE_FMAN_DEBUG_CODE