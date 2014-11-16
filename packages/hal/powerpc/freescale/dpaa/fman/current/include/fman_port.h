#ifndef CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_PORT_H
#define CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_PORT_H
//==========================================================================
//
//      fman_port.h
//
//      HAL support for Freescale DPAA Frame Manager
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
// Description:  Freescale DPAA Frame Manager
//
//
//####DESCRIPTIONEND####
//
//==========================================================================


// Some default port configuration
#define FMAN_BMI_MAX_FIFO_SIZE                    0x10000 // 64KB
#define FMAN_BMI_DEFAULT_RX_FIFO_PRI_ELEV         FMAN_BMI_MAX_FIFO_SIZE
#define FMAN_BMI_DEFAULT_RX_FIFO_THR             (FMAN_BMI_MAX_FIFO_SIZE*3/4)

#define FMAN_BMI_DEFAULT_TX_FIFO_LOW_COMFORT_LEV  0x1400  // 5KB
#define FMAN_BMI_DEFAULT_TX_FIFO_MIN_FILL_LEV     0x0

#define FMAN_BMI_DEFAULT_TX_FIFO_DQ_PDEPTH_1G     0
#define FMAN_BMI_DEFAULT_TX_FIFO_DQ_PDEPTH_2_5G   1
#define FMAN_BMI_DEFAULT_TX_FIFO_DQ_PDEPTH_10G    3
#define FMAN_BMI_DEFAULT_TX_FIFO_DQ_PDEPTH_OH     1

struct fman_tx_port_cfg {
  // Static configuration
  cyg_uint32 port;         // Specify TX hardware port number
  cyg_uint32 lport;        // Specify TX logical port number
  cyg_uint32 fifo_mfl;     // Minimum Fill Level
  cyg_uint32 fifo_dpde;    // Dequeue Pipeline Depth
  cyg_uint32 fifo_lcl;     // Low Comfort Level
  // Dynamic configuration
  cyg_uint32 tcfqid;       // Default Confirmation Frame Queue ID.
};

struct fman_rx_port_cfg {
  // Static configuration
  cyg_uint32 port;         // Specify TX hardware port number
  cyg_uint32 lport;        // Specify TX logical port number
  cyg_uint32 fifo_pel;     // Priority Elevation Level
  cyg_uint32 fifo_thr;     // RX FIFO Threshold
  // Dynamic configuration
  cyg_uint32 dfqid;        // Default Frame Queue ID
};

cyg_uint32 fman_bmi_cfg_tx_port( struct fman_tx_port_cfg *, cyg_uint32, cyg_uint32);
cyg_uint32 fman_bmi_cfg_rx_port( struct fman_rx_port_cfg *, cyg_uint32, cyg_uint32);

#endif
