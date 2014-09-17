#include <reg51.h>
#include "sw_uart.h"

void main(void)
{
	TMOD = 0x00; //timer0 in 16-bit auto reload mode
	AUXR = 0x80; //timer0 working at 1T mode
	TL0 = BAUD;
	TH0 = BAUD>>8;
	//initial timer0 and set reload value
	TR0 = 1;
	//tiemr0 start running
	ET0 = 1;
	//enable timer0 interrupt
	PT0 = 1;
	//improve timer0 interrupt priority
	EA = 1;
	//open global interrupt switch
	UART_INIT();
	
	for (;;) {
		if (REND) {
			REND = 0;
			buf[r++ & 0x0f] = RBUF;
		}
		if (TEND) {
			if (t != r) {
				TEND = 0;
				TBUF = buf[t++ & 0x0f];
				TING = 1;
			}
		}
	}
}
