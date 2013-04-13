#include "uart.h"
#include <reg52.h>

#include <rtx51tny.h>

#include "config.h"
#include "uart.h"

/* following value must be power of 2! */
#define UART_WRITE_BUF_SIZE 5
#define UART_BYTE_SEND_TIME 2

static idata uint8_t send_wr_buf1[UART_WRITE_BUF_SIZE];
static idata uint8_t send_wr_buf2[UART_WRITE_BUF_SIZE];
static uint8_t idata *send_wr_buf = send_wr_buf1;
static uint8_t idata *send_rd_buf = send_wr_buf2;

static int8_t send_rd_index = 0;
static int8_t send_wr_index = 0;

static bit send_is_ready = 1;

#define UART_READ_BUF_SIZE  10
#define UART_BYTE_RECV_TIME 2

static idata uint8_t recv_rd_buf[UART_READ_BUF_SIZE];

static uint8_t recv_rd_index = 0;
static uint8_t recv_wr_index = 0;

static bit recv_is_full  = 0;
static bit recv_is_empty = 1;

void UART_Init(void)
{
	// setup UART mode to 8N1 with timer 1 sync
	SCON  = 0x50;
	// setup timer 1 to UART mode
	TH1	  = 0xFF;
	TMOD |= 0x20;
	// run timer 1
	TR1	  = 1;
	ES	  = 1;
	EA    = 1;
}

static void UART_eventHandler(void) interrupt 4 using 2
{
	if (RI) {
		RI = 0;
		if (!recv_is_full) {
			recv_rd_buf[recv_wr_index++] = SBUF;
			if (recv_wr_index == recv_rd_index) {
				recv_is_full = 1;
			}
		}
	} else {
		/* there are data to send in current buffer */
		TI = 0;
		if (send_rd_index >= 0) {
			SBUF = send_rd_buf[send_rd_index--];
			if (send_rd_index < 0) {
				send_is_ready = 1;
			}
		}
	}
}

/* data writer implementation */

uint8_t UART_WriteByte(uint8_t byte)
{
	if (send_wr_index < UART_WRITE_BUF_SIZE) {
		/* there are empy room in buffer */
		send_wr_buf[send_wr_index++] = byte;
		return 0x00;
	}
	return 0x01;
}

void uart_writer(void) _task_ TSK_UART_WRITE
{
	while(TRUE) {
		/* wait for wake up signal */
		os_wait2(K_TMO, UART_BYTE_SEND_TIME);
		
		/* wait for UART module ready */
		if (send_is_ready && send_wr_index > 0) {
			send_rd_index = send_wr_index;
			send_wr_index = 0;
			send_is_ready = 0;
			
			if (send_wr_buf == send_wr_buf1) {
				send_wr_buf = send_wr_buf2;
				send_rd_buf = send_wr_buf1;
			} else {
				send_wr_buf = send_wr_buf1;
				send_rd_buf = send_wr_buf2;
			}

			/* send first byte from queue */
			SBUF = send_rd_buf[send_rd_index--];
		}
	}
}

/* data reader implementation */

uint16_t UART_ReadByte(void)
{
	uint16_t res = 0x0000;
	if (recv_rd_index > 0) {
		/* read a byte from receiver input buffer */
		res = 0x0100|recv_rd_buf[recv_rd_index++];
		/* reset receiver fullness flag */
		recv_is_full = 0;
		/* and set empty flag if needed */
		if (recv_rd_index == recv_wr_index) {
			recv_is_empty = 1;
		}
		return res;
	}
	return res; 
}

void uart_reader(void) _task_ TSK_UART_READ
{
	while(TRUE) {
		/* wait for wake up signal */
		os_wait2(K_TMO, UART_BYTE_RECV_TIME);
		
		/* signal if data are available */
		if (!recv_is_empty) {
			recv_is_empty = 1;
			
			/* signal about new data and wait to next update */
			sys_set_status(UART_DATA_IS_AVAILABLE);
			os_send_signal(TSK_MAIN);
		}
	}
}
