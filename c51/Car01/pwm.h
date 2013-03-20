#ifndef PWM_H_
#define PWM_H_

#include "types.h"

#include "pwm_config.h"

#define PWM_PINS_NUMBER 2

#ifndef CRYSTAL_FREQUENCY
#error Crystal frequency must be defined
#endif

#ifndef PWM_OUTPUT_PORT
#error "PWM_OUTPUT_PORT" must be defined
#endif

#ifndef PWM_PIN1_MASK
#error "PWM_PIN1_MASK" must be defined
#else
#define PWM_PIN1_IMASK (~PWM_PIN1_MASK)
#endif

#ifndef PWM_PIN2_MASK
#error "PWM_PIN2_MASK" must be defined
#else
#define PWM_PIN2_IMASK (~PWM_PIN2_MASK)
#endif

#ifndef PWM_LEVELS_NUMBER
#error "PWM_LEVEL_NUMBER" must be defined
#endif

/* Initializes PWM module */
void PWM_Init(void);

/*
 * Reconfigures the specified pin settings. 
 * @param pinNumber   - should be 0 or 1 according to 2 PWM pins
 * @param onOffFactor - set appropriate output level for the pin. If it's set
 *                      to '0' - 0 voltage output, 'PWM_LEVELS_NUMBER' - max
 *                      voltage output and other values are in the range.
 */
void PWM_SetPinOnOffFactor(uint8_t pinNumber, uint8_t onOffFactor);

/* Starts the PWM timer - only after that PWM works */
#define PWM_Start() { TR0 = 1; }

/* Stops the PWM timer and resets output pins */
#define PWM_Stop() { \
	TR0 = 0; \
	PWM_OUTPUT_PORT &= PWM_PIN1_IMASK; \
	PWM_OUTPUT_PORT &= PWM_PIN2_IMASK; \
 }

#endif
