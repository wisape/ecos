//==========================================================================
//
//      mpu6050.inl
//
//      GYRO MPU6050 driver
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
// Author(s):    Wisape
// Contributors:
// Date:         2015-03-08
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================
#ifndef CYG_MPU6050_INL
#define CYG_MPU6050_INL
#include <cyg/io/i2c_stm32.h>

CYG_I2C_DEVICE(
	i2c_mpu6050,
	&i2c_bus1,
	0x68,
	0x00,
	CYG_I2C_DEFAULT_DELAY
);


DEVIO_TABLE(mpu6050_handles,
	mpu6050_write,
	mpu6050_read,
	NULL,
	mpu6050_get_config,
	mpu6050_set_config
);

DEVTAB_ENTRY(mpu6050_io,
	CYGDAT_DEVS_GYRO_MPU6050_NAME,
	NULL,
	&mpu6050_handles,
	mpu6050_init,
	mpu6050_lookup,
	NULL
);

#endif
