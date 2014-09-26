#include "sw_uart.h"

#define BUFFER_SIZE 4

static uint8_t rx_buf[BUFFER_SIZE];
static uint8_t tx_buf[BUFFER_SIZE];

static uint8_t rx_rd_pos;
static uint8_t rx_wr_pos;
static uint8_t tx_rd_pos;
static uint8_t tx_wr_pos;

volatile enum {
	TX_INACTIVE,
	TX_STARTING,
	TX_SENDING,
	TX_STOPPING,
} tx_state;

static volatile bool_t tx_is_ready;
static volatile bool_t tx_data_available;
static volatile uint8_t tx_clk_count;
static volatile uint8_t tx_bit_count;
static volatile uint8_t tx_send_byte;

volatile uint8_t RBUF;
volatile uint8_t RDAT;
volatile uint8_t RCNT;
volatile uint8_t RBIT;

static volatile bool_t rx_is_ready;
static volatile bool_t rx_data_available;
static volatile bool_t RING;
static volatile bool_t REND;

// Rx pin (input)
#define RXB  P3_0
// Tx pin (output)
#define TXB  P3_1

#define UART_lock()   (ET0 = 0)
#define UART_unlock() (ET0 = 1)

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
	tx_state = TX_INACTIVE;
	tx_data_available = 0;
	tx_clk_count = 0;
	tx_bit_count = 0;
	tx_send_byte = 0;
	
	REND = 0;
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
		uint8_t wr_pos = tx_wr_pos;
		tx_buf[wr_pos++] = byte;
		if (wr_pos == BUFFER_SIZE) wr_pos = 0;
		UART_lock();
		if (wr_pos == tx_rd_pos) tx_is_ready = 0;
		tx_wr_pos = wr_pos;
		tx_data_available = 1;
		UART_unlock();
		rv = 0;
	}	
	return rv;
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
	// Tx channel functionality
	if (--tx_clk_count == 0) {
		tx_clk_count = 3; // reset send baudrate counter
		switch (tx_state) {
		case TX_INACTIVE: // do nothing - no data to transmit
			break;
		case TX_STARTING: // send start bit and get byte from queue
			TXB = 0;
			tx_send_byte = tx_buf[tx_rd_pos++];
			tx_is_ready = 1;
			if (tx_rd_pos == BUFFER_SIZE) tx_rd_pos = 0;
			if (tx_rd_pos == tx_wr_pos) tx_data_available = 0;
			tx_bit_count = 8;
			tx_state = TX_SENDING;
			break;
		case TX_SENDING:
			tx_send_byte <<= 1;
			TXB = CY;
			if (--tx_bit_count == 0) tx_state = TX_STOPPING;
			break;
		case TX_STOPPING:
			TXB = 1;
			tx_state = TX_INACTIVE;
			break;
		}
	}
	if (tx_clk_count == 2) {
		// check if new data is available for sending
		if (tx_state == TX_INACTIVE && tx_data_available) {
			tx_state = TX_STARTING;
		}
	}
}
