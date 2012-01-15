#include <reg52.h>
#include "utils.h"

sfr outIndSelectorPort = 0x90;
sfr outSymbolPort = 0xA0;

unsigned char secsCount = 0;
unsigned char minsCount = 0;
unsigned char hoursCount = 0;

code unsigned char symbolsMap[] = { 
	0x03, 0xCF, 0x91, 0x87, 0x4D, 0x25, 0x21, 0x8F, 0x01, 0x05 };
unsigned char symbols[4] = {0xFF};

void UpdateTime()
{
	switch (CheckTimer()) {
	case 2:
		StartTimer(1000);
		break;
	case 1:
		StartTimer(1000);
		secsCount++;
		symbols[3] = symbolsMap[secsCount%10];
		break;
	default:
		break;
	} 
}

void UpdateIndOut(void)
{
	static unsigned char count = 0;
	outIndSelectorPort = ~(0x01 << (count%8));
	outSymbolPort = 0x91;
	count++;
}

void main(void)
{
	InitUtils();	

	while(1) {
		UpdateTime();
		UpdateIndOut();
	}
}
