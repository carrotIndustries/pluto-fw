#include "svc.h"

static int8_t adj_s = 0;

void svc_rtc_adj_sec(int8_t s) {
	if(ABS(s) < 59)
		adj_s = s;
}

int8_t svc_rtc_adj_get_sec(void) {
	return adj_s;
}

uint8_t svc_rtc_adj_get_pending(void) {
	return adj_s!=0;
}

void svc_rtc_adj_process() {
	if(adj_s!=0) {
		hal_rtc_timedate_t td;
		hal_rtc_get(&td);
		int8_t sec = td.s+adj_s;
		if(sec>=0 && sec <=59) { //can adj
			td.s = sec;
			hal_rtc_set_time(&td);
			adj_s = 0;
			svc_seconds_since_last_set_reset();
		}
	}
}
