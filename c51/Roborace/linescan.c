#include "linescan.h"
#include <rtx51tny.h>

static uint8_t line_status;

uint8_t LINE_SCAN_GetState(void)
{
	return line_status;
}

void scan_line(void) _task_ TSK_LINE_SCAN
{
	/* task main loop */
	while(1) {
		/* read sensor data */
		LINE_SCAN_PORT |= LINE_SCAN_PINS_MASK;
		line_status = (LINE_SCAN_PORT&LINE_SCAN_PINS_MASK)>>LINE_SCAN_PINS_SHIFT;

		/* signal about new data and wait to next update */
		sys_set_status(LINE_SCAN_IS_READY);

		os_send_signal(TSK_MAIN);
		os_wait2(K_TMO, LINE_SCAN_TIMEOUT);
	} 
}
