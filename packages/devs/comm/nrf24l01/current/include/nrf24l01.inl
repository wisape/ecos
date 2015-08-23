//==========================================================================
//
//      nrf24l01.inl
//
//      2.4G RF NRF24L01 driver
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
// Date:         2015-08-05
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================
#ifndef CYG_NRF24L01_INL
#define CYG_NRF24L01_INL
#include <cyg/io/spi_stm32.h>

CYG_DEVS_SPI_CORTEXM_STM32_DEVICE (
    nrf24l01_spi_device, 2, 0, false, 0, 0, 1000000, 1, 1, 1
);

DEVIO_TABLE(nrf24l01_handles,
	nrf24l01_write,
	nrf24l01_read,
	NULL,
	nrf24l01_get_config,
	nrf24l01_set_config
);

DEVTAB_ENTRY(nrf24l01_io,
	CYGDAT_DEVS_COMM_NRF24L01_NAME,
	NULL,
	&nrf24l01_handles,
	nrf24l01_init,
	nrf24l01_lookup,
	NULL
);

#endif
