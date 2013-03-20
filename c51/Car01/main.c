#include <reg51.h>
#include "types.h"

#include "pwm.h"

void main(void)
{
	// initialize PWM module
	PWM_Init();

	// setup PWM
	PWM_SetPinOnOffFactor(0, 5);
	PWM_SetPinOnOffFactor(1, 10);

	P1 = 0x00;

	// start PWM
	PWM_Start();

	while (TRUE) {
	}
}

