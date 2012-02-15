#include <reg52.h>
#include "utils.h"

sbit servoPin = 0x90 + 1;

void main(void)
{
	/* utils countains a clock, so it should be initialized */
	InitUtils();	

	while(1) {
		uint8_t angle = 0;
		uint8_t delay = 7;

		for (angle = 0; angle <= 16; angle += 1) {
			servoPin = 1;
			Delay(delay + angle);
			servoPin = 0;
			Delay(10000);
		}
	}
}
