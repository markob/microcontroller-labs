#include <stc12.h>

#include "pt/pt.h"
#include "sw_uart.h"

static struct pt nrf24l01_pt;
static struct pt worker_pt;

/**
 * nRF24L01 service. Reads data from nRF module and writes to it.
 **/
static PT_THREAD(nrf24l01_task(struct pt *pt))
{
	PT_BEGIN(pt);
	for (;;) {
		PT_YIELD(pt);
	}
	PT_END(pt);
}

static PT_THREAD(worker_task(struct pt *pt))
{
	PT_BEGIN(pt);
	for (;;) {
		uint16_t data;
		if ((data = UART_getb()) >> 8) {
			uint8_t byte = (uint8_t) data;
			UART_putb(byte);
		}
		PT_YIELD(pt);
	}
	PT_END(pt);
}

void main()
{ 
	// init section
	UART_init();
	
	// enable interrupts
	EA = 1;
	
	// initialize all pthread tasks
	PT_INIT(&nrf24l01_pt);
	PT_INIT(&worker_pt);
	for (;;) {
		nrf24l01_task(&nrf24l01_pt);
		worker_task(&worker_pt);
	}
}
