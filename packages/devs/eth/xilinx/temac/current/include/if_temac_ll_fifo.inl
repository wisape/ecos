//==========================================================================
//
//      if_temac_ll_fifo.inl
//
//      Support for Xilinx TEMAC Ethernet controller (LL FIFO interface)
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

#ifndef CYGONCE_DEV_ETH_XILINX_TEMAC_LL_FIFO_INL
#define CYGONCE_DEV_ETH_XILINX_TEMAC_LL_FIFO_INL

//
// This function is called to handle Local Link FIFO interface error
//
static void
temac_eth_ll_fifo_Error( struct eth_drv_sc *sc, cyg_uint32 isr )
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    cyg_uint8 rx_error = 0;
    cyg_uint8 tx_error = 0;

    if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPUR ) {
        temac_err_printf("TEMAC- LL FIFO Receive Packet Underrun Read\n");
        rx_error = 1;
    }
    if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPOR ) {
        temac_err_printf("TEMAC- LL FIFO Receive Packet Overrun\n");
        rx_error = 1;
    }
    if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_RPU ) {
        temac_err_printf("TEMAC- LL FIFO Receive Packet Overrun\n");
        rx_error = 1;
    }
    if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TPO ) {
        temac_err_printf("TEMAC- LL FIFO Transmit Packet Overrun\n");
        tx_error = 1;
    }
    if( isr & CYGHWR_HAL_MB_XPS_LL_FIFO_IxR_TS ) {
        temac_err_printf("TEMAC- LL FIFO Transmit size Error\n");
        tx_error = 1;
    }

    if( rx_error ) {
        CYGHWR_HAL_MB_XPS_LL_FIFO_RESET_RX( qi->llf_base )
    }
    if( tx_error ) {
        CYGHWR_HAL_MB_XPS_LL_FIFO_RESET_TX( qi->llf_base )
    }
}


//
// This function is called to see if another packet can be sent.
// It should return the number of packets which can be handled.
// Zero should be returned if the interface is busy and can not send any more.
//
int
temac_eth_ll_fifo_can_send(struct eth_drv_sc *sc)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;

    if( qi->txkey[qi->txi] == 0 )
      return 1;
    else
      return 0;
}


static void
temac_eth_ll_fifo_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, int sg_len)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    int i = 0;
    cyg_uint32 data = 0;
    cyg_uint32 len = 0, pos = 0;
    cyg_uint8  *bbuf = NULL;
    cyg_uint32 *wbuf = NULL;

    // Best performance case, the receive buffer is a single word aligned buffer,
    // copy directly from the FIFO to IP stack buffer
    if( ( sg_len == 1 ) && ( (sg_list[i].buf & 0x3 ) == 0 ) ) {
        if (sg_list[i].buf != 0)
        {
          wbuf = (cyg_uint32 *) sg_list[i].buf;
          len = sg_list[i].len;
          for(; len > 3; len-=4 ) {
            CYGHWR_HAL_MB_XPS_LL_FIFO_READ_WORD( qi->llf_base , wbuf[pos]);
            pos += 1;
          }
          if( len ) {
            data = CYGHWR_HAL_MB_XPS_LL_FIFO_GET_WORD( qi->llf_base );
            memcpy((void *) &wbuf[pos], &data, len);
          } 
        }
    }
    // Otherwise, go through an intermediate buffer..
    else {
        // Figure out the packet length
        for (i = 0; i < sg_len; i++) {
          if (sg_list[i].buf != 0)
            len += sg_list[i].len;
        }
        wbuf = (cyg_uint32 *) qi->init_rxbufs;
        bbuf = qi->init_rxbufs;
        // Copy from FIFO to intermediate buffer
        for(; len > 3; len-=4 ) {
          CYGHWR_HAL_MB_XPS_LL_FIFO_READ_WORD( qi->llf_base , wbuf[pos]);
          pos += 1;
        }
        if( len ) {
          data = CYGHWR_HAL_MB_XPS_LL_FIFO_GET_WORD( qi->llf_base );
          memcpy((void *) &wbuf[pos], &data, len);
        }
        // Copy from intermediate buffer to scatter/gather IP stack buffer
        for (i = 0; i < sg_len; i++)
        {
          if (sg_list[i].buf != 0)
          {
            memcpy((void *) sg_list[i].buf, bbuf, sg_list[i].len);
            bbuf += sg_list[i].len;
          }
        }
    }
}


//
// This routine is called to send data to the hardware.
void
temac_eth_ll_fifo_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
                int sg_len, int total_len, unsigned long key)
{
    struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
    cyg_uint32 *bp  = (cyg_uint32 *) qi->init_txbufs;
    cyg_uint8  *buf = (cyg_uint8 *)  qi->init_txbufs;
    cyg_uint16 i = 0;

    if( qi->txkey[qi->txi] != 0 ){
      temac_eth_printf("ETH TEMAC - return from send %d\n", qi->txi);
      return;
    }

    temac_eth_printf("ETH TEMAC - transmit pkt, length %d - %d\n", total_len, sg_len);
    qi->txkey[qi->txi] = key;
    qi->txi = (qi->txi+1)%2;

    // Copy the data to a word aligned buffer, this is really inefficient..
    for (i = 0; i < sg_len; i++) {
        memcpy((void *) buf, (void *) sg_list[i].buf, sg_list[i].len);
        buf += sg_list[i].len;
    }
 
    // Finally, push the data in the Local Link FIFO
    bp = (cyg_uint32 *) qi->init_txbufs;
    for (i = 0; i < (total_len / 4); i++) {
        CYGHWR_HAL_MB_XPS_LL_FIFO_WRITE_WORD( qi->llf_base, bp[i] );
    }

    // Write last byte(s)
    if( total_len % 4 ){
        CYGHWR_HAL_MB_XPS_LL_FIFO_WRITE_WORD( qi->llf_base, bp[i] );
    }

    // Write data length in bytes
    CYGHWR_HAL_MB_XPS_LL_FIFO_SET_TX_BYTE_COUNT( qi->llf_base, total_len );
}


//
// This function is called when a packet has been received.  It's job is
// to prepare to unload the packet from the hardware.  Once the length of
// the packet is known, the upper layer of the driver can be told.  When
// the upper layer is ready to unload the packet, the internal function
// 'fec_eth_recv' will be called to actually fetch it from the hardware.
//
static void
temac_eth_ll_fifo_RxEvent(struct eth_drv_sc *sc)
{
  struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
  cyg_int32 pkt_len = 0;

  while( CYGHWR_HAL_MB_XPS_LL_FIFO_GET_RX_WORD_OCCUPANCY( qi->llf_base ) > 0 ){
    pkt_len = CYGHWR_HAL_MB_XPS_LL_FIFO_GET_RX_BYTE_COUNT( qi->llf_base );
    temac_eth_printf("ETH TEMAC - Receive pkt, length %d\n", pkt_len);
    (sc->funs->eth_drv->recv)(sc, pkt_len);
  }
}


//
// This function is called for low level "control" operations
//
void
temac_eth_ll_fifo_TxEvent(struct eth_drv_sc *sc)
{
  struct temac_eth_info *qi = (struct temac_eth_info *) sc->driver_private;
  unsigned long key;

  if ((key = qi->txkey[qi->txo]) != 0)
  {
    qi->txkey[qi->txo] = 0;
    qi->txo = (qi->txo+1)%2;
    (sc->funs->eth_drv->tx_done)(sc, key, 0);
  }
}

#endif // CYGONCE_DEV_ETH_XILINX_TEMAC_LL_FIFO_INL
