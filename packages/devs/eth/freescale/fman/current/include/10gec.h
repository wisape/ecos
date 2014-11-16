#ifndef CYGONCE_DEVS_ETH_FREESCALE_10GEC_H
#define CYGONCE_DEVS_ETH_FREESCALE_10GEC_H
//==========================================================================
//
//      dev/10gec.h
//
//      Freescale 10-Gigabit Ethernet Media Access Controller
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
// 2008, 2009 Free Software Foundation, Inc.
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
// Description:  Hardware driver for Freescale 10GEC
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

struct rmon_10gec
{
    cyg_uint32 tfrm_u;
    cyg_uint32 tfrm_l;
    cyg_uint32 rfrm_u;
    cyg_uint32 rfrm_l;
    cyg_uint32 rfcs_u;
    cyg_uint32 rfcs_l;
    cyg_uint32 raln_u;
    cyg_uint32 raln_l;
    cyg_uint32 txpf_u;
    cyg_uint32 txpf_l;
    cyg_uint32 rxpf_u;
    cyg_uint32 rxpf_l;
    cyg_uint32 rlong_u;
    cyg_uint32 rlong_l;
    cyg_uint32 rflr_u;
    cyg_uint32 rflr_l;
    cyg_uint32 tvlan_u;
    cyg_uint32 tvlan_l;
    cyg_uint32 rvlan_u;
    cyg_uint32 rvlan_l;
    cyg_uint32 toct_u;
    cyg_uint32 toct_l;
    cyg_uint32 roct_u;
    cyg_uint32 roct_l;
    cyg_uint32 ruca_u;
    cyg_uint32 ruca_l;
    cyg_uint32 rmca_u;
    cyg_uint32 rmca_l;
    cyg_uint32 rbca_u;
    cyg_uint32 rbca_l;
    cyg_uint32 terr_u;
    cyg_uint32 terr_l;
    cyg_uint32 tuca_u;
    cyg_uint32 tuca_l;
    cyg_uint32 tmca_u;
    cyg_uint32 tmca_l;
    cyg_uint32 tbca_u;
    cyg_uint32 tbca_l;
    cyg_uint32 rdrp_u;
    cyg_uint32 rdrp_l;
    cyg_uint32 reoct_u;
    cyg_uint32 reoct_l;
    cyg_uint32 rpkt_u;
    cyg_uint32 rpkt_l;
    cyg_uint32 trund_u;
    cyg_uint32 trund_l;
    cyg_uint32 r64_u;
    cyg_uint32 r64_l;
    cyg_uint32 r127_u;
    cyg_uint32 r127_l;
    cyg_uint32 r255_u;
    cyg_uint32 r255_l;
    cyg_uint32 r511_u;
    cyg_uint32 r511_l;
    cyg_uint32 r1023_u;
    cyg_uint32 r1023_l;
    cyg_uint32 r1518_u;
    cyg_uint32 r1518_l;
    cyg_uint32 r1519_u;
    cyg_uint32 r1519_l;
    cyg_uint32 trovr_u;
    cyg_uint32 trovr_l;
    cyg_uint32 trjbr_u;
    cyg_uint32 trjbr_l;
    cyg_uint32 trfrg_u;
    cyg_uint32 trfrg_l;
    cyg_uint32 rerr_u;
    cyg_uint32 rerr_l;
};

struct gec_reg {
    cyg_uint32 id;
    cyg_uint32 _reserved1;
    cyg_uint32 command_config;
    cyg_uint32 mac_addr_0;
    cyg_uint32 mac_addr_1;
    cyg_uint32 maxfrm;
    cyg_uint32 pause_quant;
    cyg_uint32 _reserved2[0x04];
    cyg_uint32 hashtable_ctrl;
    cyg_uint32 _reserved3[0x04];
    cyg_uint32 status;
    cyg_uint32 tx_ipg_length;
    cyg_uint32 mac_addr_2;
    cyg_uint32 mac_addr_3;
    cyg_uint32 _reserved4[0x04];
    cyg_uint32 imask;
    cyg_uint32 ievent;
    cyg_uint32 _reserved5[0x06];
    struct rmon_10gec rmon;
    cyg_uint32 _reserved6[0x39a];
};

// Bit field definitions are TBD

#endif
