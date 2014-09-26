#include "sw_uart.h"

#define BUFFER_SIZE 4

static uint8_t rx_buf[BUFFER_SIZE];
static uint8_t tx_buf[BUFFER_SIZE];

static uint8_t rx_rd_pos;
static uint8_t rx_wr_pos;
static uint8_t tx_rd_pos;
static uint8_t tx_wr_pos;

// flag indicates that transmission is active
static volatile bool_t tx_is_active;
// flag indicates that buffer is not full
static volatile bool_t tx_is_ready;
// flag indicates that buffer has data to transmit
static volatile bool_t tx_data_available;
// internal Tx channel implementation stuff
static volatile uint8_t tx_clk_count;
static volatile uint8_t tx_bit_count;
static volatile uint8_t tx_send_byte;

// flag indicates that data is receiving
static volatile bool_t rx_is_active;
// flag indicates that ???
static volatile bool_t rx_is_ready;
// flag indicates that data is available in buffer
static volatile bool_t rx_data_available;
// internal Rx channel implementation stuff
static volatile uint8_t rx_clk_count;
static volatile uint8_t rx_bit_count;
static volatile uint8_t rx_recv_byte;

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
	// Tx channel initialization
	tx_is_active = 0;
	tx_data_available = 0;
	tx_clk_count = 0;
	tx_bit_count = 0;
	tx_send_byte = 0;
	// Rx channel initialization
	rx_is_active = 0;
	rx_data_available = 0;
	rx_clk_count = 0;
	rx_bit_count = 0;
	rx_recv_byte = 0;
	
	// clock initialization
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
		uint8_t rd_pos = rx_rd_pos;
		rv = rx_buf[rd_pos++];
		if (rd_pos == BUFFER_SIZE) rd_pos = 0;
		UART_lock();
		if (rx_wr_pos == rx_rd_pos) rx_is_ready = 0;
		rx_rd_pos = rd_pos;
		UART_unlock();
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
	// Rx channel functionality
	if (rx_is_active) {
		if (--rx_clk_count == 0) {
			rx_clk_count = 3;
			if (--rx_bit_count == 0) {
				rx_buf[rx_wr_pos++] = rx_recv_byte;
				if (rx_wr_pos == BUFFER_SIZE) rx_wr_pos = 0;
				if (rx_wr_pos == rx_rd_pos) rx_is_ready = 0;
				rx_is_active = 0;
			} else {
				rx_recv_byte >>= 1;
				if (RXB) rx_recv_byte |= 0x80;
			}
		}
	} else {
		if (!RXB) {
			rx_is_active = 1;
			rx_bit_count = 9;
			rx_clk_count = 4;
		}
	}
	// Tx channel functionality
	if (--tx_clk_count == 0) {
		tx_clk_count = 3; // reset send baudrate counter
		if (tx_is_active) {
			if (--tx_bit_count == 9) {
				// send start bit and get byte from queue
				TXB = 0;
				tx_send_byte = tx_buf[tx_rd_pos++];
				tx_is_ready = 1;
				if (tx_rd_pos == BUFFER_SIZE) tx_rd_pos = 0;
				if (tx_rd_pos == tx_wr_pos) tx_data_available = 0;
			} else if (tx_bit_count == 0) {
				TXB = 1;
				tx_is_active = 0;
			} else {
				tx_send_byte <<= 1;
				TXB = CY;
			}
		}
	} else if (tx_clk_count == 2) {
		// check if new data is available for sending
		if (!tx_is_active && tx_data_available) {
			tx_is_active = 1;
			tx_bit_count = 10;
		}
	}
}
