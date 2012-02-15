#include <reg52.h>
#include "types.h"

#define CRYSTAL_FREQUENCY 24000000
#define CLOCK_RESOLV_FREQ 100000

#define TIMER0_REG_TH ((0xFFFF - CRYSTAL_FREQUENCY/CLOCK_RESOLV_FREQ/12 + 1)/256)
#define TIMER0_REG_TL ((0xFFFF - CRYSTAL_FREQUENCY/CLOCK_RESOLV_FREQ/12 + 1)%256)

sbit servoPin = 0x90;

uint8_t timerCount = 0;

void Timer0ISR(void) interrupt 1 using 3
{
	/* restart timer */
	TH0 = TIMER0_REG_TH;
	TL0 = TIMER0_REG_TL;
	TF0 = 1;

	/* do useful stuf */
	timerCount++;
}

void InitTimer0(void)
{
	timerCount = 0;

	TMOD |= 0x01;

	TH0 = TIMER0_REG_TH;
	TL0 = TIMER0_REG_TL;

	PT0 = 1;
	EA  = 1;
	ET0 = 1;
}

void main(void)
{
	InitTimer0();

	while (TRUE) {
		
	}
}
