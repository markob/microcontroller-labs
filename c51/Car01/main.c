#include <reg51.h>
#include "types.h"

#include "uart.h"
#include "pwm.h"
#include "car01.h"

uint8_t ProcessCommand(void);

void main(void)
{
	// init UART and send initial 'AT' to the BlueCore
	UART_Init();
	UART_SendByte('A');
	UART_SendByte('T');

	// initialize PWM module
	PWM_Init();

	// setup PWM
	PWM_SetPinOnOffFactor(0, 12);
	PWM_SetPinOnOffFactor(1, 15);

	// start PWM
	PWM_Start();

	while (TRUE) {
		while (UART_SendByte('X'));
		while (UART_SendByte('Y'));
		while (UART_SendByte('Z'));
		// input command processing
		ProcessCommand();
		while (UART_SendByte('K'));
	}
}

uint8_t ProcessCommand(void)
{
	uint16_t recvInfo = UART_RecvByte();
	uint8_t rv = 0;

	if (recvInfo>>8 > 0) {
		switch ((uint8_t)recvInfo) {
		case 'f':
			CAR_MoveFw();
			break;
		case 'b':
			CAR_MoveBw();
			break;
		case 'l':
			CAR_TurnL();
			break;
		case 'r':
			CAR_TurnR();
			break;
		case 's':
			CAR_Start();
			break;
		case 't':
			CAR_Stop();
			break;
		default:
			break;
		}
	}

	return rv;
}
