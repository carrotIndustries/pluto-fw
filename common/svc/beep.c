#include "beep.h"
#include "melody.h"
#include "common/hal/hal.h"
#include "aux_timer.h"
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

static uint16_t SECTION_INFOMEM beep_key_freq = 2000;
static uint8_t  SECTION_INFOMEM beep_key_enable = 1;
static uint16_t beep_key_duration = 5;

void svc_beep_key(void) {
	svc_melody_stop();
	if(beep_key_enable) {
		svc_beep_timed(beep_key_freq, beep_key_duration);
	}
}

uint8_t SECTION_INFOMEM beep_hour_enable = 1;

void svc_beep_hour(void) {
	static uint8_t hour_last = 255;
	
	hal_rtc_timedate_t td;
	hal_rtc_get(&td);
	
	if((td.h != hour_last) && beep_hour_enable) {
		svc_beep_timed(beep_key_freq, 96);
		hour_last = td.h;
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
