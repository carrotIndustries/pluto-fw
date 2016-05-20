#include <msp430.h>
#include <stdint.h>
#include "beepled.h"
#include "io.h"
#include "common/hal/hal.h"

uint16_t g_freq;
uint8_t g_brightness;

void beep_init(void)
{
	PCONF(7, 3, (FUNC1 | OUL)); /* BEEP timer ctrl */

	PCONF(1, 0, (FUNC1 | OUL)); /* LED1 timer ctrl */
	PCONF(J, 1, OUL); /* LED2 to GND */

	TA0CTL = TASSEL__SMCLK;
	TA0CCR2 = 1; /* beep */
}

static void beep_backlight(uint16_t freq, uint8_t brightness) {
	if(!freq && !brightness) { //no beep, no light
		TA0CCTL2 = 0;
		TA0CCTL1 = 0;
		TA0CTL &= ~MC_3;
		PCONF(J, 1, OUL); //light off
	}
	else if(freq && !brightness) { //beep, no light
		TA0CCTL2 = OUTMOD_4;
		TA0CCR0 = 2000000/freq;
		TA0CTL |= MC__UP;
		PCONF(J, 1, OUL); //light off
	}
	else if(!freq && brightness) { //no beep, light
		TA0CCTL2 = 0;
		TA0CCR0 = 30000; // PWM freq: 100 Hz
		TA0CTL |= MC__UP;
		TA0CCR1 = brightness * 3000;
		TA0CCTL1 = OUTMOD_3;
		PCONF(J, 1, OUH); //light on
	}
	else if(freq && brightness) { //beep, light
		TA0CCTL2 = OUTMOD_4;
		TA0CCR0 = 2000000/freq;
		TA0CCR1 = (TA0CCR0/10)*brightness;
		TA0CCTL1 = OUTMOD_3;
		TA0CTL |= MC__UP;

		PCONF(J, 1, OUH); //light on
	}
}

void hal_beep(uint16_t freq) {
	g_freq = freq;
	beep_backlight(g_freq, g_brightness);
}

void hal_backlight_set(uint8_t brightness) {
	g_brightness = brightness;
	beep_backlight(g_freq, g_brightness);
}
