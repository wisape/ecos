//==========================================================================
//
//      nrf24l01.c
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
#include <cyg/hal/hal_endian.h>
#include <cyg/io/nrf24l01.h>
#include <cyg/io/spi.h>

#include CYGDAT_DEVS_NRF24L01_INL

#define DUMMY_BYTE    0xA5

/* nRF24L SPI commands */
#define CMD_R_REG              0x00
#define CMD_W_REG              0x20
#define CMD_R_RX_PAYLOAD       0x61
#define CMD_W_TX_PAYLOAD       0xA0
#define CMD_FLUSH_TX           0xE1
#define CMD_FLUSH_RX           0xE2
#define CMD_REUSE_TX_PL        0xE3
#define CMD_ACTIVATE           0x50
#define CMD_RX_PL_WID          0x60
#define CMD_W_ACK_PAYLOAD(P)  (0xA8|(P&0x0F))
#define CMD_W_PAYLOAD_NO_ACK   0xD0
#define CMD_NOP                0xFF

//SPI(NRF24L01)寄存器地址

#define CONFIG          0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
                              //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR      0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH           0x05  //RF通道,bit6:0,工作通道频率;
#define RF_SETUP        0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS          0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                              //bit5:数据发送完成中断;bit6:接收数据中断;
#define MAX_TX  	0x10  //达到最大发送次数中断
#define TX_OK   	0x20  //TX发送完成中断
#define RX_OK   	0x40  //接收到数据中断
#define OBSERVE_TX      0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD              0x09  //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2      0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define FIFO_STATUS     0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
                              //bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;

/* CONFIG register */
#define DEFAULT_CFG	0x8
#define PWR_UP		0x02 //Power up
#define PRX		0x01 //RX mode
#define PTX		0x00 //TX mode

#define R_REG(_reg)	(CMD_R_REG | (_reg & 0x1F))
#define W_REG(_reg)	(CMD_W_REG | (_reg & 0x1F))

#define NRF24L01_PRIO 100

#define CE_L()     CYGHWR_HAL_STM32_GPIO_OUT(CYGHWR_HAL_STM32_PIN_OUT(C, 0, OPENDRAIN, NONE, AT_LEAST(50)), 0)
#define CE_H()     CYGHWR_HAL_STM32_GPIO_OUT(CYGHWR_HAL_STM32_PIN_OUT(C, 0, OPENDRAIN, NONE, AT_LEAST(50)), 1)

/* Default addresses */
const cyg_uint8 RX_ADDRESS[RX_ADR_WIDTH]="serv1";
const cyg_uint8 TX_ADDRESS[TX_ADR_WIDTH]="clie1";
cyg_handle_t   nrf24l01_interrupt_handle;
cyg_interrupt  nrf24l01_interrupt;

int first_write;

static void nrf24l01_ce_set(cyg_uint8 val) {
	if (val)
		CE_H();
	else
		CE_L();
}

static void nrf24l01_spi_read(cyg_uint8 reg, cyg_uint8 *buf, cyg_uint32 count)
{
    cyg_spi_device *spi_device =  &nrf24l01_spi_device;

    // Implement the read operation as a multistage SPI transaction.
    cyg_spi_transaction_begin(spi_device);
    cyg_spi_transaction_transfer(spi_device, true,
                                 1, &reg, NULL, false);
    cyg_spi_transaction_transfer(spi_device, true, count, NULL,
                                 buf, true);
    cyg_spi_transaction_end(spi_device);
}

static void nrf24l01_spi_write(cyg_uint8 reg, const cyg_uint8 *buf, cyg_uint32 count)
{
    cyg_spi_device *spi_device = &nrf24l01_spi_device;

    // Implement the read operation as a multistage SPI transaction.
    cyg_spi_transaction_begin(spi_device);
    cyg_spi_transaction_transfer(spi_device, true,
                                 1, &reg, NULL, false);
    cyg_spi_transaction_transfer(spi_device, true, count, buf,
                                 NULL, true);
    cyg_spi_transaction_end(spi_device);

}

static void nrf24l01_spi_write_reg(cyg_uint8 reg, cyg_uint8 val)
{
	nrf24l01_spi_write(reg, &val, 1);
}

