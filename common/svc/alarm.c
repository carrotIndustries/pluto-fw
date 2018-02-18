#include "alarm.h"
#include "melody.h"
#include "common/hal/hal.h"
#include "lcd.h"
#include "common/hal/lcd_segments.h"
#include <string.h>
#include "platform.h"
#define N_ALARMS 10

static svc_alarm_t SECTION_INFOMEM svc_alarms[N_ALARMS] = {
	{.h=19, .m=21, .days=0xff, .enable=1}

};
const uint8_t svc_alarms_n = N_ALARMS;

typedef enum {
	AF_ENABLED = (1<<0),
	AF_PENDING = (1<<1),
} alarm_flag_t;

static alarm_flag_t alarm_flags = 0;
static uint8_t alarm_pending;

void svc_alarm_get(uint8_t index, svc_alarm_t *out) {
	memcpy(out, &(svc_alarms[index]), sizeof(svc_alarm_t));
}

void svc_alarm_set_time(uint8_t index, uint8_t h, uint8_t m) {
	svc_alarms[index].h = h;
	svc_alarms[index].m = m;
}

void svc_alarm_set_enable(uint8_t index, uint8_t enable) {
	svc_alarms[index].enable = !!enable;
	alarm_flags &= ~AF_ENABLED;
	for(uint8_t i=0; i<svc_alarms_n; i++) {
		if(svc_alarms[i].enable) {
			alarm_flags |= AF_ENABLED;
		}
	}
}

void svc_alarm_set_day(uint8_t index, uint8_t day, uint8_t enable) {
	if(enable) {
		svc_alarms[index].days |= (1<<day);
	}
	else {
		svc_alarms[index].days &= ~(1<<day);
	}
}

void svc_alarm_set_melody(uint8_t index, uint8_t melody) {
	svc_alarms[index].melody = melody;
}

uint8_t svc_alarm_get_any_enabled(void) {
	return !!(alarm_flags & AF_ENABLED);
}

uint8_t svc_alarm_get_pending(void) {
	return !!(alarm_flags & AF_PENDING);
}

void svc_alarm_clear_pending(void) {
	alarm_flags &= ~AF_PENDING;
}

void svc_alarm_init(void) {
	alarm_flags &= ~AF_ENABLED;
	for(uint8_t i=0; i<svc_alarms_n; i++) {
		svc_alarms[i].melody = svc_default_melody_get();
		if(svc_alarms[i].enable) {
			alarm_flags |= AF_ENABLED;
		}
	}
}

static uint8_t svc_alarm_match(svc_alarm_t *al, hal_rtc_timedate_t *td) {
	return (al->enable) && (al->h == td->h) && (al->m == td->m) && (al->days & (1<<td->dow));
}

void svc_alarm_process(void) {
	static uint8_t min_last = 255;

	hal_rtc_timedate_t td;
	hal_rtc_get(&td);

	if(td.m != min_last) {
		for(uint8_t i=0; i<svc_alarms_n; i++) {
			if(svc_alarm_match(&(svc_alarms[i]), &td)) {
				svc_melody_play_repeat(svc_alarms[i].melody, svc_melody_alarm_repetitions_get());
				alarm_flags |= AF_PENDING;
				alarm_pending = i;
			}
		}
		min_last = td.m;
	}
}

void svc_alarm_draw_popup(void) {
	static uint8_t div;
	if(alarm_flags & AF_PENDING) {
		if(div < 4) {
			hal_lcd_seg_set(HAL_LCD_SEG_COLON, 0);
			svc_lcd_puts(0, "alrm");
			svc_lcd_puti(4, 2, alarm_pending);
			svc_lcd_force_redraw();
		}

	}
	div = (div+1)%8;
}
