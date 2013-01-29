#include <reg52.h>

void scheduler__init( void )
{
		
}

void timer__callback() interrupt 3 using 1
{
	/* just switch context to another task */
	
}
