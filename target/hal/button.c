#include <msp430.h>
#include <stdint.h>
#include "button.h"
#include "io.h"

#define REPEAT_MASK     BTN_ALL
#define REPEAT_START    50   /* 500 ms */
#define REPEAT_NEXT     20   /* 200 ms */

#define DEB_RUNTIME     500 /* 5s */

volatile uint8_t button_state;
volatile uint8_t button_press;
volatile uint8_t button_rpt;
volatile uint16_t debrun = 0;

void button_init(void)
{
	PCONF(9, 4, INPD); /* button alarm */
	PCONF(J, 0, INPD); /* button light */
	PCONF(J, 2, INPD); /* button mode */

	TA1CTL |= TASSEL__ACLK | TAIE;
	TA1CCR0 = 327; /* results in around 100Hz */

	TA3CTL |= TASSEL__ACLK | MC__UP | TAIE;
	TA3CCR0 = 2048; /* results in 16Hz */
}

void __attribute__((interrupt ((TIMER3_A1_VECTOR)))) TimerA3_1_ISR(void)
{
	TA3CTL &= ~TAIFG;
	if(PGET(9,4) | PGET(J,2) | PGET(J,0)) {
		TA1CTL |= MC__UP;
		debrun = 0;
	}
}

/* credit for awesome debouncing routine: Peter Dannegger **********************/
void __attribute__((interrupt ((TIMER1_A1_VECTOR)))) TimerA1_1_ISR(void)
{
	static uint8_t ct0 = 0xFF, ct1 = 0xFF, rpt;
	uint8_t i;
	TA1CTL &= ~TAIFG;

	i = button_state ^ ((PGET(9,4)<<2) | (PGET(J,2)<<1) | PGET(J,0));
	ct0 = ~(ct0 & i);
	ct1 = ct0 ^ (ct1 & i);
	i &= ct0 & ct1;
	button_state ^= i;
	button_press |= button_state & i;

	if((button_state & REPEAT_MASK) == 0)
		rpt = REPEAT_START;
	if(--rpt == 0){
		rpt = REPEAT_NEXT;
		button_rpt |= button_state & REPEAT_MASK;
	}

	if(debrun < DEB_RUNTIME)
		debrun += 1;
	else {
		TA1CTL &= ~MC_3;
	}
	LPM3_EXIT;
}

uint8_t get_button_press(uint8_t button_mask)
{
	__dint();
	__no_operation();
	button_mask &= button_press;
	button_press ^= button_mask;
	__nop();
	__eint();
	return button_mask;
}

uint8_t get_button_rpt(uint8_t button_mask)
{
	__dint();
	__no_operation();
	button_mask &= button_rpt;
	button_rpt ^= button_mask;
	__nop();
	__eint();
	return button_mask;
}

uint8_t get_button_state(uint8_t button_mask)
{
	button_mask &= button_state;
	return button_mask;
}

uint8_t get_button_short(uint8_t button_mask)
{
	return get_button_press(~button_state & button_mask);
}

uint8_t get_button_long(uint8_t button_mask)
{
	return get_button_press(get_button_rpt(button_mask));
}

uint8_t get_button_common(uint8_t button_mask)
{
	return get_button_press((button_press & button_mask) == button_mask ? button_mask : 0);
}

