#include <reg51.h>
#include "types.h"

#include "config.h"
#include "../lib/pwm_bits.h"
#include "../lib/uart.h"

typedef enum {
	MOVE_FW,
	MOVE_BW,
	TURN_R,
	TURN_L,
} ControlCmd_t;

#define REAR_WHEELS_FW PWM_PIN11
#define REAR_WHEELS_BW PWM_PIN12
#define FRONT_WHEELS_R PWM_PIN21
#define FRONT_WHEELS_L PWM_PIN22

#define InitMotors() do { \
	PWM_Init(); \
	PWM_SetPinOnOffFactor(REAR_WHEELS_FW, 0); \
	PWM_SetPinOnOffFactor(FRONT_WHEELS_R, 0); \
	PWM_Start(); } while (0)
#define CarMoveFw(s) PWM_SetPinOnOffFactor(REAR_WHEELS_FW, (s))
#define CarMoveBw(s) PWM_SetPinOnOffFactor(REAR_WHEELS_BW, (s))
#define CarTurnR() PWM_SetPinOnOffFactor(FRONT_WHEELS_R, 10)
#define CarTurnL() PWM_SetPinOnOffFactor(FRONT_WHEELS_L, 10)
#define CarStop() do { \
	PWM_SetPinOnOffFactor(REAR_WHEELS_FW, 0); \
	PWM_SetPinOnOffFactor(FRONT_WHEELS_R, 0); \
	} while(0);


void main(void)
{
	uint8_t count = 0;

	// initialize motors control module (PWM)
	InitMotors();

	// initialize UART
	UART_Init();

	while (TRUE) {
		uint16_t i = 0; for (++i; i != 0; i++);

		CarMoveFw(count%17);
		if (count&0x01) CarTurnR();
		else CarTurnL();

		++count;
	}
}

void ProcCmd(uint8_t cmd)
{
	switch (cmd) {
	case MOVE_FW:
		CarMoveFw(10);
		break;
	case MOVE_BW:
		CarMoveBw(10);
		break;
	case TURN_L:
		CarTurnL();
		break;
	case TURN_R:
		CarTurnR();
		break;
	default:
		break;
	}
}
