#include <reg52.h>
#include <rtx51tny.h>

#define SYS_MAIN_TASK 1
#include "config.h"

uint8_t SYS_status = 0x00;

#include "linescan.h"
#include "motorctrl.h"
#include "uart.h"

#define DEBUG_MODE 1

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
			uint8_t line_state = LINE_SCAN_GetState();

			sys_clr_status(LINE_SCAN_IS_READY);
			
			/* get data from line sensor */
			#if DEBUG_MODE == 1
			UART_WriteByte(line_state);
			#endif
		}
		
		/* range scan module */	
		if (sys_get_status(RANGE_SCAN_IS_READY)) {
			sys_clr_status(RANGE_SCAN_IS_READY);
		}

		/* UART module */
		if (sys_get_status(UART_DATA_IS_AVAILABLE)) {
			uint16_t recv_data;

			sys_clr_status(UART_DATA_IS_AVAILABLE);
			
			/* process input data from UART */
			while (0x0100&(recv_data = UART_ReadByte())) {
				uint8_t recv_byte = 0xFF&recv_data;
				UART_WriteByte(recv_byte);
			}
		}
	}
}