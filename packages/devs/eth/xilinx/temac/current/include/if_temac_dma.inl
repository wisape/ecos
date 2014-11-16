//==========================================================================
//
//      if_temac_dma.inl
//
//      Support for Xilinx TEMAC Ethernet controller (DMA interface)
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

#ifndef CYGONCE_DEV_ETH_XILINX_TEMAC_DMA_INL
#define CYGONCE_DEV_ETH_XILINX_TEMAC_DMA_INL


//
// This function is called to handle DMA error
//
static void
temac_eth_dma_Error( struct eth_drv_sc *sc, cyg_uint32 isr )
{

}


//
// This function is called to see if another packet can be sent.
// It should return the number of packets which can be handled.
// Zero should be returned if the interface is busy and can not send any more.
//
int
temac_eth_dma_can_send(struct eth_drv_sc *sc)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;

    return (qi->txactive < CYGNUM_DEVS_ETH_XILINX_TEMAC_ETH0_TxNUM);
}


static void
temac_eth_dma_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len)
{
   // TODO
}


//
// This routine is called to send data to the hardware.
 void
temac_eth_dma_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
                int sg_len, int total_len, unsigned long key)
{
   // TODO
}


//
// This function is called when a packet has been received.  It's job is
// to prepare to unload the packet from the hardware.  Once the length of
// the packet is known, the upper layer of the driver can be told.  When
// the upper layer is ready to unload the packet, the internal function
// 'fec_eth_recv' will be called to actually fetch it from the hardware.
//
static void
temac_eth_dma_RxEvent(struct eth_drv_sc *sc)
{
   // TODO
}


//
// This function is called for low level "control" operations
//
 void
temac_eth_dma_TxEvent(struct eth_drv_sc *sc)
{
   // TODO
}

#endif // CYGONCE_DEV_ETH_XILINX_TEMAC_DMA_INL
