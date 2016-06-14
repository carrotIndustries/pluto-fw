#include <msp430.h>
#include "common/hal/hal.h"

uint16_t hal_debug_read(uint8_t index) {
	switch(index) {
		case 0 :
			return SYSRSTIV;
		break;
		
		default :
			return 0;
	}
}
