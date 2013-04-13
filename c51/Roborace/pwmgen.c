#include <reg52.h>
#include <string.h>

#include "pwmgen.h"

/*
 * Description of the PWM generator algorithm
 *
 * Actually this algorithm behavior is the same as hardware implemented	'Phase
 * Correct PWM' on AVRs. However, a few words should be said about modulation
 * frequency and so on. According to different articles about PWM and
 * electromotor control, the optimal modulation frequency depends on a certain
 * motor characteristics and on switching keys optimal frequency. From the one
 * side, for motor higher frequency is better, but switching keys has limited
 * frequency and with higher frequency they becames inefective or not working
 * at all.
 * So, according to previous paragraph and actual at89c2051/at89c4051 hw
 * limitations, was selected about 100 Hz modulation frequency. Please keep in
 * mind that industrial PWM regulators mostly provides PWM modulation in the
 * range from 1 to 8 kHz.
 * Another thing should be keep in mind here: PWM module resolution. Here it's
 * set to 4-bit. From one side - 16 levels is enough for motor speed
 * regulation and from other side - it's good for this slow microcontroller
 * performance - it will be suitable to do other thing without big risk to be
 * interrupted by PWM module ISR.
 */

#define PWM_TIMER_REG_TH ((0xFFFF - CRYSTAL_FREQUENCY/3200/12 + 1)/256)
#define PWM_TIMER_REG_TL ((0xFFFF - CRYSTAL_FREQUENCY/3200/12 + 1)%256)

static uint8_t PWM_pin1Up;
static uint8_t PWM_pin2Up;

static uint8_t PWM_tickCount = 0;

static bit PWM_pin1dir = 0;
static bit PWM_pin2dir = 0;

/* Initialize PWM source based on timer 0 */
void PWM_Init(void)
{
	// set PWM stuf
	PWM_tickCount = PWM_LEVELS_NUMBER - 1;

	// setup timer 0 as 16-bit timer
	T2CON= 0x00;
	// setup initial value for timer
	RCAP2H = PWM_TIMER_REG_TH;
	RCAP2L = PWM_TIMER_REG_TL;
	// run pwm timer with high priority
	PT2 = 1;
	EA  = 1;
	ET2 = 1;
}

/* All PWM routine functionality is embedded to the PWM timer ISR */
void PWM_timerHandle(void) interrupt 5 using 2	 
{
	// restart timer
	TF2 = 0;

	if (PWM_tickCount == PWM_LEVELS_NUMBER - 1) {
	 	if (PWM_pin1Up) {
			PWM_pin11 = 0;
			PWM_pin12 = 0;
		}
		if (PWM_pin2Up) {
			PWM_pin21 = 0;
			PWM_pin22 = 0;
		}
	}
	
	// update timer tick counters
	PWM_tickCount = ++PWM_tickCount&PWM_LEVELS_MASK;

	if (PWM_tickCount == PWM_pin1Up) {
		if (PWM_pin1dir) PWM_pin11 = 1;
		else PWM_pin12 = 1;
	}
	if (PWM_tickCount == PWM_pin2Up) {
		if (PWM_pin2dir) PWM_pin21 = 1;
		else PWM_pin22 = 1;
	}
}

void PWM_SetPinOnOffFactor(PWM_OutPin pin, uint8_t onOffFactor)
{
	uint8_t count = PWM_LEVELS_NUMBER - onOffFactor;

	switch (pin) {
	case PWM_PIN11:
		PWM_pin1dir = 1;
		PWM_pin1Up = count;
		break;
	case PWM_PIN12:
		PWM_pin1dir = 0;
		PWM_pin1Up = count;
		break;
	case PWM_PIN21:
		PWM_pin2dir = 1;
		PWM_pin2Up = count;
		break;
	case PWM_PIN22:
		PWM_pin2dir = 0;
		PWM_pin2Up = count;
		break;
	}
}
