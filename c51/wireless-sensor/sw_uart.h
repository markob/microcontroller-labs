#ifndef _SW_UART_H
#define _SW_UART_H

#include <stc12.h>
#include "types.h"

//define baudrate const
//BAUD = 256 - FOSC/3/BAUDRATE/M (1T:M=1; 12T:M=12)
//NOTE: (FOSC/3/BAUDRATE) must be greater than 98, (RECOMMEND GREATER THAN 110)
//#define BAUD	 0xF400 //  1200bps @ 11.0592MHz
//#define BAUD	 0xFA00 //  2400bps @ 11.0592MHz
//#define BAUD	 0xFD00 //  4800bps @ 11.0592MHz
//#define BAUD	 0xFE80 //  9600bps @ 11.0592MHz
//#define BAUD	 0xFF40 // 19200bps @ 11.0592MHz
//#define BAUD	 0xFFA0	// 38400bps @ 11.0592MHz
//#define BAUD	 0xEC00 //  1200bps @ 18.432MHz
//#define BAUD	 0xF600 //  2400bps @ 18.432MHz
//#define BAUD	 0xFB00 //  4800bps @ 18.432MHz
#define BAUD	   0xFD80 //  9600bps @ 18.432MHz
//#define BAUD	 0xFEC0 // 19200bps @ 18.432MHz
//#define BAUD	 0xFF60 // 38400bps @ 18.432MHz
//#define BAUD	 0xE800 //  1200bps @ 22.1184MHz
//#define BAUD	 0xF400 //  2400bps @ 22.1184MHz
//#define BAUD	 0xFA00 //  4800bps @ 22.1184MHz
//#define BAUD	 0xFD00 //  9600bps @ 22.1184MHz
//#define BAUD	 0xFE80 // 19200bps @ 22.1184MHz
//#define BAUD	 0xFF40 // 38400bps @ 22.1184MHz
//#define BAUD	 0xFF80 // 57600bps @ 22.1184MHz

void UART_init(void);
uint16_t UART_getb(void);
uint8_t UART_putb(uint8_t byte);

#endif
