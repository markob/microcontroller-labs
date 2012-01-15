#include "utils.h"
#include <reg52.h>

static uint16_t tickCount = 0;

static uint16_t tickTillVal;
static bit 		isTicksOverflow;
static bit		isTimerStarted;

static void Timer2Int(void) interrupt 5 using 1
{
	/* just reset timer flags and update counter */
	TF2 = 0;
	tickCount++;

	/* reset overflow flag on overflow */
	if (!tickCount) isTicksOverflow = 0;
}

void InitUtils(void)
{
	/* Clock initialization */
 	tickCount = 0;

	/* setup timer with 1 ms resolution */
	RCAP2H = 0xF8;
	RCAP2L = 0x30;

	/* setup timer modes and run it */
	CP_RL2 = 0;
	C_T2   = 0;

	ET2    = 1;
	EA	   = 1;
	TR2    = 1;

	/* init timer API's */
	isTimerStarted = 0;
}

void Delay(uint16_t msDelay)
{
	uint16_t startPos = tickCount;
	uint16_t endPos = startPos + msDelay;

	if (startPos > endPos) while (tickCount > endPos);
	while (tickCount <= endPos);
}

void StartTimer(uint16_t msDelay)
{
	uint16_t startTick = tickCount;

	if (isTimerStarted) return;
	isTimerStarted = 1;
	
	tickTillVal = startTick + msDelay;
	isTicksOverflow = startTick > tickTillVal;	
}

uint8_t CheckTimer()
{
	if (!isTimerStarted) return 2;
	if (isTicksOverflow || tickCount > tickTillVal) return 0;
	
	isTimerStarted = 0;
	return 1; 
}
