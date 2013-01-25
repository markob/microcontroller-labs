#include <reg51.h>
#include "types.h"

#define CRYSTAL_FREQUENCY 14745600

///////////////////////////////////////////////////////////////////////////////
// Timer 0 definitions and globals
///////////////////////////////////////////////////////////////////////////////

#define TIMER0_TICS_PER_SECOND 20
#define TIMER0_PERIOD CRYSTAL_FREQUENCY/TIMER0_TICS_PER_SECOND/12

#define TIMER0_REG_TH ((0xFFFF - TIMER0_PERIOD + 1)/256)
#define TIMER0_REG_TL ((0xFFFF - TIMER0_PERIOD + 1)%256)

bit TIMER0_overflow = FALSE;

void TIMER0_Init();

#define TIMER0_Start() do { TR0 = 1; } while (0)
#define TIMER0_Stop() do { TR0 = 0; } while (0)

///////////////////////////////////////////////////////////////////////////////
// Timer 1 definitions and globals
///////////////////////////////////////////////////////////////////////////////

#define TIMER1_TICS_PER_SECOND 200
#define TIMER1_PERIOD CRYSTAL_FREQUENCY/TIMER1_TICS_PER_SECOND/12

#define TIMER1_REG_TH ((0xFFFF - TIMER1_PERIOD + 1)/256)
#define TIMER1_REG_TL ((0xFFFF - TIMER1_PERIOD + 1)%256)

void TIMER1_Init();

#define TIMER1_Start() do { TR1 = 1; } while (0)
#define TIMER1_Stop() do { TR1 = 0; } while (0)

///////////////////////////////////////////////////////////////////////////////
// LED matrix definitions and globals
///////////////////////////////////////////////////////////////////////////////

code const uint8_t LED_symbols[] = {
	0x09, // 0
	0xAF, // 1
	0x31, // 2
	0x23, // 3
	0x87, // 4
	0x43, // 5
	0x41, // 6
	0x2F, // 7
	0x01, // 8
	0x03, // 9
	0xFE  // .
};

uint8_t LED_output[2] = { 0 };
uint8_t LED_currentPos = 0x00;
uint8_t LED_dot[2] = { FALSE };

sbit SYMBOL_0_SELECT_PIN = P3^0;
sbit SYMBOL_1_SELECT_PIN = P3^1;

code const uint8_t PORT_1_LED_AND_MASK = 0x0F;
code const uint8_t PORT_3_LED_AND_MASK = 0xC3;
code const uint8_t PORT_1_LED_OR_MASK  = 0xF0;
code const uint8_t PORT_3_LED_OR_MASK  = 0x3C;

///////////////////////////////////////////////////////////////////////////////
// Buttons defines and globals
///////////////////////////////////////////////////////////////////////////////

#define BUTTON_SKIP_COUNT 4

sbit BUTTON_0_PIN = P1^2;
sbit BUTTON_1_PIN = P1^3;

bit BUTTON_0_is_pressed = 0;
bit BUTTON_1_is_pressed = 0;

///////////////////////////////////////////////////////////////////////////////
// Relay output	definition
///////////////////////////////////////////////////////////////////////////////

sbit RELAY_PIN = P3^7;

///////////////////////////////////////////////////////////////////////////////
// Timer 0 functionality implementation
///////////////////////////////////////////////////////////////////////////////

void TIMER0_Init()
{
	// setup timer 0 as 16-bit timer
	TMOD |= 0x01;
	
	// setup initial value for timer
	TH0 = TIMER0_REG_TH;
	TL0 = TIMER0_REG_TL;

	// run TIMER0 with high priority
	PT0 = 1;
	EA  = 1;
	ET0 = 1;
}

