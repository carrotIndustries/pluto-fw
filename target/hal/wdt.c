#include <msp430.h>
#include "wdt.h"
#include "common/hal/lcd_segments.h"
#include "common/svc/svc.h"

uint8_t tick_event=0;

void wdt_init(void)
{
	WDTCTL =  WDT_ADLY_250;
	SFRIE1 |= WDTIE;
}

void __attribute__((interrupt ((WDT_VECTOR)))) WDTServiceRoutine(void) {
	SFRIFG1 &= ~WDTIFG;
	tick_event = 1;
	LPM3_EXIT;
}
