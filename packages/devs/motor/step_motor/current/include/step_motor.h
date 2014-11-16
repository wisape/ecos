#ifndef CYGONCE_DEVS_STEP_MOTOR_H
#define CYGONCE_DEVS_STEP_MOTOR_H
//==========================================================================
//
//      motor.h
//
//      Step Motor device driver
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

#define CYG_DEVS_SET_CONFIG_STEP_MOTOR_MODE 0x0001
#define CYG_DEVS_GET_CONFIG_STEP_MOTOR_MODE 0x0001

#define CYG_DEVS_STEP_MOTOR_DIR_FORWARD   0
#define CYG_DEVS_STEP_MOTOR_DIR_BACKWARD  1

#define CYG_DEVS_STEP_MOTOR_FULL_STEP 0
#define CYG_DEVS_STEP_MOTOR_HALF_STEP 1

//==========================================================================
// Configuration information structure

typedef struct
{
    cyg_uint32  direction; // direction of the rotation
    cyg_uint32  speed;     // speed of the rotation in tr/min
    cyg_uint32  steps;     // number of steps to rotate
} cyg_step_motor_data_t;


typedef struct
{
    cyg_uint32  mode;      // Half / Full Step Mode
} cyg_step_motor_info_t;

#endif // CYGONCE_DEVS_STEP_MOTOR_H

//---------------------------------------------------------------------------
//
