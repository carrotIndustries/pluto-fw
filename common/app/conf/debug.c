#include "app.h"


void app_app_conf_debug_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		INC_MOD(PRIV(app)->debug_item_current, HAL_DEBUG_N);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		DEC_MOD(PRIV(app)->debug_item_current, HAL_DEBUG_N);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_set_view(app, 0);
		PRIV(app)->debug_item_current = 0;
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
		PRIV(app)->debug_value = hal_debug_read(PRIV(app)->debug_item_current);
	}
	svc_lcd_puts(8, "db");
	svc_lcd_puti(6, 2, PRIV(app)->debug_item_current);
	svc_lcd_putix(0, 4, PRIV(app)->debug_value);
}
