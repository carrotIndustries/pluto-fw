#include "beep.h"
#include "melody.h"
#include "common/hal/hal.h"
#include "aux_timer.h"
#include <string.h>
#include "platform.h"

static uint16_t beep_freq;
static uint16_t beep_duration;
static uint8_t beep_start;

void svc_beep_timed(uint16_t freq, uint16_t duration) {
	svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_BEEP, 1);
	beep_start = 1;
	beep_freq = freq;
	beep_duration = duration;
}

void svc_aux_timer_beep_handler(void) {
	if(beep_duration) {
		if(beep_start) {
			hal_beep(beep_freq);
			beep_start = 0;
		}
		beep_duration--;
	}
	else {
		hal_beep(0);
		svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_BEEP, 0);
	}
}

static uint16_t SECTION_INFOMEM beep_key_freq = 550;
static uint8_t  SECTION_INFOMEM beep_key_enable = 1;
static uint16_t SECTION_INFOMEM beep_key_duration = 5;

void svc_beep_key(void) {
	svc_melody_stop();
	if(beep_key_enable) {
		svc_beep_timed(beep_key_freq, beep_key_duration);
	}
}

static uint16_t SECTION_INFOMEM beep_hour_freq = 2000;
static uint8_t  SECTION_INFOMEM beep_hour_enable = 1;
static uint16_t SECTION_INFOMEM beep_hour_duration = 10;
static uint8_t  SECTION_INFOMEM beep_hour_quiet_enable = 0;
static svc_beep_hour_quiet_t beep_hour_quiet[2] = {
	{.h=21, .m=59},
	{.h=06, .m=59}
};
static uint8_t beep_hour_muted = 0;

void svc_beep_hour(void) {
	static uint8_t hour_last = 255;

	hal_rtc_timedate_t td;
	hal_rtc_get(&td);

	if (td.h == hour_last)
		return;

	hour_last = td.h;

	if (beep_hour_quiet_enable) {
		if ((td.h == beep_hour_quiet[0].h) &&
			(td.m == beep_hour_quiet[0].m))
			beep_hour_muted = 1;
		if ((td.h == beep_hour_quiet[1].h) &&
			(td.m == beep_hour_quiet[1].m))
			beep_hour_muted = 0;
	}

	if (beep_hour_quiet_enable && beep_hour_muted)
		return;

	if (beep_hour_enable) {
		svc_beep_timed(beep_hour_freq, beep_hour_duration);
	}
}

uint16_t svc_beep_key_get_freq(void) {
	return beep_key_freq;
}

uint16_t svc_beep_key_get_duration(void) {
	return beep_key_duration;
}

void svc_beep_key_set_freq(uint16_t f) {
	beep_key_freq = f;
}

void svc_beep_key_set_duration(uint16_t d) {
	beep_key_duration = d;
}

void svc_beep_key_set_enable(uint8_t e) {
	beep_key_enable = !!e;
}

uint8_t svc_beep_key_get_enable(void) {
	return beep_key_enable;
}

void svc_beep_hour_set_enable(uint8_t e) {
	beep_hour_enable = !!e;
}

uint8_t svc_beep_hour_get_enable(void) {
	return beep_hour_enable;
}

uint16_t svc_beep_hour_get_freq(void) {
	return beep_hour_freq;
}

uint16_t svc_beep_hour_get_duration(void) {
	return beep_hour_duration;
}

void svc_beep_hour_set_freq(uint16_t f) {
	beep_hour_freq = f;
}

void svc_beep_hour_set_duration(uint16_t d) {
	beep_hour_duration = d;
}

uint8_t svc_beep_hour_quiet_get_enable(void) {
	return beep_hour_quiet_enable;
}

void svc_beep_hour_quiet_set_enable(uint8_t e) {
	beep_hour_quiet_enable = !!e;
}

void svc_beep_hour_quiet_get_time(uint8_t start, svc_beep_hour_quiet_t *out) {
	memcpy(out, &(beep_hour_quiet[!!start]), sizeof(svc_beep_hour_quiet_t));
}

void svc_beep_hour_quiet_set_time(uint8_t start, uint8_t h, uint8_t m) {
	hal_rtc_timedate_t td;
	start = !!start;

	beep_hour_quiet[start].h = h;
	beep_hour_quiet[start].m = m;

	hal_rtc_get(&td);

	if ((td.h >= h) && (td.m >= m))
		beep_hour_muted = start;
}
