#include <reg52.h>
#include <stdio.h>

#define BANK 2 

void change_state(void);
void state__wait_for_command(void);
void state__process_command(void);

void state__wait_for_command(void) using BANK
{
	while (1) {
		char ch = getchar();

		if ( 'r' == ch ) {
		/* turn cw */
			return 'r';
		}

		if ( 'l' == ch ) {
		/* turn ccw */
			return 'l';
		}

		/* all ather symbols are ignored */
	}
}

void state__process_command() using BANK
{
	send_sync_command();
}
