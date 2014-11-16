//==========================================================================
//
//      step_motor_test.c
//
//      Step Motor Test Application
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2008 Free Software Foundation, Inc.
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
// Date:         2010-10-23
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================
#include <pkgconf/system.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/io.h>
#include <cyg/io/step_motor.h>


int
step_motor_test( void )
{
   cyg_step_motor_data_t mdata;
   cyg_uint32 step_len = sizeof(cyg_step_motor_data_t);
   cyg_io_handle_t motor0;
   cyg_step_motor_info_t cfg_data;
   cyg_uint32 len = sizeof(cyg_step_motor_info_t);
   int res;

   // Lookup motor
   res = cyg_io_lookup( "/dev/motor0", &motor0 );

   if( res != ENOERR )
   {
      diag_printf("Error looking up Motor0 \n\r");
      return -1;
   }

   cfg_data.mode = CYG_DEVS_STEP_MOTOR_HALF_STEP;
   res = cyg_io_set_config(motor0, CYG_DEVS_SET_CONFIG_STEP_MOTOR_MODE, &cfg_data, &len);

   if ( res != ENOERR )
   {
      diag_printf("Error configuring Motor0 \n\r");
      return -1;
   }

   mdata.direction = CYG_DEVS_STEP_MOTOR_DIR_FORWARD;
   mdata.steps = 60;
   mdata.speed = 30;

   diag_printf("Motor0 speed %d [tr/min], steps %d ( 1tr = 50 steps )\n",
           mdata.speed,
           mdata.steps);

   res = cyg_io_write( motor0, &mdata, &step_len );

   if( res != ENOERR )
   {
      diag_printf("Error driving Motor0 \n\r");
      return -1;
   }

   return 0;
}

void
cyg_start( void )
{
   step_motor_test();
}


