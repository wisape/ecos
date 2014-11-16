#ifndef CYGONCE_PHY_BCM54xx_HEADER_
#define CYGONCE_PHY_BCM54xx_HEADER_
//==========================================================================
//
//      phy/BCM54xx.h
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
// Date:         2013-02-08
// Purpose:      
// Description:  Register definition for the Broadcom BCM54xx PHY family
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define BCM54xx_MII_STAT_REG            0x01
#define BCM54xx_MII_AUTO_COMPLETED      0x0020  // Auto-negotiation complete
#define BCM54xx_MII_PHY_STAT_LINK_UP    0x0004  // Link up

#define BCM54xx_MII_AUXSTATUS           0x19

#endif // CYGONCE_PHY_BCM54xx_HEADER_
