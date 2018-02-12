#include "app.h"

void app_app_alarm_days_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);

	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		INC_MOD(PRIV(app)->day_current, 7);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		DEC_MOD(PRIV(app)->day_current, 7);
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_set_view(app, 1);
	}

	svc_lcd_puts(8, "da");
	svc_lcd_puti(6, 2, PRIV(app_current)->alarm_current);

	svc_lcd_puts(4, svc_dow_to_string(PRIV(app)->day_current, SVC_LANG_EN));
	if(al.days & (1<<(PRIV(app)->day_current))) {
		svc_lcd_puts(2, "on");
	}
	else {
		svc_lcd_puts(2, "of");
	}
	if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
		svc_alarm_set_day(PRIV(app_current)->alarm_current, PRIV(app_current)->day_current, !(al.days & (1<<(PRIV(app)->day_current))));
	}
}
