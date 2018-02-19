#include "countdown.h"
#include "melody.h"
#include "lcd.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"
#include <string.h>
#include "platform.h"

#define N_COUNTDOWNS 12

typedef struct {
	SVC_COUNTDOWN_COMMON
	int8_t sh;
	int8_t sm;
	int8_t ss;
} svc_countdown_priv_t;

static svc_countdown_priv_t SECTION_INFOMEM svc_countdowns[N_COUNTDOWNS] = {
	{.h=0, .m=0, .s=5}
};

#define NO_COUNTDOWN_PENDING 0xff

static uint8_t countdown_pending = NO_COUNTDOWN_PENDING;

static uint8_t countdowns_running = 0;

const uint8_t svc_countdowns_n = N_COUNTDOWNS;

void svc_countdown_init(void) {
	for(uint8_t i=0; i<svc_countdowns_n; i++) {
		svc_countdowns[i].melody = svc_default_melody_get();
	}
}

void svc_countdown_get(uint8_t index, svc_countdown_t *out) {
	memcpy(out, &(svc_countdowns[index]), sizeof(svc_countdown_t));
}

void svc_countdown_set_time(uint8_t index, uint8_t h, uint8_t m, uint8_t s) {
	svc_countdowns[index].h = h;
	svc_countdowns[index].sh = h;
	svc_countdowns[index].m = m;
	svc_countdowns[index].sm = m;
	svc_countdowns[index].s = s;
	svc_countdowns[index].ss = s;
}

static void _svc_countdown_stop(svc_countdown_priv_t *cd) {
	cd->state = SVC_COUNTDOWN_STATE_STOP;
	cd->h = cd->sh;
	cd->m = cd->sm;
	cd->s = cd->ss;
	if(countdowns_running) {
		countdowns_running--;
	}
}

void svc_countdown_stop(uint8_t index) {
	_svc_countdown_stop(&(svc_countdowns[index]));
}

void svc_countdown_start(uint8_t index) {
	countdowns_running++;
	svc_countdowns[index].state = SVC_COUNTDOWN_STATE_RUN;
}

static uint8_t svc_countdown_dec(svc_countdown_priv_t *cd) {
	if(cd->state != SVC_COUNTDOWN_STATE_RUN) {
		return 0;
	}
	cd->s--;
	if(cd->s == -1) {
		cd->s = 59;
		cd->m--;
		if(cd->m == -1) {
			cd->m = 59;
			cd->h--;
			if(cd->h == -1) {
				_svc_countdown_stop(cd);
				return 1;
			}
		}
	}
	return 0;
}

void svc_countdown_process(void) {
	if(countdowns_running > 0) {
		for(uint8_t i=0; i<svc_countdowns_n; i++) {
			if(svc_countdown_dec(&(svc_countdowns[i]))) {
				svc_melody_play_repeat(svc_countdowns[i].melody, svc_melody_alarm_repetitions_get());
				countdown_pending = i;
			}
		}
	}
}

void svc_countdown_draw_popup(void) {
	static uint8_t div;
	if(countdown_pending != NO_COUNTDOWN_PENDING) {
		if(div < 4) {
			hal_lcd_seg_set(HAL_LCD_SEG_COLON, 0);
			svc_lcd_puts(0, "ctdn");
			svc_lcd_puti(4, 2, countdown_pending);
			svc_lcd_force_redraw();
		}
	}
	div = (div+1)%8;
}

uint8_t svc_countdown_get_n_running(void) {
	return countdowns_running;
}

uint8_t svc_countdown_get_pending(void) {
	return countdown_pending != NO_COUNTDOWN_PENDING;
}

void svc_countdown_clear_pending(void) {
	countdown_pending = NO_COUNTDOWN_PENDING;
}

void svc_countdown_set_melody(uint8_t index, uint8_t melody) {
	svc_countdowns[index].melody = melody;
}
