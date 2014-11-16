//==========================================================================
//
//      fman_port.c
//
//      HAL support for Freescale Frame Manager
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
// Description:  Freescale Frame Manager device driver
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/hal_powerpc_freescale_dpaa_fman.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/infra/cyg_ass.h>          // CYG_FAIL

#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>

#include "cyg/io/fman.h"
#include "cyg/io/fman_port.h"

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_CHATTER
#define fman_printf(args...)   diag_printf(args)
#else
#define fman_printf(args...)   /* NOOP */
#endif

//
// This function is used to Reset / Start / Stop the statistics
// and performances counters
//
void
fman_bmi_port_resetcnt(volatile struct fman_bmi_port *p, fman_port_t ptype,
                                fman_cnt_action_t action)
{
  switch(ptype){
  case FMAN_TX_PORT:
    if( action == FMAN_CNT_RST) {
      p->u.tx.fmbm_tfrc   = 0;
      p->u.tx.fmbm_tfdc   = 0;
      p->u.tx.fmbm_tfledc = 0;
      p->u.tx.fmbm_tfufdc = 0;
      p->u.tx.fmbm_tbdc   = 0;
      p->u.tx.fmbm_tpcp   = 0;
      p->u.tx.fmbm_tccn   = 0;
      p->u.tx.fmbm_ttuc   = 0;
      p->u.tx.fmbm_ttcquc = 0;
      p->u.tx.fmbm_tduc   = 0;
      p->u.tx.fmbm_tfuc   = 0;
    }
    else if( action == FMAN_CNT_EN ) {
      p->u.tx.fmbm_tstc |= FMAN_FMBM_TSTC_EN;
      p->u.tx.fmbm_tpc  |= FMAN_FMBM_TPC_EN;
    }
    else if( action == FMAN_CNT_DIS ) {
      p->u.tx.fmbm_tstc &= ~FMAN_FMBM_TSTC_EN;
      p->u.tx.fmbm_tpc  &= ~FMAN_FMBM_TPC_EN;
    }
    break;
  case FMAN_RX_PORT:
    if( action == FMAN_CNT_RST) {
      p->u.rx.fmbm_rfrc   = 0;
      p->u.rx.fmbm_rbfc   = 0;
      p->u.rx.fmbm_rlfc   = 0;
      p->u.rx.fmbm_rffc   = 0;
      p->u.rx.fmbm_rfdc   = 0;
      p->u.rx.fmbm_rfldec = 0;
      p->u.rx.fmbm_rodc   = 0;
      p->u.rx.fmbm_rpcp   = 0;
      p->u.rx.fmbm_rccn   = 0;
      p->u.rx.fmbm_rtuc   = 0;
      p->u.rx.fmbm_rrquc  = 0;
      p->u.rx.fmbm_rduc   = 0;
      p->u.rx.fmbm_rfuc   = 0;
      p->u.rx.fmbm_rpac   = 0;
    }
    else if( action == FMAN_CNT_EN ) {
      p->u.rx.fmbm_rstc |= FMAN_FMBM_RSTC_EN;
      p->u.rx.fmbm_rpc  |= FMAN_FMBM_RPC_EN;
    }
    else if( action == FMAN_CNT_DIS ) {
      p->u.rx.fmbm_rstc &= ~FMAN_FMBM_RSTC_EN;
      p->u.rx.fmbm_rpc  &= ~FMAN_FMBM_RPC_EN;
    }
    break;
  case FMAN_OFFLINE_PORT:
    if( action == FMAN_CNT_RST) {
      p->u.oh.fmbm_ofrc   = 0;
      p->u.oh.fmbm_ofdc   = 0;
      p->u.oh.fmbm_ofledc = 0;
      p->u.oh.fmbm_ofufdc = 0;
      p->u.oh.fmbm_offc   = 0;
      p->u.oh.fmbm_ofwdc  = 0;
      p->u.oh.fmbm_ofldec = 0;
      p->u.oh.fmbm_obdc   = 0;
      p->u.oh.fmbm_opcp   = 0;
      p->u.oh.fmbm_occn   = 0;
      p->u.oh.fmbm_otuc   = 0;
      p->u.oh.fmbm_oduc   = 0;
      p->u.oh.fmbm_ofuc   = 0;
    }
    else if( action == FMAN_CNT_EN ) {
      p->u.oh.fmbm_ostc |= FMAN_FMBM_OSTC_EN;
      p->u.oh.fmbm_opc  |= FMAN_FMBM_OPC_EN;
    }
    else if( action == FMAN_CNT_DIS ) {
      p->u.oh.fmbm_ostc &= ~FMAN_FMBM_OSTC_EN;
      p->u.oh.fmbm_opc  &= ~FMAN_FMBM_OPC_EN;
    }
    break;
  default:
    return;
  }
  HAL_MEMORY_BARRIER();
}