static cyg_uint8 nrf24l01_read_packet(cyg_uint8 *rxbuf)
{
	cyg_uint8 tmp_val;
	nrf24l01_spi_read(STATUS, &tmp_val, 1);
	nrf24l01_spi_write(W_REG(STATUS), &tmp_val, 1);
	if (tmp_val & RX_OK) {
		nrf24l01_spi_read(CMD_R_RX_PAYLOAD, rxbuf, RX_PLOAD_WIDTH);
		nrf24l01_spi_write_reg(CMD_FLUSH_RX, 0xff);
		diag_printf("!!can read data\n");
		return 0;
	}

	return 1;
}

static cyg_uint8 nrf24l01_write_packet(cyg_uint8 *txbuf)
{
	cyg_uint8 tmp_val;

	if (first_write == 1) {
	nrf24l01_spi_read(STATUS, &tmp_val, 1);
	//nrf24l01_spi_write(W_REG(STATUS), &tmp_val, 1);
	if (tmp_val & TX_OK || tmp_val & MAX_TX) {
		nrf24l01_spi_write(W_REG(STATUS), &tmp_val, 1);
		nrf24l01_ce_set(0);
		nrf24l01_spi_write_reg(W_REG(CONFIG), DEFAULT_CFG | PWR_UP | PTX);
		nrf24l01_spi_write_reg(CMD_FLUSH_TX, 0xff);
		nrf24l01_spi_write(CMD_W_TX_PAYLOAD, txbuf, RX_PLOAD_WIDTH);
		diag_printf("!!can write data 0x%x\n", tmp_val);
		nrf24l01_ce_set(1);
		return 0;
	}
	}else {
		nrf24l01_ce_set(0);
		nrf24l01_spi_write_reg(W_REG(CONFIG), DEFAULT_CFG | PWR_UP | PTX);
		nrf24l01_spi_write_reg(CMD_FLUSH_TX, 0xff);
		nrf24l01_spi_write(CMD_W_TX_PAYLOAD, txbuf, RX_PLOAD_WIDTH);
		diag_printf("!!!first read can write data\n");
		nrf24l01_ce_set(1);
		first_write = 1;
		return 0;
	}

	return 1;
}

static cyg_uint32 nrf24l01_ISR(cyg_vector_t vector, cyg_addrword_t data)
{
	cyg_drv_interrupt_mask(vector);
	diag_printf("!!! in nrf24l01_ISR\n");
	cyg_drv_interrupt_acknowledge(vector);
	return (CYG_ISR_CALL_DSR | CYG_ISR_HANDLED);
}
static void nrf24l01_DSR(cyg_vector_t vector, cyg_ucount32 count,
                                cyg_addrword_t data)
{
	diag_printf("!!! in nrf24l01_DSR\n");
	cyg_drv_interrupt_unmask(vector);
}

static Cyg_ErrNo nrf24l01_write(cyg_io_handle_t handle,
				const void *buf,
				cyg_uint32 *len)
{
	cyg_uint8 tmp_val;
	int i;

	if (*len < TX_PLOAD_WIDTH)
		return ENOMEM;

	if (nrf24l01_write_packet(buf))
		return EBUSY;

	return ENOERR;
}

static Cyg_ErrNo nrf24l01_read(cyg_io_handle_t handle, void *buf,
				cyg_uint32 *len)
{
	cyg_uint8 tmp_val;
	int i;

	if (*len < RX_PLOAD_WIDTH)
		return ENOMEM;

	if (nrf24l01_read_packet(buf))
		return EBUSY;
	return ENOERR;
}

static Cyg_ErrNo nrf24l01_set_config(cyg_io_handle_t handle,
				cyg_uint32 cmd,
				const void *buf,
				cyg_uint32 *len)
{
	switch(cmd) {
	case S_ADDR:
		if (*len != RX_ADR_WIDTH)
			return EINVAL;
		nrf24l01_ce_set(0);
		nrf24l01_spi_write(W_REG(RX_ADDR_P1), buf, RX_ADR_WIDTH);
		nrf24l01_ce_set(1);
		break;
	case T_ADDR:
		if (*len != TX_ADR_WIDTH)
			return EINVAL;
		/*RX_ADDR_P0 must be set to the sending addr for auto ack to work.*/
		nrf24l01_spi_write(W_REG(RX_ADDR_P0), buf, TX_ADR_WIDTH);
		nrf24l01_spi_write(W_REG(TX_ADDR), buf, TX_ADR_WIDTH);
		break;
	case CHN:
		if (*len != sizeof(cyg_uint8))
			return EINVAL;
		nrf24l01_spi_write(W_REG(RF_CH), (cyg_uint8*)buf, *len);
		break;
	default:
		return EINVAL;
	}
	return ENOERR;
}

