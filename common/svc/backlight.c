#include "backlight.h"
#include "common/hal/hal.h"
#include "platform.h"

static uint8_t SECTION_INFOMEM g_brightness = 10;
static uint8_t SECTION_INFOMEM g_timeout = 2;
static uint8_t timer;


void svc_backlight_brightness_set(uint8_t brightness) {
	g_brightness = brightness;
}
uint8_t svc_backlight_brightness_get(void) {
	return g_brightness;
}

void svc_backlight_timeout_set(uint8_t timeout) {
	g_timeout = timeout;
}

uint8_t svc_backlight_timeout_get(void) {
	return g_timeout;
}

void svc_backlight_process(svc_main_proc_event_t ev) {
	if(ev & SVC_MAIN_PROC_EVENT_KEY_UP_LONG) {
		timer = g_timeout*4;
		hal_backlight_set(g_brightness);
	}
	if((ev & (SVC_MAIN_PROC_EVENT_KEY_ANY | SVC_MAIN_PROC_EVENT_KEY_ANY_LONG)) && timer) {
		timer = g_timeout*4;
	}
	
	if(ev & SVC_MAIN_PROC_EVENT_TICK) { //decrement
		if(timer) {
			timer--;
		}
		else {
			hal_backlight_set(0);
		}
	}
}

void svc_backlight_start(void) {
	
}