//
// Disable Off-line / TX / RX port (BMI)
//
externC cyg_uint32
fman_bmi_port_off(volatile struct fman_bmi_port *p, fman_port_t ptype)
{
  cyg_uint32 timeout = 1000;
  cyg_uint32 ret = FMAN_ERR_WRONG_TYPE;

  switch(ptype){
  case FMAN_TX_PORT:
    p->u.tx.fmbm_tcfg &= ~FMAN_FMBM_TCFG_EN;
    HAL_MEMORY_BARRIER();
    while( (p->u.tx.fmbm_tst & FMAN_FMBM_TST_BSY) && --timeout )
      hal_delay_us(1);
    ret = (p->u.tx.fmbm_tst & FMAN_FMBM_TST_BSY) ? FMAN_ERR_PORT_BUSY : FMAN_OK;
    break;
  case FMAN_RX_PORT:
    p->u.rx.fmbm_rcfg &= ~FMAN_FMBM_RCFG_EN;
    HAL_MEMORY_BARRIER();
    while( (p->u.rx.fmbm_rst & FMAN_FMBM_RST_BSY) && --timeout )
      hal_delay_us(1);
    ret = (p->u.rx.fmbm_rst & FMAN_FMBM_RST_BSY) ? FMAN_ERR_PORT_BUSY : FMAN_OK;
    break;
  case FMAN_OFFLINE_PORT:
    p->u.oh.fmbm_ocfg &= ~FMAN_FMBM_OCFG_EN;
    HAL_MEMORY_BARRIER();
    while( (p->u.oh.fmbm_ost & FMAN_FMBM_OST_BSY) && --timeout )
      hal_delay_us(1);
    ret = (p->u.oh.fmbm_ost & FMAN_FMBM_OST_BSY) ? FMAN_ERR_PORT_BUSY : FMAN_OK;
    break;
  default:
    ret = FMAN_ERR_WRONG_TYPE;
    break;
  }

  if( ret == FMAN_ERR_PORT_BUSY )
    diag_printf("FMAN BMI stop: port is busy!\n");

  return ret;
}

