#include "sw_uart.h"

#define BUFFER_SIZE 4

static u8 rx_buf[BUFFER_SIZE];
static u8 tx_buf[BUFFER_SIZE];

static u8 rx_rd_pos;
static u8 rx_wr_pos;
static u8 tx_rd_pos;
static u8 tx_wr_pos;

static __bit rx_is_ready;
static __bit tx_is_ready;

void UART_init(void)
{
	rx_rd_pos = 0;
	rx_wr_pos = 0;
	tx_rd_pos = 0;
	tx_wr_pos = 0;

	rx_is_ready = 1;
	tx_is_ready = 1;
}

u16 UART_getb(void)
{
	u16 rv = 0x0100;
	if (rx_is_ready) {
		rv = rx_buf[rx_rd_pos++];
		if (rx_rd_pos == BUFFER_SIZE) rx_rd_pos = 0;
		if (rx_wr_pos == rx_rd_pos) rx_is_ready = 0;
	}
	return rv;
}

u8 UART_putb(u8 byte)
{
	u8 rv = 0x01;
	if (tx_is_ready) {
		tx_buf[tx_wr_pos++] = byte;
		if (tx_wr_pos == BUFFER_SIZE) tx_wr_pos = 0;
		if (tx_wr_pos == tx_rd_pos) tx_is_ready = 0;
		rv = 0;
	}
	return rv;
}

void TIMER1_ISR(void) __interrupt 3 __using 1
{
}
