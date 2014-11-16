//==========================================================================
//
//      phy/BCM5482.c
//
//      Ethernet PHY driver
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
// Date:         2013-02-10
// Purpose:
// Description:  Hardware driver for Ethernet PHY Broadcom BCM5482
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/devs_eth_phy.h>

#include <cyg/infra/cyg_type.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_tables.h>

#include <cyg/io/eth_phy.h>
#include <cyg/io/eth_phy_dev.h>

#include "BCM54xx.h"

static bool BCM5482_stat(eth_phy_access_t *f, int *state)
{
    unsigned short phy_state;
    int tries;
    int auto_completed = 1;
    int link_up = 0;

    // Read negotiated state from the Quick Poll Detailed Status Register
    if (_eth_phy_read(f, BCM54xx_MII_STAT_REG, f->phy_addr, &phy_state))
    {
        if ((phy_state & BCM54xx_MII_AUTO_COMPLETED) == 0)
        {
            auto_completed = 0;
            eth_phy_printf("... waiting for auto-negotiation");
            for (tries = 0;  tries < CYGINT_DEVS_ETH_PHY_AUTO_NEGOTIATION_TIME;  tries++)
            {
                if (_eth_phy_read(f, BCM54xx_MII_STAT_REG, f->phy_addr, &phy_state))
                {
                    if ((phy_state & BCM54xx_MII_AUTO_COMPLETED) != 0)
                    {
                        auto_completed = 1;
                        break;
                    }
                }
                CYGACC_CALL_IF_DELAY_US(1000000);   // 1 second
                eth_phy_printf(".");
            }
            eth_phy_printf("\n");
        }

        _eth_phy_read(f, BCM54xx_MII_STAT_REG, f->phy_addr, &phy_state);
        if(phy_state & BCM54xx_MII_PHY_STAT_LINK_UP)
          link_up = 1;

        if(auto_completed)
        {
          if(link_up)
            *state |= ETH_PHY_STAT_LINK;

          _eth_phy_read(f, BCM54xx_MII_AUXSTATUS, f->phy_addr, &phy_state);
          phy_state = ((phy_state & 0x0700 ) >> 8);

          if(phy_state == 2 ){
            *state |= ETH_PHY_STAT_FDX;
          }
          else if(phy_state == 3 ){
            *state |= ETH_PHY_STAT_100MB;
          }
          else if(phy_state == 5 ){
            *state |= ETH_PHY_STAT_FDX;
            *state |= ETH_PHY_STAT_100MB;
          }
          else if(phy_state == 6 ){
            *state |= ETH_PHY_STAT_1000MB;
          }
          else if(phy_state == 7 ){
            *state |= ETH_PHY_STAT_1000MB;
            *state |= ETH_PHY_STAT_FDX;
          }

          return true;
        }

    }
    return false;
}

_eth_phy_dev("Broadcom BCM5482", 0x0143BCB2, BCM5482_stat) // This is for Rev 2
