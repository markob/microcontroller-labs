#include "sw_uart.h"

#define BUFFER_SIZE 4

static uint8_t rx_buf[BUFFER_SIZE];
static uint8_t tx_buf[BUFFER_SIZE];

static uint8_t rx_rd_pos;
static uint8_t rx_wr_pos;
static uint8_t tx_rd_pos;
static uint8_t tx_wr_pos;

static volatile bool_t rx_is_ready;
static volatile bool_t tx_is_ready;

__data __at 0x08 volatile uint8_t TBUF;
__data __at 0x09 volatile uint8_t RBUF;
__data __at 0x0A volatile uint8_t TDAT;
__data __at 0x0B volatile uint8_t RDAT;
__data __at 0x0C volatile uint8_t TCNT;
__data __at 0x0D volatile uint8_t RCNT;
__data __at 0x0E volatile uint8_t TBIT;
__data __at 0x0F volatile uint8_t RBIT;

static volatile bool_t TING, RING;
static volatile bool_t TEND, REND;

// Rx pin (input)
#define RXB  P3_0
// Tx pin (output)
#define TXB  P3_1

void UART_init(void)
{
	// generic part
	rx_rd_pos = 0;
	rx_wr_pos = 0;
	tx_rd_pos = 0;
	tx_wr_pos = 0;

	rx_is_ready = 1;
	tx_is_ready = 1;
	
	// software UART part
	TING = 0;
	RING = 0;
	TEND = 1;
	REND = 0;
	TCNT = 0;
	RCNT = 0;

	TMOD = 0x00;
	AUXR = 0x80;
	TL0 = BAUD&0xFF;
	TH0 = BAUD>>8;
	TR0 = 1;
	ET0 = 1;
	PT0 = 1;
}

uint16_t UART_getb(void)
{
	uint16_t rv = 0x0100;
	if (rx_is_ready) {
		rv = rx_buf[rx_rd_pos++];
		if (rx_rd_pos == BUFFER_SIZE) rx_rd_pos = 0;
		if (rx_wr_pos == rx_rd_pos) rx_is_ready = 0;
	}
	return rv;
}

uint8_t UART_putb(uint8_t byte)
{
	uint8_t rv = 0x01;
	if (tx_is_ready) {
		tx_buf[tx_wr_pos++] = byte;
		if (tx_wr_pos == BUFFER_SIZE) tx_wr_pos = 0;
		if (tx_wr_pos == tx_rd_pos) tx_is_ready = 0;
		rv = 0;
	}
	return rv;
}

void uart_putc(unsigned char c)
{
	while (!TEND);
	TBUF = c;
	TEND = 0;
	TING = 1;	
}

/**
 * Timr 0 ISR. It is used for software UART functionality.
 */
static void TIMER0_ISR() __interrupt 1 __using 1
{
	// process input data
	if (RING) {
		if (--RCNT == 0) {
			RCNT = 3; // reset send baudrate counter
			if (--RBIT == 0) {
				RBUF = RDAT; // save data to RBUF
				RING = 0;
				REND = 1;
			} else {
				RDAT >>= 1;
				// shift Rx data to Rx buffer
				if (RXB) RDAT |= 0x80;
			}
		}
	} else if (!RXB) {
		RING = 1; // set start receive flag
		RCNT = 4; // initialize receive baudrate counter	
		RBIT = 9; // initialize receive bit number
	}
	// process output data
	if (--TCNT == 0) {
		TCNT = 3; // reset send baudrate counter
		if (TING) { // is data for sending?
			if (TBIT == 0) {
				TXB = 0; // send start bit
				TDAT = TBUF; // load data from TBUF to TDAT
				// initialize send bit number (8 data bits + 1 stop bit)
				TBIT = 9;
			} else {
				TDAT >>= 1; // shift data to CY
				if (--TBIT == 0) {
					TXB = 1;
					TING = 0; // stop sending
					TEND = 1; // set send complete flag
				} else {
					TXB = CY; // write CY to TX port
				}
			}
		}
	}
}
