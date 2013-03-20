#define BANK 3

void change_state(void);
void state__stopped(void);
void state__do_step(void);

void change_state(void)
{
}

void state__stopped(void)
{
	/* just wait for command */
}

void state__do_step()
{
	while ( count > 0 ) {
		/* do rotation step */
		push_object();
		delay();
	
		if ( !is_push_completed() ) {
		/* somesing wrong happend */
			return FATAL_ERROR;
		}
		/* rotation step has been completed */
		
		count--;
	};
}
