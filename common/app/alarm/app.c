#include "app.h"

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		INC_MOD(PRIV(app)->alarm_current, svc_alarms_n);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		DEC_MOD(PRIV(app)->alarm_current, svc_alarms_n);
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_exit();
	}

	svc_lcd_puts(8, "al");

	svc_alarm_t al;
	svc_alarm_get(PRIV(app)->alarm_current, &al);
	svc_lcd_puti(0, 2, al.h);
	svc_lcd_puti(2, 2, al.m);
	hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
	if(al.enable) {
		if(al.days) {
			svc_lcd_puts(4, "on");
		}
		else {
			svc_lcd_puts(4, "no");
		}
	}
	else {
		svc_lcd_puts(4, "of");
	}
	svc_lcd_puti(6, 2, PRIV(app)->alarm_current);
	if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
		app_set_view(app, 1);
	}
}

static app_view_t views[] = {
	{
		.main = main
	},
	{
		.main = app_app_alarm_edit_main,
	},
	{
		.main = app_app_alarm_days_main
	}
};


static priv_t priv = {0, 0};

const app_t app_app_alarm = {
	.n_views = ARRAY_SIZE(views),
	.priv = (app_priv_t*)(&priv),
	.views = views
};
