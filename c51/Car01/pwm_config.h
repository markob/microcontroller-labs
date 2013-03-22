#ifndef _PWM_CONFIG_H_
#define _PWM_CONFIG_H_

#include "types.h"
#include <reg51.h>
#include "config.h"

#define PWM_OUTPUT_PORT P1
#define PWM_PIN1_MASK 0x01
#define PWM_PIN2_MASK 0x04

#define USE_PHASE_CORRECTED_PWM 1

// PWM level number must be 2^n
#define PWM_LEVELS_NUMBER 16
#if USE_PHASE_CORRECTED_PWM == 1
#define PWM_LEVELS_MASK (2*(PWM_LEVELS_NUMBER) - 1)
#else
#define PWM_LEVELS_MASK (PWM_LEVELS_NUMBER - 1)
#endif


#endif /*_PWM_CONFIG_H_*/
