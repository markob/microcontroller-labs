#include <reg52.h>

void main(void)
{
	init_serial_port();
	init_scheduler();

	run_main_loop();
}

void init_serial_port()
{
	
}

void run_main_loop(void)
{
	while (1) {
		/* read command */
		do {
			int err = 0;
			int c_count = 0;
			char ch = getchar();

			switch (ch) {
			case 'c':
				c_count++;
				break;
			case 'w':
				
				break;
			default:
				err = 1;
				break;
			}
		} while (!err);
	}
}
