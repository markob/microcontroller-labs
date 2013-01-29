#include "clock.h"
#include <reg51.h>

static UINT16 _fsm_clock__ticks_ = FSM_CLOCK_TICKS_PER_SAMPLE;

void _fsm_clock__clock_(void) using FSM_REG_BANK_IDLE
{
	/* set timer mode to 16-bit timer */
	TMOD = (TMOD&0x0F) | 0x20;

	/* init timer start count */
	TH1 = 0x20;
	TL1 = 0xFF;

	/* enable interrupts and start timer 0 */
	ET1 = 1;
	TR1 = 1;
	EA  = 1;
}

static void _fsm_clock__interrupt_(void) interrupt 1 using FSM_REG_BANK_INTR
{
	_fsm_clock__ticks_--;

	if (0 == _fsm_clock__ticks_) {
		_fsm_clock__ticks_ = FSM_CLOCK_TICKS_PER_SAMPLE;
	}
}
