#include <reg8253.h>

#include "types.h"
#include "servo.h"
#include "utils.h"

void main(void)
{
	InitUtils();
	SERVO_Init();
	
	SERVO_SetAngle(90);
	SERVO_Start();

	while (TRUE) {
		uint8_t i;

		for (i = 2; i < 14; i++) {
			SERVO_SetAngle(i*9);
			Delay(1000);
			SERVO_SetAngle((15 - i)*9);
			Delay(1000);
		}
	}
}
