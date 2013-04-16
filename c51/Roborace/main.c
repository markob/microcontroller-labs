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
	uint8_t mv_speed = 12; /* motion speed - 12/16 by default */

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
			{ uint8_t i = 0;
			  for (; i < 4; i++) {
			  	if (line_state&(0x01<<i)) {
					UART_WriteByte('0' + i);
				} else {
					UART_WriteByte('0');
				}
			  }
			  UART_WriteByte('\n');
			  UART_WriteByte('\r');
			}
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
				switch (recv_byte) {
				case 'w': /* move forward */
					CarMoveFw(mv_speed);
					UART_WriteByte('f');
					UART_WriteByte('w');
					UART_WriteByte('\n');
					UART_WriteByte('\r');
					break;
				case 's': /* move backward */
					CarMoveBw(mv_speed);
					UART_WriteByte('b');
					UART_WriteByte('w');
					UART_WriteByte('\n');
					UART_WriteByte('\r');
					break;
				case 'a': /* turn left */
					CarTurnL();
					UART_WriteByte('t');
					UART_WriteByte('l');
					UART_WriteByte('\n');
					UART_WriteByte('\r');
					break;
				case 'd': /* turn right */
					CarTurnR();
					UART_WriteByte('t');
					UART_WriteByte('r');
					UART_WriteByte('\n');
					UART_WriteByte('\r');
					break;
				case 'q': /* stop the car */
					CarStop();
					UART_WriteByte('s');
					UART_WriteByte('t');
					UART_WriteByte('\n');
					UART_WriteByte('\r');
					break;
				case 'y': /* speed down */
					break;
				case 'x': /* speed up */
					break;
				default: /* do nothing */
					break;
				}
			}
		}
	}
}
