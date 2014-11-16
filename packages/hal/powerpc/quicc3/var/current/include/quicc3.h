//==========================================================================
//
//      quicc3.h
//
//      Variant register definitions
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System. 
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 
// 2008, 2009, 2010 Free Software Foundation, Inc.
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
// Date:         2009-11-01
// Purpose:      
// Description:  Freescale Quicc III register definitions
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef CYGONCE_HAL_PPC_QUICC3_H
#define CYGONCE_HAL_PPC_QUICC3_H

#include <cyg/hal/plf_regs.h>           // For IMM base

#define UART1_OFFSET    0x4500
#define UART2_OFFSET    0x4600

typedef struct IMMR {
  /* TBD*/
} IMMR;

typedef struct i2c_controller {
 volatile cyg_uint8 i2c1adr;
 volatile cyg_uint8 Reserved35[0x3];
 volatile cyg_uint8 i2c1fdr;
 volatile cyg_uint8 Reserved36[0x3];
 volatile cyg_uint8 i2c1cr;
 volatile cyg_uint8 Reserved37[0x3];
 volatile cyg_uint8 i2c1sr;
 volatile cyg_uint8 Reserved38[0x3];
 volatile cyg_uint8 i2c1dr;
 volatile cyg_uint8 Reserved39[0x3];
 volatile cyg_uint8 i2c1dfsrr;
 volatile cyg_uint8 Reserved40[0xEB];
}i2c_controller;


typedef struct l2cache_mm {
  volatile cyg_uint32 l2ctl;
  volatile cyg_uint32 l2cwap;
  volatile cyg_uint8  reserved0[0x8];
  volatile cyg_uint32 l2cewar0;
  volatile cyg_uint8  reserved1[0x4];
  volatile cyg_uint32 l2cewcr0;
  volatile cyg_uint8  reserved2[0x4];
  volatile cyg_uint32 l2cewar1;
  volatile cyg_uint8  reserved3[0x4];
  volatile cyg_uint32 l2cewcr1;
  volatile cyg_uint8  reserved4[0x4];
  volatile cyg_uint32 l2cewar2;
  volatile cyg_uint8  reserved5[0x4];
  volatile cyg_uint32 l2cewcr2;
  volatile cyg_uint8  reserved6[0x4];
  volatile cyg_uint32 l2cewar3;
  volatile cyg_uint8  reserved7[0x4];
  volatile cyg_uint32 l2cewcr3;
  volatile cyg_uint8  reserved8[0xB4];
  volatile cyg_uint32 l2srbar0;
  volatile cyg_uint32 l2srbarea0;
  volatile cyg_uint32 l2srbar1;
  volatile cyg_uint32 l2srbarea1;  
  volatile cyg_uint8  reserved10[0xCF0];
  volatile cyg_uint32 l2errinjhi;
  volatile cyg_uint32 l2errinjlo;
  volatile cyg_uint32 l2errinjctl;
  volatile cyg_uint8  reserved11[0x14];
  volatile cyg_uint32 l2captdatahi;
  volatile cyg_uint32 l2captdatalo;
  volatile cyg_uint32 l2captecc;
  volatile cyg_uint8  reserved12[0x14];
  volatile cyg_uint32 l2errdet;
  volatile cyg_uint32 l2errdis;
  volatile cyg_uint32 l2errinten;
  volatile cyg_uint32 l2errattr;
  volatile cyg_uint32 l2erraddrl;
  volatile cyg_uint32 l2erraddrh;
  volatile cyg_uint32 l2errctl;
} l2cache_mm;

typedef struct ecm_mm {
  volatile cyg_uint32 eebacr;
  volatile cyg_uint8  reserved0[0xc];
  volatile cyg_uint32 eebpcr;
  volatile cyg_uint8  reserved1[0xbe4];
  volatile cyg_uint32 ecm_ip_b1;
  volatile cyg_uint32 ecm_ip_b2;
  volatile cyg_uint8  reserved2[0x200];
  volatile cyg_uint32 eedr;
  volatile cyg_uint8  reserved3[0x4];
  volatile cyg_uint32 eeer;
  volatile cyg_uint32 eeatr;
  volatile cyg_uint32 eeladr;
  volatile cyg_uint32 eehadr;
} ecm_mm;

typedef struct sys_mon_85xx {
 volatile cyg_uint32 pmgc0;
 volatile cyg_uint8 Reserved0[0xc];
 volatile cyg_uint32 pmlca0;
 volatile cyg_uint32 pmlcb0;
 volatile cyg_uint32 pmlc0_lo;
 volatile cyg_uint32 pmlc0_hi;
 volatile cyg_uint32 pmlca1;
 volatile cyg_uint32 pmlcb1;
 volatile cyg_uint32 pmlc1;
 volatile cyg_uint8 Reserved1[0x4];
 volatile cyg_uint32 pmlca2;
 volatile cyg_uint32 pmlcb2;
 volatile cyg_uint32 pmlc2;
 volatile cyg_uint8 Reserved2[0x4];
 volatile cyg_uint32 pmlca3;
 volatile cyg_uint32 pmlcb3;
 volatile cyg_uint32 pmlc3;
 volatile cyg_uint8 Reserved3[0x4];
 volatile cyg_uint32 pmlca4;
 volatile cyg_uint32 pmlcb4;
 volatile cyg_uint32 pmlc4;
 volatile cyg_uint8 Reserved4[0x4];
 volatile cyg_uint32 pmlca5;
 volatile cyg_uint32 pmlcb5;
 volatile cyg_uint32 pmlc5;
 volatile cyg_uint8 Reserved5[0x4];
 volatile cyg_uint32 pmlca6;
 volatile cyg_uint32 pmlcb6;
 volatile cyg_uint32 pmlc6;
 volatile cyg_uint8 Reserved6[0x4];
 volatile cyg_uint32 pmlca7;
 volatile cyg_uint32 pmlcb7;
 volatile cyg_uint32 pmlc7;
 volatile cyg_uint8 Reserved7[0x4];
 volatile cyg_uint32 pmlca8;
 volatile cyg_uint32 pmlcb8;
 volatile cyg_uint32 pmlc8;
 volatile cyg_uint8 Reserved8[0x4];
 volatile cyg_uint32 pmlca9;
 volatile cyg_uint32 pmlcb9;
 volatile cyg_uint32 pmlc9;
 volatile cyg_uint8 Reserved9[0x4];
 volatile cyg_uint32 pmlca10;
 volatile cyg_uint32 pmlcb10;
 volatile cyg_uint32 pmlc10;
 volatile cyg_uint8 Reserved10[0x4];
 volatile cyg_uint32 pmlca11;
 volatile cyg_uint32 pmlcb11;
 volatile cyg_uint32 pmlc11;
}sys_mon_85xx;

#endif // ifndef CYGONCE_HAL_PPC_QUICC3_H
