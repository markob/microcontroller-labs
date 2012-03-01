#ifndef SERVO_H_
#define SERVO_H_

#include "types.h"
#include "servo_config.h"

#include <reg8253.h>

#if (SERVO_DEVICE_COUNT > 2) || (SERVO_DEVICE_COUNT < 1)
#error Servo motor outputs number should be 1 or 2
#endif

#ifndef SERVO_OUTPUT_PIN0
#error Servo motor output  is not defined
#endif

sbit SERVO_outputPin0 = SERVO_OUTPUT_PIN0;

#if (SERVO_DEVICE_COUNT == 2) && (!defined(SERVO_OUTPUT_PIN1))
#error Second servo output pin is not defined

#endif

#define SERVO_Start() (TR0 = 1)
#define SERVO_Stop()  (TR0 = 0)

void SERVO_Init(void);
void SERVO_SetAngle(uint8_t angle);

#endif
