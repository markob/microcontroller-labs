#ifndef UART_H_
#define UART_H_

#include "types.h"

#define UART_USE_SYNC_API 0

void UART_Init(void);

#if UART_USE_SYNC_API == 1

/* Sends a byte to through UART and wait till transmission end */
void UART_SendByte(uint8_t byte);

/* Waits till a byte will be received through UART */
uint8_t UART_RecvByte(void);

#else

/* Sends a byte to UART send buffer and immediately returns */
uint8_t UART_SendByte(uint8_t byte);

/* Returns a number of bytes in the UART input queue in the low byte and
   one byte from this queue in the hight byte of the result */
uint16_t UART_RecvByte(void);

#endif

#endif
