#include <reg52.h>
#include "utils.h"

#define INDICATORS_NUM 8

sfr outIndSelectorPort = 0x90;
sfr outSymbolPort = 0xA0;

unsigned char symsMap[] = {
	0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6 };

void main(void)
{
	InitUtils();	

	while(1) {

		/* update 7-segment display information */
		for (i = 0; i < INDICATORS_NUM; i++) {
			/* activate next indicator */
			indicatorsPort = ~(0x01 << i);
			/* update active indicator symbol */
			segsPort = outInfo[i];
		}

		/* scan keyboard for changes */
		for (i = 0; i < SCAN_TRIES; i++) {
			for (j = 0; j < KBD_ROWS_NUM; j++) {
				/* select appropriate keys row */
				keysPort |= 0x70;
				keys = keysPort&0x70;
				/* scan keys column */
				keysPort |= 0x0f;
				keys = keysPort&0x0f;
			}
		}
	}
}
