#include <msp430.h>
#include "common/hal/hal.h"

/* SYSRSTIV is cleared after each read! */
static uint16_t read_reset_reason() {
	static uint16_t resetreason=0;
	uint16_t tmp = SYSRSTIV;

	if((resetreason != tmp) && tmp)
		resetreason = tmp;

	return resetreason;
}

uint16_t hal_debug_read(uint8_t index) {
	switch(index) {
		case 0 :
			return read_reset_reason();
			break;
		default :
			return 0;
	}
}