//
// Enable Off-line / TX / RX port (BMI)
//
externC cyg_uint32
fman_bmi_port_on(volatile struct fman_bmi_port *p, fman_port_t ptype)
{
  cyg_uint32 ret = FMAN_OK;

  switch(ptype){
  case FMAN_TX_PORT:
    if( p->u.tx.fmbm_tst & FMAN_FMBM_TST_BSY ) {
      if( (p->u.tx.fmbm_tcfg & FMAN_FMBM_TCFG_EN) == 0 ) {
        ret = FMAN_ERR_PORT_BUSY;
        break;
      }
    }
    p->u.tx.fmbm_tcfg |= FMAN_FMBM_TCFG_EN;
    HAL_MEMORY_BARRIER();
    break;
  case FMAN_RX_PORT:
    if( p->u.rx.fmbm_rst & FMAN_FMBM_RST_BSY ) {
      if( (p->u.rx.fmbm_rcfg & FMAN_FMBM_RCFG_EN) == 0 ) {
        ret = FMAN_ERR_PORT_BUSY;
        break;
      }
    }
    p->u.rx.fmbm_rcfg |= FMAN_FMBM_RCFG_EN;
    HAL_MEMORY_BARRIER();
    break;
  case FMAN_OFFLINE_PORT:
    if( p->u.oh.fmbm_ost & FMAN_FMBM_OST_BSY ) {
      if( (p->u.oh.fmbm_ocfg & FMAN_FMBM_OCFG_EN) == 0 ) {
        ret = FMAN_ERR_PORT_BUSY;
        break;
      }
    }
    p->u.oh.fmbm_ocfg |= FMAN_FMBM_OCFG_EN;
    HAL_MEMORY_BARRIER();
    break;
  default:
    ret = FMAN_ERR_WRONG_TYPE;
    break;
  }

  if( ret == FMAN_ERR_PORT_BUSY )
    diag_printf("FMAN BMI Start: port is busy!\n");

  return ret;
}

//
// Disable Off-line / TX / RX port (QMI)
//
externC cyg_uint32
fman_qmi_port_off(volatile struct fman_qmi_port *p)
{
  cyg_uint32 timeout = 1000;
  cyg_uint32 ret = FMAN_OK;

  p->u.common.fmqm_pnc &= ~FMAN_FMQM_PnC_EN;
  HAL_MEMORY_BARRIER();
  while( (p->u.common.fmqm_pns & FMAN_FMQM_PnS_PBSY_DF) && --timeout )
    hal_delay_us(1);

  if( p->u.common.fmqm_pns & FMAN_FMQM_PnS_PBSY_DF )
    ret = FMAN_ERR_PORT_BUSY;

  return ret;
}

//
// Enable Off-line / TX / RX port (QMI)
//
externC cyg_uint32
fman_qmi_port_on(volatile struct fman_qmi_port *p)
{
  p->u.common.fmqm_pnc |= FMAN_FMQM_PnC_EN;
  HAL_MEMORY_BARRIER();

  return FMAN_OK;
}

//
// User/driver API to configure TX port
//
externC cyg_uint32
fman_bmi_cfg_tx_port( struct fman_tx_port_cfg *tx, cyg_uint32 fman_num,
                                                              cyg_uint32 im){
  struct fman_t *fman;
  cyg_uint8 p = (cyg_uint8) tx->port;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return FMAN_ERR_INIT_FAILED;

  //
  // Make sure the port is disable when configuring it
  if( fman->port->p[p].bmi.u.tx.fmbm_tcfg & FMAN_FMBM_TCFG_EN ){
    fman_printf("FMAN%d - TX port: %d error, can't configure ", fman_num, p);
    fman_printf("port that is already enable\n");
    return FMAN_ERR_PORT_BUSY;
  }

  // Minimum Fill Level:
  // If the amount of valid data on transmit buffer is smaller than the
  // amount specified in MFL, the BMI signals the main FM’s DMA to elevate
  // the FMan priority on the SoC main bus (CoreNet in most devices)
  // FIFO Low Comfort Level.
  // Set a byte count limit for transmit queue size. The limit should be
  // small enough to prevent high latencies of high priority frames, while
  // on the other hand it should be big enough to allow reasonable transmit
  // data streaming.
  fman->port->p[p].bmi.u.tx.fmbm_tfp =
       FMAN_FMBM_TFP_FLCL((tx->fifo_lcl/256-1)) |
       FMAN_FMBM_TFP_SET_DPDE(tx->fifo_dpde)    |
       FMAN_FMBM_TFP_SET_MFL(tx->fifo_mfl);

  // Set FMAN as next module
  fman->port->p[p].bmi.u.tx.fmbm_tfdne = FMAN_NIA_SET_ENG(FMAN_NIA_ENG_FMANC) |
       FMAN_NIA_FMANC_AC_IND_MODE_TX;
  fman->port->p[p].bmi.u.tx.fmbm_tfene = FMAN_NIA_SET_ENG(FMAN_NIA_ENG_FMANC) |
       FMAN_NIA_FMANC_AC_IND_MODE_TX;

  // Set attributes
  fman->port->p[p].bmi.u.tx.fmbm_tfca = 0x00040000;

  // Enable statistics counters
  // Note that statistics counters are not affected by the assertion of
  // soft reset, but they are affected by the assertion of hard reset
  fman_bmi_port_resetcnt(&fman->port->p[p].bmi, FMAN_TX_PORT, FMAN_CNT_DIS);
  fman_bmi_port_resetcnt(&fman->port->p[p].bmi, FMAN_TX_PORT, FMAN_CNT_RST);
  fman_bmi_port_resetcnt(&fman->port->p[p].bmi, FMAN_TX_PORT, FMAN_CNT_EN);

  // Set TX Confirmation Frame Queue ID
  if( tx->tcfqid )
    fman->port->p[p].bmi.u.tx.fmbm_tcfqid = FMAN_FMBM_TCFQID_TCFQID(tx->tcfqid);

  // Set IM bit
  if( im )
    fman->port->p[p].bmi.u.tx.fmbm_tcfg |= FMAN_FMBM_TCFG_IM;
  else
    fman->port->p[p].bmi.u.tx.fmbm_tcfg &= ~FMAN_FMBM_TCFG_IM;

  fman_printf("FMAN%d - TX port: %d configuration completed\n", fman_num, p);

  return FMAN_OK;
}

