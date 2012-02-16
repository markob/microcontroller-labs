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

#define SERVO_FREQ_20MS 50
#define SERVO_FREQ_02MS 500
#define SERVO_FREQ_01MS 1000

#define SERVO_FREQ_S1_TO_FREQ_S2(freq_s1) \
	((freq_s1)*SERVO_FREQ_20MS/((freq_s1) - SERVO_FREQ_20MS))

#define SERVO_GET_TH_REG_FROM_FREQ(freq) \
	((0xFFFF - CRYSTAL_FREQUENCY/12/(freq) + 5)/256)
#define SERVO_GET_TL_REG_FROM_FREQ(freq) \
	((0xFFFF - CRYSTAL_FREQUENCY/12/(freq) + 5)%256)

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
	
	TF0 = 1;
	
	/* do appropriate servo control operations */
	SERVO_outputPin0 = SERVO_isStartCycle;
	SERVO_isStartCycle = ~SERVO_isStartCycle;
}

void SERVO_Init(void)
{
	/* initialize timer 0 as clock for the servo lib */
	TH0 = SERVO_s1RegTH;
	TL0 = SERVO_s1RegTL;

	TR0 = 0;

	PT0 = 1;
	ET0 = 1;
	EA  = 1;

	/* initialize other servo lib stuff */
	SERVO_isStartCycle = 0;
}

void SERVO_SetAngle(uint8_t angle)
{
	uint16_t freq = angle == 0 ? 10: 10 + 1800/angle; // *100
	uint16_t count = 0xFFFF - (CRYSTAL_FREQUENCY/1200)/freq;// + 5;

	SERVO_s1RegTH = count/256;
	SERVO_s1RegTL = count%256;

	freq = freq*SERVO_FREQ_20MS/(freq - SERVO_FREQ_20MS);
	count = 0xFFFF - (CRYSTAL_FREQUENCY/1200)/freq;// + 5;
	
	SERVO_s2RegTH = count/256;;
	SERVO_s2RegTL = count%256;;		
}
