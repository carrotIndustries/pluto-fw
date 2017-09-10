#include <msp430.h>
#include "wdt.h"
#include "common/hal/lcd_segments.h"
#include "common/svc/svc.h"

void wdt_clear(void)
{
	WDTCTL = WDT_ARST_1000;
}
