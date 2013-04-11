#include <reg52.h>
#include <rtx51tny.h>

#define SYS_MAIN_TASK 1
#include "config.h"

uint8_t SYS_status = 0x00;

#include "motorctrl.h"
#include "uart.h"

/* it's main task and here is main control loop */
void main_task(void) _task_ 0
{
	/* initialize all other tasks */
	os_create_task(TSK_LINE_SCAN);  /* task for line scanner read */	
	os_create_task(TSK_RANGE_SCAN); /* task for range scanner read */
	os_create_task(TSK_UART_READ);  /* task for UART read */
	os_create_task(TSK_UART_WRITE);  /* task for UART write */

	/* initialize motor controller modules (PWM generator) */
	InitMotors();

	/* initialize UART module */
	UART_Init();

	/* task main loop */
	while(1) {
		/* wait for some event(signal) */
		os_wait1(K_SIG);

		/* process retrieved signal */

		/* line scan module */
		if (sys_get_status(LINE_SCAN_IS_READY)) {
			sys_clr_status(LINE_SCAN_IS_READY);
		}
		
		/* range scan module */	
		if (sys_get_status(RANGE_SCAN_IS_READY)) {
			sys_clr_status(RANGE_SCAN_IS_READY);
		}

		/* UART module */
		if (sys_get_status(UART_DATA_IS_AVAILABLE)) {
			sys_clr_status(UART_DATA_IS_AVAILABLE);

			
		}
	}
}
