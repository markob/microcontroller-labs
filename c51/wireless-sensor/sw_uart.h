#ifndef _SW_UART_H
#define _SW_UART_H

#include <stc12.h>
#include "types.h"

void UART_init(void);
u16 UART_getb(void);
u8 UART_putb(u8 byte);

#endif
