#include "app.h"

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		INC_MOD(PRIV(app)->countdown_current, svc_countdowns_n+1);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		DEC_MOD(PRIV(app)->countdown_current, svc_countdowns_n+1);
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_exit();
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN_LONG) {
		svc_countdown_t cd;
		svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
		if(cd.state == SVC_COUNTDOWN_STATE_STOP) {
			svc_countdown_start(PRIV(app_current)->countdown_current);
		}
		else {
			svc_countdown_stop(PRIV(app_current)->countdown_current);
		}
	}
	if(PRIV(app)->countdown_current == svc_countdowns_n) {
		svc_lcd_puts(8, "cd");
		svc_lcd_puts(0, "----up");
		if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
			PRIV(app)->countdown_current = 0;
			app_exit();
		}
	}
	else {
		svc_countdown_t cd;
		svc_countdown_get(PRIV(app)->countdown_current, &cd);
		svc_lcd_puti(0, 2, cd.h);
		svc_lcd_puti(2, 2, cd.m);
		svc_lcd_puti(4, 2, cd.s);
		hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
		hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, cd.state == SVC_COUNTDOWN_STATE_RUN);
		if(cd.state == SVC_COUNTDOWN_STATE_RUN) {
			svc_lcd_puts(8, "ru");
		}
		else {
			svc_lcd_puts(8, "st");
		}
		svc_lcd_puti(6, 2, PRIV(app)->countdown_current);
		if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
			app_set_view(app, 1);
		}
	}
	
}

static app_view_t views[] = {
	{
		.main = main
	},
	{
		.main = app_app_countdown_edit_main,
	},
};


static priv_t priv = {0, 0};

const app_t app_app_countdown = {
	.n_views = ARRAY_SIZE(views),
	.priv = (app_priv_t*)(&priv),
	.views = views
};
