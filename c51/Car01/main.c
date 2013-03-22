#include <reg51.h>
#include "types.h"

#include "pwm.h"

void main(void)
{
	uint8_t count = 0;

	// initialize PWM module
	PWM_Init();

	// setup PWM
	PWM_SetPinOnOffFactor(0, 0);
	PWM_SetPinOnOffFactor(1, 0);

	P1 = 0x00;

	// start PWM
	PWM_Start();

	while (TRUE) {
		uint16_t i = 0; for (++i; i != 0; i++);

		PWM_SetPinOnOffFactor(0, count%17);
		PWM_SetPinOnOffFactor(1, count%17);

		++count;
	}
}
