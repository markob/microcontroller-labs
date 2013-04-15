#ifndef _MOTOR_CTRL_H_
#define _MOTOR_CTRL_H_

#include <reg52.h>

#include "config.h"
#include "pwmgen.h"

typedef enum {
	MOVE_FW,
	MOVE_BW,
	TURN_R,
	TURN_L,
} ControlCmd_t;

#define REAR_WHEELS_FW PWM_PIN22
#define REAR_WHEELS_BW PWM_PIN21
#define FRONT_WHEELS_R PWM_PIN11
#define FRONT_WHEELS_L PWM_PIN12

#define InitMotors() do { \
	PWM_Init(); \
	PWM_SetPinOnOffFactor(REAR_WHEELS_FW, 0); \
	PWM_SetPinOnOffFactor(FRONT_WHEELS_R, 0); \
	PWM_Start(); } while (0)
#define CarMoveFw(s) PWM_SetPinOnOffFactor(REAR_WHEELS_FW, (s))
#define CarMoveBw(s) PWM_SetPinOnOffFactor(REAR_WHEELS_BW, (s))
#define CarTurnR() PWM_SetPinOnOffFactor(FRONT_WHEELS_R, 8)
#define CarTurnL() PWM_SetPinOnOffFactor(FRONT_WHEELS_L, 8)
#define CarStop() do { \
	PWM_SetPinOnOffFactor(REAR_WHEELS_FW, 0); \
	PWM_SetPinOnOffFactor(FRONT_WHEELS_R, 0); \
	} while(0);

#endif /*_MOTOR_CTRL_H_*/
