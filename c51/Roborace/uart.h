#ifndef _UART_H_
#define _UART_H_

#include "types.h"

void UART_Init(void);

/* Sends a byte to through UART and wait till transmission end */
uint8_t UART_WriteByte(uint8_t byte);

/* Waits till a byte will be received through UART */
uint16_t UART_ReadByte(void);

#endif /*_UART_H_*/
