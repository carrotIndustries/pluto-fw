#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	APP_PRIV_COMMON
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	svc_chro_t ch;
	svc_chro_get(0, &ch);
	svc_chro_convert(&ch);
	svc_chro_state_t st = svc_chro_get_state(0);
	if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
		switch(st) {
			case SVC_CHRO_STATE_RUN :
				svc_chro_hold(0);
				break;

			case SVC_CHRO_STATE_HOLD :
			case SVC_CHRO_STATE_STOP :
				svc_chro_start(0);
				break;
		}
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		switch(st) {
			case SVC_CHRO_STATE_RUN :
			case SVC_CHRO_STATE_HOLD :
				svc_chro_stop(0);
				break;

			case SVC_CHRO_STATE_STOP :
				svc_chro_reset(0);
				break;
		}
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		app_exit();
	}

	switch(st) {
		case SVC_CHRO_STATE_RUN :
			svc_lcd_puts(8, "ru");
			break;

		case SVC_CHRO_STATE_HOLD :
			svc_lcd_puts(8, "hl");
			break;

		case SVC_CHRO_STATE_STOP :
			svc_lcd_puts(8, "st");
			break;
	}
	svc_lcd_puti(0, 2, ch.min);
	svc_lcd_puti(2, 2, ch.sec);
	svc_lcd_puti(4, 2, ch.subsec);
	hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
	hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, st==SVC_CHRO_STATE_RUN);
}

static app_view_t view = {
	.main = main
};

static priv_t priv = {0};

const app_t app_app_chro = {
	.n_views = 1,
	.priv = (app_priv_t*)(&priv),
	.views = &view
};
