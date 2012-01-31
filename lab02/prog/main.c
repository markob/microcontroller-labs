#include <reg52.h>
#include "types.h"

#define INDICATORS_NUM	4
#define KBD_ROWS_NUM	4
#define KBD_COLS_NUM	3
#define KBD_SCAN_TRIES	3

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
	uint8_t i, j, k, keys;
	uint8_t key[KBD_COLS_NUM] = {0};

	/* scan keyboard for changes */
	/* scan keys column several times in order to avoid a jitter */
	for (k = 0; k < KBD_SCAN_TRIES; k++) {
		for (i = 0; i < KBD_ROWS_NUM; i++) {
			uint8_t keyMask = 0x01;

			keysPort = 0x7f&(0x10<<i);
			keys = keysPort&0x0f;

			for (j = 0; j < KBD_COLS_NUM; j++) {
				key[j] += (keys&keyMask)>>j;

				/* return first key code which is in high level 2 times */				
				if (key[j] == 2) {
					return j + i*KBD_COLS_NUM;
				}

				keyMask <<= 1;
			}
		}
	}

	return KBD_ROWS_NUM*KBD_COLS_NUM;
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
