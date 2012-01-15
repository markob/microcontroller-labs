#include <reg52.h>
#include "utils.h"

sfr outPort = 0x90;
sfr inPort = 0xA0;

void main(void)
{
	uint8_t outSym = 0xFE;
	uint8_t speed  = 0;
	uint16_t delay = 100;

	/* utils countains a clock, so it should be initialized */
	InitUtils();	

	while(1) {
		/* update speed from input */
		inPort = 0x0F;
		speed = inPort&0x0F;

		/* update output */
		outPort = outSym;
		outSym = outSym == 0x7F ? 0xFE: (outSym << 1) + 1;

		/* wait a while */
		Delay(delay*speed);
	}
}