//
// User/driver API to configure RX port
//
externC cyg_uint32
fman_bmi_cfg_rx_port( struct fman_rx_port_cfg *rx, cyg_uint32 fman_num,
                                                              cyg_uint32 im){
  struct fman_t *fman;
  cyg_uint8 p = (cyg_uint8) rx->port;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return FMAN_ERR_INIT_FAILED;

  //
  // Make sure the port is disable when configuring it
  if( fman->port->p[p].bmi.u.rx.fmbm_rcfg & FMAN_FMBM_RCFG_EN ){
    fman_printf("FMAN%d - RX port: %d error, can't configure ", fman_num, p);
    fman_printf("port that is already enable\n");
    return FMAN_ERR_PORT_BUSY;
  }

  // Priority Elevation Level:
  // If the total number of buffers which are currently in use and
  // associated with the specific RX port exceed the amount specified
  // in PEL, the BMI signals the DMA module to elevate its priority.
  // FIFO threshold:
  // Set the threshold of the FIFO fill level. If the total number
  // of buffers which are currently in use and associated with the
  // specific RX port exceed this threshold, the BMI signals the MAC
  // to send a pause frame over the link. Pause time is programmed
  // in the MAC registers.
  fman->port->p[p].bmi.u.rx.fmbm_rfp =
       FMAN_FMBM_RFP_SET_PEL((rx->fifo_pel/256-1)) |
       FMAN_FMBM_RFP_FTH((rx->fifo_pel/256-1));

  // Set FMAN as next module
  fman->port->p[p].bmi.u.rx.fmbm_rfne  = FMAN_NIA_SET_ENG(FMAN_NIA_ENG_FMANC) |
       FMAN_NIA_FMANC_AC_IND_MODE_RX;

  // Set attributes
  fman->port->p[p].bmi.u.rx.fmbm_rfca = 0x003C0000;

  // Frame Offset = 0
  fman->port->p[p].bmi.u.rx.fmbm_rim = 0;

  // Enable statistics counters
  // Note that statistics counters are not affected by the assertion of
  // soft reset, but they are affected by the assertion of hard reset
  fman_bmi_port_resetcnt(&fman->port->p[p].bmi, FMAN_RX_PORT, FMAN_CNT_DIS);
  fman_bmi_port_resetcnt(&fman->port->p[p].bmi, FMAN_RX_PORT, FMAN_CNT_RST);
  fman_bmi_port_resetcnt(&fman->port->p[p].bmi, FMAN_RX_PORT, FMAN_CNT_EN);

  // Set Default Frame Queue ID
  if( rx->dfqid )
    fman->port->p[p].bmi.u.rx.fmbm_rfqid = FMAN_FMBM_RFQID_DFQID(rx->dfqid);

  // Set IM bit
  if( im )
    fman->port->p[p].bmi.u.rx.fmbm_rcfg |= FMAN_FMBM_RCFG_IM;
  else
    fman->port->p[p].bmi.u.rx.fmbm_rcfg &= ~FMAN_FMBM_RCFG_IM;

  fman_printf("FMAN%d - RX port: %d configuration completed\n", fman_num, p);

  return FMAN_OK;
}

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_DEBUG_CODE

