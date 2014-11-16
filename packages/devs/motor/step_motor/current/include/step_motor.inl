//==========================================================================
//
//      step_motor.inl
//
//      Step Motor IO interface
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

#ifndef CYGONCE_DEVS_STEP_MOTOR_INL
#define CYGONCE_DEVS_STEP_MOTOR_INL

// Declare Step Motor0
#ifdef CYGPKG_DEVS_STEP_MOTOR0

//==========================================================================
// Instantiate data structures

static step_motor_info step_motor_info0 =
{
  // 4 IOs to control bipolar step motor
  .sig_a     = CYGDAT_DEVS_STEP_MOTOR0_IO_A,
  .sig_a_bar = CYGDAT_DEVS_STEP_MOTOR0_IO_A_BAR,
  .sig_b     = CYGDAT_DEVS_STEP_MOTOR0_IO_B,
  .sig_b_bar = CYGDAT_DEVS_STEP_MOTOR0_IO_B_BAR,
  .steps     = CYGDAT_DEVS_STEP_MOTOR0_STEPS,
  .max_speed = CYGDAT_DEVS_STEP_MOTOR0_MAX_SPEED,
  .user_info = {
       .mode = CYG_DEVS_STEP_MOTOR_FULL_STEP
   }
};


// -------------------------------------------------------------------------
// Device instance

DEVIO_TABLE(cyg_io_motor_devio,
    step_motor_write,
    NULL,
    NULL,
    step_motor_get_config,
    step_motor_set_config
);

DEVTAB_ENTRY(
    motor_io0,
    CYGDAT_DEVS_STEP_MOTOR0_NAME,
    0,
    &cyg_io_motor_devio,
    step_motor_init,
    step_motor_lookup,
    &step_motor_info0
);

#endif // CYGPKG_DEVS_STEP_MOTOR0

#endif // CYGONCE_DEVS_STEP_MOTOR_INL
