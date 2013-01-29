#ifndef PWM_CONFIG_H_
#define PWM_CONFIG_H_

#include "types.h"
#include <reg51.h>

#define CRYSTAL_FREQUENCY 14765400

#define PWM_OUTPUT_PORT P1
#define PWM_PIN1_MASK 0x01
#define PWM_PIN2_MASK 0x02

#define PWM_LEVELS_NUMBER 16

#endif
