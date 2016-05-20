#include "compass.h"

static uint8_t timeout; //if 0, compass is off

uint8_t svc_compass_read(hal_compass_result_t *out) {
	if(timeout == 0) {
		hal_compass_set_power(1);
	}
	timeout = SVC_COMPASS_TIMEOUT*4;
	return hal_compass_read(out);
}

void svc_compass_process(void) {
	if(timeout) {
		timeout--;
	}
	else {
		hal_compass_set_power(0);
	}
}
