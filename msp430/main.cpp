
#include "io430.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= 0x41;
  P1DIR &= 0x7F;
  
  P1OUT  = 0x00;
  
  for (;;) {
    if (!(P1IN&0x80)) P1OUT = ~0x41;
    else P1OUT = 0x41;
  }
}
