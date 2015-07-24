//==========================================================================
//
//      pwm_stm32.c
//
//      STM32 PWM driver
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
// Date:         2015-06-14
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================
#include <cyg/hal/hal_endian.h>
#include <cyg/io/pwm_stm32.h>

#include CYGDAT_DEVS_STM32_PWM_INL

#define MAX_PERIOD		0x200
#define DEFAULT_CRR		300
#define CC1_PWM1_MODE		VALUE_(4, 6)
#define CC2_PWM1_MODE		VALUE_(12, 6)
#define CC3_PWM1_MODE		VALUE_(4, 6)
#define CC4_PWM1_MODE		VALUE_(12, 6)
#define CC1_PE			BIT_(3)
#define CC2_PE			BIT_(11)
#define CC3_PE			BIT_(3)
#define CC4_PE			BIT_(11)

#define CCxP			BIT_(1)
#define CCxE			BIT_(0)
#define CCx			(CCxP | CCxE)
#define CC1			VALUE_(0, CCx)
#define CC2			VALUE_(4, CCx)
#define CC3			VALUE_(8, CCx)
#define CC4			VALUE_(12, CCx)


static Cyg_ErrNo stm32_pwm_write(cyg_io_handle_t handle,
				const void *buf,
				cyg_uint32 *len)
{

	return ENOERR;

}

static Cyg_ErrNo stm32_pwm_read(cyg_io_handle_t handle,
				void *buf,
				cyg_uint32 *len)
{
	return ENOERR;

}

static Cyg_ErrNo stm32_pwm_set_config(cyg_io_handle_t handle,
				cyg_uint32 cmd,
				const void *buf,
				cyg_uint32 *len)
{
	return ENOERR;

}

static Cyg_ErrNo stm32_pwm_get_config(cyg_io_handle_t handle,
				cyg_uint32 cmd,
				void *buf,
				cyg_uint32 *len)
{
	return ENOERR;

}

static Cyg_ErrNo stm32_pwm_lookup(struct cyg_devtab_entry **tab,
				struct cyg_devtab_entry  *sub_tab,
				const char *name)
{
	diag_printf("In pwm lookup func!!\r\n");
	return ENOERR;

}

/* Init the timer which use for pwm */
static void init_pwm_timer(void)
{
	cyg_uint32 tim_base;
	cyg_uint16 temp;

	/* Use TIM3 as the PWM out timer */
	tim_base = CYGHWR_HAL_STM32_TIM3;

	/* If the pins need remap, add remap code here*/
	/* To add remap code */

	/* Default counterMode_Up */
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_PSC, 0);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CR2, 0 );
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_ARR, MAX_PERIOD - 1);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_EGR, BIT_(0));

	/* Set PWM mode and set Preload enable */
	HAL_READ_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCMR1, temp);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCMR1,
				temp | CC1_PWM1_MODE | CC2_PWM1_MODE
				| CC1_PE | CC2_PE);
	HAL_READ_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCMR2, temp);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCMR2,
				temp | CC3_PWM1_MODE | CC4_PWM1_MODE
				| CC3_PE | CC4_PE);

	/* Enable auto preload */
	HAL_READ_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CR1, temp);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CR1,
				temp | CYGHWR_HAL_STM32_TIM_CR1_ARPE);

	/* Set default level and enable output */
	HAL_READ_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCER, temp);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCER,
				temp | CC1 | CC2 | CC3 | CC4);

	/* Set default compare value */
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCR1, DEFAULT_CRR);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCR2, DEFAULT_CRR);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCR3, DEFAULT_CRR);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CCR4, DEFAULT_CRR);

	/* Enable PWM timer */
	HAL_READ_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CR1, temp);
	HAL_WRITE_UINT16(tim_base+CYGHWR_HAL_STM32_TIM_CR1,
			temp | CYGHWR_HAL_STM32_TIM_CR1_CEN);
}


static bool stm32_pwm_init(struct cyg_devtab_entry *tab)
{
	cyg_uint32 pwm_0, pwm_1, pwm_2, pwm_3;

	/* Enable gpio and pwm timer clock */

	diag_printf("In pwm init func!!\r\n");

	/* Configure the pwm GPIOs */
	pwm_0 = CYGHWR_HAL_STM32_GPIO(A, 6, OUT_50MHZ, ALT_PUSHPULL);
	pwm_1 = CYGHWR_HAL_STM32_GPIO(A, 7, OUT_50MHZ, ALT_PUSHPULL);
	pwm_2 = CYGHWR_HAL_STM32_GPIO(B, 0, OUT_50MHZ, ALT_PUSHPULL);
	pwm_3 = CYGHWR_HAL_STM32_GPIO(B, 1, OUT_50MHZ, ALT_PUSHPULL);

	CYGHWR_HAL_STM32_GPIO_SET(pwm_0);
	CYGHWR_HAL_STM32_GPIO_SET(pwm_1);
	CYGHWR_HAL_STM32_GPIO_SET(pwm_2);
	CYGHWR_HAL_STM32_GPIO_SET(pwm_3);

	init_pwm_timer();

	return true;
}