externC void
fman_disp_tx_port( cyg_uint8 fman_num, cyg_uint8 p )
{
  struct fman_t *fman;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return;

  diag_printf("FMAN%d - TX port %d @%p\n",
                 fman_num, p, &fman->port->p[p]);
  diag_printf("----------------------------\n");

  diag_printf("FMBM_TCFG   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tcfg);
  diag_printf("FMBM_TFP    : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfp);
  diag_printf("FMBM_TST    : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tst);
  diag_printf("FMBM_TFCA   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfca);
  diag_printf("FMBM_TFDNE  : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfdne);
  diag_printf("FMBM_TFENE  : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfene);
  diag_printf("FMBM_TCFQID : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tcfqid);
  diag_printf("--\n");
  diag_printf("FMBM_TFRC   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfrc);
  diag_printf("FMBM_TFDC   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfdc);
  diag_printf("FMBM_TFLEDC : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfledc);
  diag_printf("FMBM_TFUFDC : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfufdc);
  diag_printf("FMBM_TBDC   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tbdc);
  diag_printf("FMBM_TPCP   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tpcp);
  diag_printf("FMBM_TCCN   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tccn);
  diag_printf("FMBM_TTUC   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_ttuc);
  diag_printf("FMBM_TTCQUC : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_ttcquc);
  diag_printf("FMBM_TDUC   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tduc);
  diag_printf("FMBM_TFUC   : %08x\n",
                 fman->port->p[p].bmi.u.tx.fmbm_tfuc);
  return;
}

externC void
fman_disp_rx_port( cyg_uint8 fman_num, cyg_uint8 p )
{
  struct fman_t *fman;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return;

  diag_printf("FMAN%d - RX port %d @%p\n",
                 fman_num, p, &fman->port->p[p]);
  diag_printf("----------------------------\n");

  diag_printf("FMBM_RCFG   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rcfg);
  diag_printf("FMBM_RFP    : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfp);
  diag_printf("FMBM_RST    : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rst);
  diag_printf("FMBM_RFNE   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfne);
  diag_printf("FMBM_RFCA   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfca);
  diag_printf("FMBM_RIM    : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rim);
  diag_printf("FMBM_RFQID  : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfqid);
  diag_printf("--\n");
  diag_printf("FMBM_RFRC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfrc);
  diag_printf("FMBM_RBFC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rbfc);
  diag_printf("FMBM_RLFC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rlfc);
  diag_printf("FMBM_RFFC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rffc);
  diag_printf("FMBM_RFDC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfdc);
  diag_printf("FMBM_RFLDEC : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfldec);
  diag_printf("FMBM_RODC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rodc);
  diag_printf("FMBM_RPCP   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rpcp);
  diag_printf("FMBM_RCCN   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rccn);
  diag_printf("FMBM_RTUC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rtuc);
  diag_printf("FMBM_RRQUC  : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rrquc);
  diag_printf("FMBM_RDUC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rduc);
  diag_printf("FMBM_RFUC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rfuc);
  diag_printf("FMBM_RPAC   : %08x\n",
                 fman->port->p[p].bmi.u.rx.fmbm_rpac);
  return;
}

#endif // CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_DEBUG_CODE
