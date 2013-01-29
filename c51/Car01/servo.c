#include "servo.h"

#if SYSVER == 52
#include <reg52.h>
#else
#include <reg51.h>
#endif

/* calculate system clock ticks per 1/100 of servo range (per 10 usecs) */
#define SERVO_TICKS_PER_10US (CRYSTAL_FREQUENCY/12/100000)

/* check timer ticks per 1/100 of servo range (about 10 usecs), it should be no less than 10 */
#if SERVO_TICKS_PER_10US < 10
#error system clock resolution is too low to control the servo
#endif

#define SERVO_TIMER0_REG_TH ((0xFFFF - SERVO_TICKS_PER_10US + 1)/256)
#define SERVO_TIMER0_REG_TL ((0xFFFF - SERVO_TICKS_PER_10US + 1)%256)

static bit SERVO_isStartCycle = 0;

void SERVO_Timer0ISR(void) interrupt 1 using 3
{
	/* restart the timer */
	TH0 = SERVO_TIMER0_REG_TH;	
	TL0 = SERVO_TIMER0_REG_TL;
	
	TF0 = 1;
	
	/* do appropriate servo control operations */
	SERVO_outputPin0 = SERVO_isStartCycle;
	SERVO_isStartCycle = ~SERVO_isStartCycle;
}
