#include "sw_uart.h"

__data __at 0x08 volatile BYTE TBUF;
__data __at 0x09 volatile BYTE RBUF;
__data __at 0x0A volatile BYTE TDAT;
__data __at 0x0B volatile BYTE RDAT;
__data __at 0x0C volatile BYTE TCNT;
__data __at 0x0D volatile BYTE RCNT;
__data __at 0x0E volatile BYTE TBIT;
__data __at 0x0F volatile BYTE RBIT;

volatile BOOL TING,RING;
volatile BOOL TEND,REND;

void UART_INIT();

void uart_putc(unsigned char c)
{
	while (!TEND);
	TBUF = c;
	TEND = 0;
	TING = 1;	
}

#ifdef UART_TEST
BYTE t, r;
BYTE buf[16];

void main()
{
	UART_INIT();

	while (1) {
		if (REND) {
			REND = 0;
			buf[r++ & 0x0f] = RBUF;
		}
		if (TEND) {
			if (t != r) {
				TEND = 0;
				TBUF = buf[t++ & 0x0f];
				TING = 1;
			}
		}
	}
}
#endif

void TIMER0_ISR() interrupt 1 using 1
{
	__asm
		jb	_RING,00002$	
		jb	_RXB,00000$	
		setb	_RING
		mov	r5,#4
		mov	r7,#9
		sjmp	00000$	
	00002$:
		djnz	r5, 00000$	;  if (--RCNT==0)
		mov	r5,#3		; RCNT=3
		djnz	r7, 00001$	;  if (--RBIT==0)
		mov	_RBUF,r3	; RBUF <= r3 (RDAT)
		clr	_RING
		setb	_REND
		sjmp	00000$
	00001$:
		mov	a,r3		; r3=RDAT	
		mov	C,_RXB		; (C+RDAT)>>1
		rrc	a
		mov	r3,a
	00000$:
	__endasm;
	
	__asm
		djnz	r4,00010$
		mov	r4,#3
		jnb	_TING, 00010$
		mov	a, r6
		jnz	00011$
		clr	_TXB
		mov	_TDAT,r0		; TDAT=r2 <= r0=TBUF
		mov	r6,#9
		sjmp	00010$
	00011$:
		mov	a,r2
		setb	c
		rrc	a
		mov	r2,a
		mov	_TXB,c
		djnz	r6, 00010$
		clr	_TING
		setb	_TEND
	00010$:
	__endasm;
}

void UART_INIT()
{
	TING = 0;
	RING = 0;
	TEND = 1;
	REND = 0;
	TCNT = 0;
	RCNT = 0;

	TMOD = 0x00;
	AUXR = 0x80;
	TL0 = BAUD&0xFF;
	TH0 = BAUD>>8;
	TR0 = 1;
	ET0 = 1;
	PT0 = 1;
	EA  = 1;
}