#ifndef CAR01_H_
#define CAR01_H_

#include "types.h"
#include "car01_config.h"

#ifndef CAR_CONTROL_PORT_NUM
#error "CAR_CONTROL_PORT_NUM" must be defined
#endif

#ifndef CAR_MOVE_FWD_PIN_NUM
#error "CAR_MOVE_FWD_PIN_NUM" must be defined
#endif

#ifndef CAR_MOVE_BWD_PIN_NUM
#error "CAR_MOVE_BWD_PIN_NUM" must be defined
#endif

#ifndef CAR_TURN_R_PIN_NUM
#error "CAR_TURN_R_PIN_NUM" must be defined
#endif

#ifndef CAR_TURN_L_PIN_NUM
#error "CAR_TURN_L_PIN_NUM" must be defined
#endif

#if CAR_CONTROL_PORT_NUM == 0
#define CAR_CONTROL_PORT_BASE 0x80
#elif CAR_CONTROL_PORT_NUM == 1
#define CAR_CONTROL_PORT_BASE 0x90
#elif CAR_CONTROL_PORT_NUM == 2
#define CAR_CONTROL_PORT_BASE 0xA0
#elif CAR_CONTROL_PORT_NUM == 3
#define CAR_CONTROL_PORT_BASE 0xB0
#else 
#error "CAR_CONTROL_PORT_NUM" value must be from 0 to 3
#endif

sbit CAR_MOVE_FWD_PIN = CAR_CONTROL_PORT_BASE + CAR_MOVE_FWD_PIN_NUM;
sbit CAR_MOVE_BWD_PIN = CAR_CONTROL_PORT_BASE + CAR_MOVE_BWD_PIN_NUM;
sbit CAR_TURN_L_PIN = CAR_CONTROL_PORT_BASE + CAR_TURN_L_PIN_NUM;
sbit CAR_TURN_R_PIN = CAR_CONTROL_PORT_BASE + CAR_TURN_R_PIN_NUM;

#define CAR_MoveFw() { CAR_MOVE_BWD_PIN = 0; CAR_MOVE_FWD_PIN = 1; }
#define CAR_MoveBw() { CAR_MOVE_FWD_PIN = 0; CAR_MOVE_BWD_PIN = 1; }
#define CAR_TurnL()  { CAR_TURN_R_PIN = 0; CAR_TURN_L_PIN = 1; }
#define CAR_TurnR()  { CAR_TURN_L_PIN = 0; CAR_TURN_R_PIN = 1; }

#define CAR_Start() PWM_Start() 
#define CAR_Stop()  PWM_Stop()

#define CAR_SetSpeed(speed) PWM_SetOnOffFactor(0, speed)

#endif
