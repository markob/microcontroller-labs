#include "uart.h"
#include <reg52.h>

#include <rtx51tny.h>

#include "config.h"
#include "uart.h"

/* following value must be power of 2! */
#define UART_WRITE_BUF_SIZE 8
#define UART_BYTE_SEND_TIME 1
#define UART_SLEEP_TIME 5

static uint8_t wr_buf1[UART_WRITE_BUF_SIZE];
static uint8_t wr_buf2[UART_WRITE_BUF_SIZE];
static uint8_t* wr_buf = wr_buf1;

static uint8_t rd_index = 0;
static uint8_t wr_index = 0;
static bit wr_is_full = 0;
static bit wr_is_empty = 1;
static bit wr_is_ready = 0;

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

static void UART_eventHandler(void) interrupt 4 using 3
{
	if (RI) {
		// if input queue is full we are unable to receive any data
		/*if (UART_isInputQueueFull) return;
		// clear received flag to switch the UART to the ready state
		RI = 0;
		// store a received byte in the input queue
		UART_inputQueue[UART_inputQueueWriteIndex++] = SBUF;
		UART_inputQueueWriteIndex %= UART_INPUT_QUEUE_SIZE;
		if (UART_inputQueueWriteIndex == UART_inputQueueReadIndex) {
			UART_isInputQueueFull = 1;
		}*/
	} else {
		/* signal send task that UART module is ready to send data */
		os_send_signal(TSK_UART_WRITE);
	}
}

/* data writer implementation */

uint8_t UART_WriteByte(uint8_t byte)
{
	if (wr_is_full) {
		// output queue is full, should to try again later
		return 0xFF;
	} else {
		wr_buf[wr_index++] = byte;
		wr_index &= UART_WRITE_BUF_SIZE;
		wr_is_full = wr_index == rd_index;
		wr_is_empty = 0;
		if (wr_is_ready) {
			wr_is_ready = 0;
			TI = 1;
		}
	}
	return 0x00;
}

void uart_writer(void) _task_ TSK_UART_WRITE
{
	while(TRUE) {
		/* wait for wake up signal */
		os_wait1(K_SIG);
		
		/* wait for UART module ready */
		while (!TI);
		TI = 0;
		
		/* send one byte and wait */
		SBUF = wr_buf[rd_index++];
		rd_index &= UART_WRITE_BUF_SIZE;
		
		/* input queue is empty - switch to another queue */
		if (rd_index == wr_index) {
			if (wr_buf == wr_buf1) {
				wr_buf = wr_buf2;
				rd_index = 0;
			} else {
				wr_buf = wr_buf1;
				rd_index = 0;
			}
		}
		
		
	}
}

/* data reader implementation */

#if 0

#define UART_READ_BUF_SIZE 8
uint8_t rd_buf[UART_READ_BUF_SIZE];

uint16_t UART_RecvByte(void)
{
	RI = 0;
	while (!RI);

	return SBUF;
}

void uart_read_data(void) _task_ TSK_UART_READ
{
	while(TRUE) {
		UART_RecvData_t rd_data;
		
		/* read received data */
		do {
			rd_data.wd = UART_RecvByte();
			if (rd_data.bt.hb) {
					rd_buf[0] = rd_data.bt.lb;
			} else {
				break;
			}
		} while (TRUE);
		
		/* signal about new data and wait to next update */
		sys_set_status(UART_DATA_IS_AVAILABLE);

		os_send_signal(TSK_MAIN);
		os_wait1(K_SIG);
	}
}
#endif
