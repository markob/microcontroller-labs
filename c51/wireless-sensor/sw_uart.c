#include "sw_uart.h"

#define BUFFER_SIZE 4

static uint8_t rx_buf[BUFFER_SIZE];
static uint8_t tx_buf[BUFFER_SIZE];

static uint8_t rx_rd_pos;
static uint8_t rx_wr_pos;
static uint8_t tx_rd_pos;
static uint8_t tx_wr_pos;

static volatile bool_t rx_is_ready;
static volatile bool_t tx_is_ready;

__data __at 0x08 volatile uint8_t TBUF;
__data __at 0x09 volatile uint8_t RBUF;
__data __at 0x0A volatile uint8_t TDAT;
__data __at 0x0B volatile uint8_t RDAT;
__data __at 0x0C volatile uint8_t TCNT;
__data __at 0x0D volatile uint8_t RCNT;
__data __at 0x0E volatile uint8_t TBIT;
__data __at 0x0F volatile uint8_t RBIT;

static volatile bool_t TING, RING;
static volatile bool_t TEND, REND;

void UART_init(void)
{
	rx_rd_pos = 0;
	rx_wr_pos = 0;
	tx_rd_pos = 0;
	tx_wr_pos = 0;

	rx_is_ready = 1;
	tx_is_ready = 1;
	
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

uint16_t UART_getb(void)
{
	uint16_t rv = 0x0100;
	if (rx_is_ready) {
		rv = rx_buf[rx_rd_pos++];
		if (rx_rd_pos == BUFFER_SIZE) rx_rd_pos = 0;
		if (rx_wr_pos == rx_rd_pos) rx_is_ready = 0;
	}
	return rv;
}

uint8_t UART_putb(uint8_t byte)
{
	uint8_t rv = 0x01;
	if (tx_is_ready) {
		tx_buf[tx_wr_pos++] = byte;
		if (tx_wr_pos == BUFFER_SIZE) tx_wr_pos = 0;
		if (tx_wr_pos == tx_rd_pos) tx_is_ready = 0;
		rv = 0;
	}
	return rv;
}

void uart_putc(unsigned char c)
{
	while (!TEND);
	TBUF = c;
	TEND = 0;
	TING = 1;	
}

static void TIMER0_ISR() __interrupt 1 __using 1
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
		mov	_TDAT,r0	; TDAT=r2 <= r0=TBUF
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