void TIMER0_Handler(void) interrupt 1 using 1	 
{
	static uint8_t TIMER0_tics = 0;	

	// restart timer
	TH0 = TIMER0_REG_TH;
	TL0 = TIMER0_REG_TL;

	TF0 = 0;

	// update timer tick counts
	if (++TIMER0_tics == TIMER0_TICS_PER_SECOND) {
		TIMER0_tics = 0;
		TIMER0_overflow = TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Timer 1 functionality implementation
///////////////////////////////////////////////////////////////////////////////

void TIMER1_Init()
{
	 // setup timer 0 as 16-bit timer
	TMOD |= 0x10;
	
	// setup initial value for timer
	TH1 = TIMER1_REG_TH;
	TL1 = TIMER1_REG_TL;

	// run TIMER1 with low priority
	PT1 = 0;
	EA  = 1;
	ET1 = 1;
}	

void TIMER1_Handler(void) interrupt 3 using 2
{
	register uint8_t currentPos = LED_currentPos++%2;

	static uint8_t BUTTON_0_skip_count = 0;
	static uint8_t BUTTON_1_skip_count = 0;

	// restart timer
	TH1 = TIMER1_REG_TH;
	TL1 = TIMER1_REG_TL;

	TF1 = 0;

	// process buttons
	if (!BUTTON_0_skip_count) {
		BUTTON_0_PIN = 1;
		if (!BUTTON_0_PIN && !BUTTON_0_is_pressed) {
			BUTTON_0_is_pressed = 1;
			BUTTON_0_skip_count = BUTTON_SKIP_COUNT;
		}
	} else {
		// if button is pressed during button skip count it means
		// that button was not released yet
		BUTTON_0_PIN = 1;
		if (!BUTTON_0_PIN) BUTTON_0_skip_count = BUTTON_SKIP_COUNT;
		else --BUTTON_0_skip_count;  
	}
	
	if (!BUTTON_1_skip_count) {
		BUTTON_1_PIN = 1;
		if (!BUTTON_1_PIN && !BUTTON_1_is_pressed) {
			BUTTON_1_is_pressed = 1;
			BUTTON_1_skip_count = BUTTON_SKIP_COUNT;
		}
	} else {
		// if button is pressed during button skip count it means
		// that button was not released yet
		BUTTON_1_PIN = 1;
		if (!BUTTON_1_PIN) BUTTON_1_skip_count = BUTTON_SKIP_COUNT;
		else --BUTTON_1_skip_count;
	}

	// turn off previous symbol
	P1 |= PORT_1_LED_OR_MASK;
	P3 |= PORT_3_LED_OR_MASK;

	// select another symbol
	if (currentPos) {
		SYMBOL_1_SELECT_PIN = 1;
		SYMBOL_0_SELECT_PIN = 0;
	} else {
		SYMBOL_0_SELECT_PIN = 1;
		SYMBOL_1_SELECT_PIN = 0;
	}

	// turn on selected symbol
	P1 &= LED_output[currentPos]|PORT_1_LED_AND_MASK;
	P3 &= (LED_output[currentPos]<<2)|PORT_3_LED_AND_MASK;
}

///////////////////////////////////////////////////////////////////////////////
// LED matrix functionality implementation
///////////////////////////////////////////////////////////////////////////////

void LED_DisplayDigit(uint8_t pos, uint8_t digit)
{
	if (pos <= 1 && digit <= 9) { 
		if (LED_dot[pos]) {
			LED_output[pos] = LED_symbols[0x0A]&LED_symbols[digit];
		} else {
			LED_output[pos] = ~LED_symbols[0x0A]|LED_symbols[digit];
		}
	}
}

void LED_DisplayDot(uint8_t pos, uint8_t dot)
{
	if (pos <= 1) {
		if (dot) {
			LED_dot[pos] = TRUE;
			LED_output[pos] &= LED_symbols[0x0A];
		} else {
			LED_dot[pos] = FALSE;
			LED_output[pos] |= ~LED_symbols[0x0A];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Buttons functionality implementation
///////////////////////////////////////////////////////////////////////////////

uint8_t IsButton1Pressed()
{
	if (BUTTON_0_is_pressed) {
		BUTTON_0_is_pressed = FALSE;
		return TRUE;
	}

	return FALSE;
}

uint8_t IsButton2Pressed()
{
	if (BUTTON_1_is_pressed) {
		BUTTON_1_is_pressed = FALSE;
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Different utilities used in the project
///////////////////////////////////////////////////////////////////////////////

void WaitForSecond()
{
	while (!TIMER0_overflow);
	TIMER0_overflow = FALSE; 
}

void CountDown(uint8_t seconds, uint8_t scale)
{
	uint16_t count = seconds * scale;
	
	WaitForSecond();

	while (count > 0) {		
		uint8_t ticks = 0x00;

		ticks = count / scale;
		LED_DisplayDigit(0x00, ticks % 10);
		LED_DisplayDigit(0x01, ticks / 10);

		if (IsButton1Pressed()) return;

		WaitForSecond();
		count--;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main routine state machine implementation
///////////////////////////////////////////////////////////////////////////////

#define TIME_SCALE 5
#define LDIGIT_INITIAL 0
#define HDIGIT_INITIAL 6

void main(void)
{
	uint8_t ldigit  = LDIGIT_INITIAL;
	uint8_t hdigit  = HDIGIT_INITIAL;

	RELAY_PIN = 0;

	TIMER0_Init();
	TIMER1_Init();

	TIMER0_Start();
	TIMER1_Start();

READY:
	LED_DisplayDigit(0x00, ldigit);
	LED_DisplayDigit(0x01, hdigit);

	LED_DisplayDot(0x00, TRUE);
	LED_DisplayDot(0x01, TRUE);

	if (IsButton1Pressed())	goto TIMER;
	if (IsButton2Pressed()) goto EDIT1;
	goto READY;
EDIT1:
	LED_DisplayDot(0x00, TRUE);
	LED_DisplayDot(0x01, FALSE);

	if (IsButton1Pressed()) {
		ldigit = (ldigit + 1) % 10;
		LED_DisplayDigit(0x00, ldigit);
		goto EDIT1;
	}
	if (IsButton2Pressed()) goto EDIT2;
	goto EDIT1;
EDIT2:
	LED_DisplayDot(0x00, FALSE);
	LED_DisplayDot(0x01, TRUE);

	if (IsButton1Pressed()) {
		hdigit = (hdigit + 1) % 10;
		LED_DisplayDigit(0x01, hdigit);
		goto EDIT2;
	}
	if (IsButton2Pressed()) goto READY;
	goto EDIT2;
TIMER:
	LED_DisplayDot(0x00, FALSE);
	LED_DisplayDot(0x01, FALSE);

	RELAY_PIN = 1;
	CountDown(10*hdigit + ldigit, TIME_SCALE);
	RELAY_PIN = 0;

	goto READY;
}
