#include "sw_uart.h"

#define BUFFER_SIZE 8

static unsigned char rx_buf[BUFFER_SIZE];

void UART_init(void)
{
	unsigned int i;
	for (i = 0; i < BUFFER_SIZE; i++)
		rx_buf[i] = 0;
}
