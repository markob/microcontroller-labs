#ifndef PWM_H_
#define PWM_H_

#include "types.h"
#include "config.h"

// PWM level number must be 2^n
#define PWM_LEVELS_NUMBER 16
#define PWM_LEVELS_MASK (PWM_LEVELS_NUMBER - 1)

#define PWM_PINS_NUMBER 2

#ifndef CRYSTAL_FREQUENCY
#error Crystal frequency must be defined
#endif

#ifndef PWM_OUTPUT_PORT
#error "PWM_OUTPUT_PORT" must be defined
#endif

#ifndef PWM_LEVELS_NUMBER
#error "PWM_LEVEL_NUMBER" must be defined
#endif

typedef enum {
	PWM_PIN11,
	PWM_PIN12,
	PWM_PIN21,
	PWM_PIN22,
} PWM_OutPin;

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
#define PWM_Start() { TR2 = 1; }

/* Stops the PWM timer and resets output pins */
#define PWM_Stop() { \
	TR2 = 0; \
	PWM_OUTPUT_PORT &= PWM_PIN1_IMASK; \
	PWM_OUTPUT_PORT &= PWM_PIN2_IMASK; \
 }

#endif
