#include <reg52.h>
#include "../../../lib/types.h"

#define INDICATORS_NUM	4

#define INVALID_KEY KBD_COLS_NUM*KBD_ROWS_NUM

#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))

sfr indicatorsPort 	= 0x90;
sfr segmentsPort 	= 0xA0;

sfr kbdOutPort 	= 0x90; /* P1 - 4 active keyboard pins - selectors */
sfr kbdScanPort = 0xB0;	/* P3 - 3 passive keyboard pins to scan */

code uint8_t symsMap[] = {
	0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09 };

#define KBD_ROWS_NUM 4
#define KBD_COLS_NUM 3

code uint8_t keyMap[KBD_ROWS_NUM][KBD_COLS_NUM] = {
	{'0', '.', '*'},
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'}};

uint8_t outInfo[INDICATORS_NUM] = { 0x00 };

uint8_t GetKeyPressed(void)
{
	uint8_t i, key;

	/* scan keyboard for changes */
	for (i = 0; i < KBD_ROWS_NUM; i++) {
		key = kbdOutPort&0x0F;
		kbdOutPort = key|(0x10<<i);

		kbdScanPort |= 0xE0;
		key = kbdScanPort&0xE0;

		switch (key) {
		case 0xC0:
			return keyMap[i][0];
		case 0xA0:
			return keyMap[i][1];
		case 0x60:
			return keyMap[i][2];
		default:
			break;
		}
	}

	return INVALID_KEY;
}

void UpdateDisplay(void)
{
	static uint8_t i = 0;

	/* it's just needed to avoid massive CPU usage in simulator and to make each
	   symbol be switched on for a longer time */
	static uint8_t j = 0;
	if (++j) return;

	i++;
	i %= INDICATORS_NUM;

	/* clean current indicator */
	segmentsPort = 0xFF;

	/* activate next indicator */
	indicatorsPort = 0x01 << i;
	/* update active indicator symbol */
	segmentsPort = outInfo[i];	
}

void main(void)
{
	uint8_t activeIndicatorVal 	  = 0xFF;
	uint16_t activeIndicatorCount = 0;
	uint8_t activeIndicator		  = 0;

	while(1) {
		uint8_t key = GetKeyPressed();

		if (key < SIZEOF_ARRAY(symsMap)) {
			/* update output synbol for active indicator appropriate to pressed key */
			outInfo[activeIndicator] = symsMap[key];
		} else if (key == SIZEOF_ARRAY(symsMap)) {
			/* shift active indicator right/left */
			activeIndicator++;
			activeIndicator %= INDICATORS_NUM;
		}

		/* flip-flop an active indicator */
		if (!activeIndicatorCount++) {
			if (activeIndicatorVal == 0xFF) {
				activeIndicatorVal = outInfo[activeIndicator];
				outInfo[activeIndicator] = 0xFF;
			} else {
				outInfo[activeIndicator] = activeIndicatorVal;
				activeIndicatorVal = 0xFF;
			}
		}

		UpdateDisplay();
	}
}
