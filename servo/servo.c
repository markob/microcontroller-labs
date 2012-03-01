#include "servo.h"

#if SYSVER == 53
#include <reg8253.H>
#elif SYSVER == 52
#include <reg52.h>
#else
#include <reg51.h>
#endif

#define SERVO_TIMER_COUNT_20MS  (CRYSTAL_FREQUENCY/600)
#define SERVO_TIMER_COUNT_005MS (CRYSTAL_FREQUENCY/24000)

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
	
	PT0 = 1;
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
	uint16_t freq = 20*angle/9;		   
	uint16_t count = 0xFFFF -
		SERVO_TIMER_COUNT_005MS*(100 + freq)/100 + 20;

	SERVO_s1RegTH = count/256;
	SERVO_s1RegTL = count%256;

	count = 0xFFFF - SERVO_TIMER_COUNT_20MS +
			SERVO_TIMER_COUNT_005MS*(100 + freq)/100 + 20;
	
	SERVO_s2RegTH = count/256;;
	SERVO_s2RegTL = count%256;;		
}
