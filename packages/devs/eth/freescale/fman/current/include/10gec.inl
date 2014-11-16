//==========================================================================
//
//      dev/10gec.c
//
//      10-Gigabit Ethernet Media Access Controller
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
// Date:         2013-01-30
// Purpose:
// Description:  Freescale 10GEC
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

// Not implemented

//
// Set 10GEC register pointer
//
static void
gec_eth_set_if(struct eth_drv_sc *sc, cyg_uint32 fman_id, cyg_uint8 mac_id)
{
  struct eth_fman_t *qi = (struct eth_fman_t *) sc->driver_private;
  CYG_ADDRESS fman_base = 0;

  if( (fman_base = CYGNUM_DPAA_FMAN_PORT_BASE_ADDR(fman_id)) == 0)
    return;

  switch (mac_id)
  {
    case 1:
      qi->gec = (volatile struct gec_reg *) ((unsigned char *) \
                CYGARC_IMM_BASE + fman_base + CYGARC_REG_IMM_DPAA_FMAN_10GEC);
      break;
    default:
      qi->gec = NULL;
      break;
  }
}

//
// Initialize the Media Access Controller layer
//
static cyg_uint32
gec_eth_init(struct eth_drv_sc *sc, cyg_uint8 *enaddr, cyg_uint32 speed, 
                                             cyg_uint32 dplx, cyg_uint32 mode){
  feth_printf("GEC MAC layer initialization - Not implemented\n");
  return FMAN_MAC_FAILURE;
}

//
// This function is called to enable the RX/TX
//
static cyg_uint32
gec_eth_start(struct eth_drv_sc *sc, cyg_uint32 ctl)
{
  feth_printf("GEC MAC layer start - Not implemented\n");
  return FMAN_MAC_FAILURE;
}

//
// This function is called to shut down the interface.
//
static cyg_uint32
gec_eth_stop(struct eth_drv_sc *sc, cyg_uint32 ctl)
{
  feth_printf("GEC MAC layer stop - Not implemented\n");
  return FMAN_MAC_FAILURE;
}

FMAN_MAC_ENTRIES(gec_sc,
    gec_eth_init,
    gec_eth_start,
    gec_eth_stop);


