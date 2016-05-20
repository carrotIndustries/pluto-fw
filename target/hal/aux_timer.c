#include "aux_timer.h"
#include "io.h"
#include <msp430.h>

uint8_t aux_timer_event;

void __attribute__((interrupt ((TIMER2_A1_VECTOR)))) TimerA2_1_ISR(void) {
	TA2CTL &= ~TAIFG;
	aux_timer_event = 1;
	LPM3_EXIT;
}

void hal_aux_timer_set(uint8_t running) {
	if(running) {
		TA2CTL |= MC__UP;
	}
	else {
		TA2CTL &= ~MC_3;
	}
}

void aux_timer_init(void) {
	TA2CTL = TASSEL__ACLK | TAIE;
	TA2CCR0 = 255;
}
