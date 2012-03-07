#include <reg8253.h>

#include "types.h"
#include "utils.h"
#include "uart.h"

typedef union {
	struct {
		uint8_t h;
		uint8_t l;
	} byte;
	uint16_t word;
} DataReceived_t;

void main(void)
{
	InitUtils();
	UART_Init();
	
	UART_SendByte('A');
	UART_SendByte('T');

	while (TRUE) {
		DataReceived_t serialData = { 0 };

		do {
			serialData.word = UART_RecvByte();
		} while (!serialData.byte.h);

		P1 = serialData.byte.l;

		Delay(5000);

		while(UART_SendByte(serialData.byte.l));
	}
}
