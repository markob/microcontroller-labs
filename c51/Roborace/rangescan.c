#include <reg52.h>
#include <rtx51tny.h>

#include "config.h"
#include "../lib/types.h"

void scan_range(vois) _task_ TSK_RANGE_SCAN
{
	/* initial setup */

	/* task main loop */
	while(1) {
		//uint8_t line_scan = 0x00;
		/* read sensor data */
		//LINE_SCAN_PORT |= LINE_SCAN_MASK;
		//line_scan = LINE_SCAN_PORT&LINE_SCAN_MASK;

		/* signal about new data and wait to next update */
		sys_set_status(RANGE_SCAN_IS_READY);

		os_send_signal(TSK_MAIN);
		os_wait2(K_TMO, RANGE_SCAN_TIMEOUT);
	} 
}
