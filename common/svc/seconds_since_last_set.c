#include "svc.h"

static uint32_t seconds_since_last_set = 10000000;

void svc_seconds_since_last_set_reset() {
	seconds_since_last_set = 1;
}

void svc_seconds_since_last_set_process() {
	static uint8_t div;
	if(div == 0) {
		seconds_since_last_set++;
	}
	div = (div+1)%4;
}

uint32_t svc_seconds_since_last_set_get() {
	return seconds_since_last_set;
}
