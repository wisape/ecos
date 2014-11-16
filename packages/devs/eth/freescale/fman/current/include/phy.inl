//==========================================================================
//
//      dev/phy.inl
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
// Presently, only the MII interface of DTSEC1 has external signals,
// therefore we can use that interface for configuring all external
// PHY of all the DTSEC interface
static dtsec_mii_t dtsec1_mii =
{
  .miimadd  = ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 +                 \
                CYGARC_REG_IMM_DPAA_FMAN_MDIO1 + CYGARC_REG_IMM_DTSECx_MIIMADD ),
  .miimcom  = ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 +                 \
                CYGARC_REG_IMM_DPAA_FMAN_MDIO1 + CYGARC_REG_IMM_DTSECx_MIIMCOM ),
  .miimstat = ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 +                 \
                CYGARC_REG_IMM_DPAA_FMAN_MDIO1 + CYGARC_REG_IMM_DTSECx_MIIMSTAT ),
  .miimind  = ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 +                 \
                CYGARC_REG_IMM_DPAA_FMAN_MDIO1 + CYGARC_REG_IMM_DTSECx_MIIMIND ),
  .miimcfg  = ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 +                 \
                CYGARC_REG_IMM_DPAA_FMAN_MDIO1 + CYGARC_REG_IMM_DTSECx_MIIMCFG ),
  .miimcon  = ( CYGARC_IMM_BASE + CYGARC_REG_IMM_DPAA_FMAN1 +                 \
                CYGARC_REG_IMM_DPAA_FMAN_MDIO1 + CYGARC_REG_IMM_DTSECx_MIIMCON ),
};

//
// Reset PHY
//
static void
dtsec_reset_phy(volatile dtsec_mii_t *mii)
{
  HAL_WRITE_UINT32(mii->miimcfg, MIIMCFG_RESET);
  HAL_WRITE_UINT32(mii->miimcfg, MIIMCFG_CLOCK_DIV_14);
  HAL_DELAY_US(12500);
}

static void
dtsec_phy_init(void)
{
  HAL_DELAY_US(12500);
}

//
// Read PHY register
//
static bool 
dtsec_phy_get_reg(dtsec_mii_t *mii, int reg, int phy,
                        unsigned short *val)
{
  cyg_uint32 address = phy;
  cyg_uint32 ret_val, i=0;
  cyg_uint32 miimcom, miimind;

  HAL_WRITE_UINT32(mii->miimadd, ((((cyg_uint32) address) << 8) | reg));
  HAL_READ_UINT32(mii->miimcom, miimcom);
  miimcom &= ~MIIMCOM_READ;
  HAL_WRITE_UINT32(mii->miimcom, miimcom);
  miimcom |= MIIMCOM_READ;
  HAL_WRITE_UINT32(mii->miimcom, miimcom);

  HAL_READ_UINT32(mii->miimcom, miimind);
  while ((miimind & MIIMIND_BUSY) == MIIMIND_BUSY && i++ < 500)
  {
    HAL_DELAY_US(10000);
    HAL_READ_UINT32(mii->miimind, miimind);
  }
  //status register
  HAL_READ_UINT32(mii->miimstat, ret_val);
  HAL_READ_UINT32(mii->miimcom, miimcom);
  miimcom &= ~MIIMCOM_READ;
  HAL_WRITE_UINT32(mii->miimcom, miimcom);

  *val = (unsigned short ) ret_val;
  
  return true;
}

//
// Write PHY register
//
static void 
dtsec_phy_put_reg(dtsec_mii_t *mii, int reg, int addr,
                     unsigned short data)
{
  cyg_uint32 address = addr;
  cyg_uint32 miimind, i = 0;
  cyg_uint32 value = (cyg_uint32) data;

  HAL_WRITE_UINT32(mii->miimadd, ((cyg_uint32) address << 8) | reg);
  //AN advertisement register
  HAL_WRITE_UINT32(mii->miimcon, value);
  HAL_READ_UINT32(mii->miimind, miimind);
  while (miimind != 0 && i++ < 500)
  {
    HAL_DELAY_US(10000);
    HAL_READ_UINT32(mii->miimind, miimind);
  }
}

static bool
dtsec1_phy_get_reg(int reg, int addr, unsigned short *val)
{
  return (dtsec_phy_get_reg(&dtsec1_mii, reg, addr, val));
}

static void
dtsec1_phy_put_reg(int reg, int addr, unsigned short data)
{
  dtsec_phy_put_reg(&dtsec1_mii, reg, addr, data);
}

