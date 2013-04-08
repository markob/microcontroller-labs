#include <reg52.h>
#include <rtx51tny.h>

#include "config.h"
#include "uart.h"

void scan_uart_input(void) _task_ TSK_UART_READ
{
	while(1) {
		/* signal about new data and wait to next update */
		sys_set_status(UART_DATA_IS_AVAILABLE);

		os_send_signal(TSK_MAIN);
		os_wait1(K_SIG);
	}
}
