#ifndef _ROBORACE_CONFIG_H_
#define _ROBORACE_CONFIG_H_

#include <reg52.h>
#include "../lib/types.h"

#define CRYSTAL_FREQUENCY 14765400

#define TSK_MAIN 0
#define TSK_LINE_SCAN 1
#define TSK_RANGE_SCAN 2
#define TSK_UART_READ 3

#ifndef SYS_MAIN_TASK
extern uint8_t SYS_status;
#endif  

/* actually line scanner takes 4 pins on P3: from 2 to 5 */
#define LINE_SCAN_PORT P3
#define LINE_SCAN_MASK 0x3C

#define LINE_SCAN_TIMEOUT 100
#define LINE_SCAN_IS_READY (0x01<<TSK_LINE_SCAN)

/* range scan configs */
#define RANGE_SCAN_PORT P3
#define RANGE_SCAN_MASK 0x01

#define RANGE_SCAN_TIMEOUT 100
#define RANGE_SCAN_IS_READY (0x01<<TSK_RANGE_SCAN)

/* UART read module */
#define UART_DATA_IS_AVAILABLE (0x01<<TSK_UART_READ)

#define sys_set_status(mask) \
	(SYS_status|=(mask))
#define sys_clr_status(mask) \
	(SYS_status&=~(mask))
#define sys_get_status(mask) \
	(SYS_status&(mask))

/* PWN module configs */
#define PWM_OUTPUT_PORT P1

sbit PWM_pin11 = PWM_OUTPUT_PORT^0;
sbit PWM_pin12 = PWM_OUTPUT_PORT^1;
sbit PWM_pin21 = PWM_OUTPUT_PORT^2;
sbit PWM_pin22 = PWM_OUTPUT_PORT^3;

#endif /*_ROBORACE_CONFIG_H_*/
