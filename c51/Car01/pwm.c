#include "pwm.h"
#include <reg51.h>
#include <string.h>

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

#define PWM_TIMER0_REG_TH ((0xFFFF - CRYSTAL_FREQUENCY/3200/12 + 1)/256)
#define PWM_TIMER0_REG_TL ((0xFFFF - CRYSTAL_FREQUENCY/3200/12 + 1)%256)

static uint8_t PWM_pin1Up;
static uint8_t PWM_pin1Down;
static uint8_t PWM_pin2Up;
static uint8_t PWM_pin2Down;

static uint8_t PWM_pin1UpCfg;
static uint8_t PWM_pin1DownCfg;
static uint8_t PWM_pin2UpCfg;
static uint8_t PWM_pin2DownCfg;

static uint8_t PWM_tickCount = 0;
static bit PWM_updateIsRequired = 0;

/* Initialize PWM source based on timer 0 */
void PWM_Init(void)
{
	// set PWM stuf
	PWM_tickCount = 0;
	PWM_updateIsRequired = 0;

	// setup timer 0 as 16-bit timer
	TMOD |= 0x01;
	// setup initial value for timer
	TH0 = PWM_TIMER0_REG_TH;
	TL0 = PWM_TIMER0_REG_TL;
	// run pwm timer with high priority
	PT0 = 1;
	EA  = 1;
	ET0 = 1;
}

/* All PWM routine functionality is embedded to the PWM timer ISR */
void PWM_timerHandle(void) interrupt 1 using 2	 
{
	// restart timer
	TH0 = PWM_TIMER0_REG_TH;
	TL0 = PWM_TIMER0_REG_TL;

	TF0 = 0;

	if (!PWM_tickCount&&PWM_updateIsRequired) {
		// here config updates should be applied
		PWM_pin1Up = PWM_pin1UpCfg;
		PWM_pin1Down = PWM_pin1DownCfg;
		PWM_pin2Up = PWM_pin2UpCfg;
		PWM_pin2Down = PWM_pin2DownCfg;

		PWM_updateIsRequired = 0;
	}
	
	// do PWM switch stuff and select next switch target
	if ((PWM_tickCount == PWM_pin1Up) &&
		(PWM_tickCount != PWM_LEVELS_NUMBER)) {
		PWM_OUTPUT_PORT |= PWM_PIN1_MASK;
	} else if (PWM_tickCount == PWM_pin1Down) {
		PWM_OUTPUT_PORT &= PWM_PIN1_IMASK;				
	}
	// do PWM switch stuff and select next switch target
	if ((PWM_tickCount == PWM_pin2Up) &&
		(PWM_tickCount != PWM_LEVELS_NUMBER)) {
		PWM_OUTPUT_PORT |= PWM_PIN2_MASK;
	} else if (PWM_tickCount == PWM_pin2Down) {
		PWM_OUTPUT_PORT &= PWM_PIN2_IMASK;				
	}

	// update timer tick counts
	PWM_tickCount = (PWM_tickCount + 1)%(2*PWM_LEVELS_NUMBER);
}

void PWM_SetPinOnOffFactor(uint8_t pinNumber, uint8_t onOffFactor)
{
	uint8_t count = PWM_LEVELS_NUMBER - onOffFactor;

	if (pinNumber == 0) {
		// first pin is going to be updated
		PWM_pin1UpCfg = count;
		PWM_pin1DownCfg = (2*PWM_LEVELS_NUMBER - count)%(2*PWM_LEVELS_NUMBER);
	} else if (pinNumber == 1) {
		// second pin is going to be updated
		PWM_pin2UpCfg = count;
		PWM_pin2DownCfg =  (2*PWM_LEVELS_NUMBER - count)%(2*PWM_LEVELS_NUMBER);
	}

	// set update required flag
	PWM_updateIsRequired = 1;
}
