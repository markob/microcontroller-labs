#include "servo.h"

#if SYSVER == 52
#include <reg52.h>
#else
#include <reg51.h>
#endif

/* calculate system clock ticks per 1/100 of servo range (per 10 usecs) */
#define SERVO_TICKS_PER_10US (CRYSTAL_FREQUENCY/12/100000)

/**
 check timer ticks per 1/100 of servo range (about 10 usecs),
 it should be no less than 10
 */
#if SERVO_TICKS_PER_10US < 10
#error system clock resolution is too low to control the servo
#endif

#define SERVO_TIMER_COUNT_20MS (CRYSTAL_FREQUENCY/600)
#define SERVO_TIMER_COUNT_01MS (CRYSTAL_FREQUENCY/12000)

static bit SERVO_isStartCycle = 0;

static uint8_t SERVO_s1RegTH = 0;
static uint8_t SERVO_s1RegTL = 0;
static uint8_t SERVO_s2RegTH = 0;
static uint8_t SERVO_s2RegTL = 0;

void SERVO_Timer0ISR(void) interrupt 1 using 3
{
	/* restart the timer */
	if (SERVO_isStartCycle) {
		TH0 = SERVO_s1RegTH;	
		TL0 = SERVO_s1RegTL;
	} else {
		TH0 = SERVO_s2RegTH;	
		TL0 = SERVO_s2RegTL;
	}
	
	TF0 = 0;
	
	/* do appropriate servo control operations */
	SERVO_outputPin0 = SERVO_isStartCycle;
	SERVO_isStartCycle = ~SERVO_isStartCycle;
}

void SERVO_Init(void)
{
	/* initialize timer 0 as clock for the servo lib */
	TMOD |= 0x01; 

	TH0 = SERVO_s1RegTH;
	TL0 = SERVO_s1RegTL;

	TR0 = 0;

	PT0 = 1;
	ET0 = 1;
	EA  = 1;

	/* initialize other servo lib stuff */
	SERVO_isStartCycle = 1;
}

void SERVO_SetAngle(uint8_t angle)
{
	uint16_t freq = angle == 0 ? 10: 1800/angle;
	uint16_t count = 0xFFFF - SERVO_TIMER_COUNT_01MS - (CRYSTAL_FREQUENCY/1200)/freq + 5;

	SERVO_s1RegTH = count/256;
	SERVO_s1RegTL = count%256;

	count = 0xFFFF - SERVO_TIMER_COUNT_20MS +
			(SERVO_TIMER_COUNT_01MS + (CRYSTAL_FREQUENCY/1200)/freq + 5);
	
	SERVO_s2RegTH = count/256;;
	SERVO_s2RegTL = count%256;;		
}
