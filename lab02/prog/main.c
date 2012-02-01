#include <reg52.h>
#include "types.h"

#define INDICATORS_NUM	4
#define KBD_ROWS_NUM	4
#define KBD_COLS_NUM	3
#define KBD_SCAN_TRIES	3

#define INVALID_KEY KBD_COLS_NUM*KBD_ROWS_NUM

#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))

sfr indicatorsPort 	= 0x90;
sfr segmentsPort 	= 0xA0;
sfr keysPort 		= 0xB0;

static uint8_t activeIndicator 	= 0;
static uint8_t activeCount 		= 0;

code uint8_t symsMap[] = {
	0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6 };

uint8_t outInfo[INDICATORS_NUM] = {0};

uint8_t GetKeyPressed(void)
{
	uint8_t i, keys;
	uint8_t key = INVALID_KEY;

	/* scan keyboard for changes */
	for (i = 0; i < KBD_ROWS_NUM; i++) {
		keysPort = 0x7f&(0x01<<i);
		keys = keysPort&0x70;

		if (keys != 0x70) {
			/* some key in row was pressed */
			switch (keys&0x70) {
			case 0x60:
				key = 1;
				break; 
			case 0x50:
				key = 0x20;
				break;
			case 0x30:
				key = 3;
				break;
			default:
				key = INVALID_KEY;
				break;
			}
		}

		if (key != INVALID_KEY) {
			return key + KBD_COLS_NUM*i;
		}
	}

	return INVALID_KEY;
}

void UpdateDisplay(void)
{
	static uint8_t i = 0;

	/* update 7-segment display information - update only one indicator in order to have
	   it powered for loger time - ti increase it's light intensity */

	/* activate next indicator */
	indicatorsPort = ~(0x01 << i);
	/* update active indicator symbol */
	if (activeIndicator != i) {
		segmentsPort = outInfo[i];
	} else {
		if (!activeCount) {
			segmentsPort = outInfo[i];
		} else {
			segmentsPort = 0x00;
		}
		
		activeCount++;	
	}

	i++;
	i %= INDICATORS_NUM;
}

void main(void)
{
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

		UpdateDisplay();		
	}
}
