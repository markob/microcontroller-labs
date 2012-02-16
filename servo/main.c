#include <reg52.h>

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

		for (i = 0; i < 10; i++) {
			SERVO_SetAngle(90);
			Delay(1000);
		}
	}
}