static Cyg_ErrNo nrf24l01_get_config(cyg_io_handle_t handle,
				cyg_uint32 cmd,
				void *buf,
				cyg_uint32 *len)
{
	return ENOERR;
}

static Cyg_ErrNo nrf24l01_lookup(struct cyg_devtab_entry **tab,
				struct cyg_devtab_entry  *sub_tab,
				const char *name)
{

	cyg_drv_interrupt_unmask(CYGNUM_HAL_INTERRUPT_EXTI0);
	diag_printf("!! in nrf lookup\n");
	return ENOERR;
}

static void nrf24l01_config(void)
{
	nrf24l01_ce_set(0);
	nrf24l01_spi_write_reg(W_REG(CONFIG), 0x03);
	/*RX_ADDR_P0 must be set to the sending addr for auto ack to work.*/
	nrf24l01_spi_write(W_REG(RX_ADDR_P0), TX_ADDRESS, TX_ADR_WIDTH);
	nrf24l01_spi_write(W_REG(TX_ADDR), TX_ADDRESS, TX_ADR_WIDTH);
	nrf24l01_spi_write(W_REG(RX_ADDR_P1), RX_ADDRESS, RX_ADR_WIDTH);
	nrf24l01_spi_write_reg(W_REG(EN_AA), 0x03);
	nrf24l01_spi_write_reg(W_REG(EN_RXADDR), 0x03);
	nrf24l01_spi_write_reg(W_REG(RF_CH), 3);
	nrf24l01_spi_write_reg(W_REG(RX_PW_P0), RX_PLOAD_WIDTH);
	nrf24l01_spi_write_reg(W_REG(RX_PW_P1), RX_PLOAD_WIDTH);
	nrf24l01_spi_write_reg(W_REG(RF_SETUP), 0x0f);
	/* Default use rx mode */
	nrf24l01_spi_write_reg(W_REG(CONFIG), DEFAULT_CFG | PWR_UP | PRX);
	nrf24l01_ce_set(1);
}

static bool nrf24l01_init(struct cyg_devtab_entry *tab)
{
	int i;
	cyg_uint8 tmp_val;

	/* Default config */
	nrf24l01_config();

	cyg_uint8 add_val[5];
	for (i = 0; i < 0x18; i++){
		nrf24l01_spi_read(i, &tmp_val, 1);
		diag_printf("%x = 0x%x\n", i, tmp_val);
	}
	nrf24l01_spi_read(RX_ADDR_P0, &add_val, 5);
	for (i = 0; i < 5; i++){
	diag_printf("add%d = 0x%x\n", i, add_val[i]);
	}
	nrf24l01_spi_read(RX_ADDR_P1, &add_val, 5);
	for (i = 0; i < 5; i++){
	diag_printf("add%d = 0x%x\n", i, add_val[i]);
	}

	CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_PIN_IN(A, 0, FLOATING));
	cyg_drv_interrupt_configure(CYGNUM_HAL_INTERRUPT_EXTI0, 0, 0);
	HAL_WRITE_UINT32(CYGHWR_HAL_STM32_AFIO + CYGHWR_HAL_STM32_AFIO_EXTICR1,
				CYGHWR_HAL_STM32_AFIO_EXTICRX_PORTA(0));
	cyg_drv_interrupt_create(CYGNUM_HAL_INTERRUPT_EXTI0,
				NRF24L01_PRIO,
				(CYG_ADDRWORD)&tmp_val,
				nrf24l01_ISR,
				nrf24l01_DSR,
				&nrf24l01_interrupt_handle,
				&nrf24l01_interrupt
				);
	cyg_drv_interrupt_attach(nrf24l01_interrupt_handle);

	first_write = 0;

	return true;

}
