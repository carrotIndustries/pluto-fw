#include "app.h"

void app_app_otp_items_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		INC_MOD(PRIV(app)->item_current, svc_otp_n_items);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		DEC_MOD(PRIV(app)->item_current, svc_otp_n_items);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_set_view(app_current, 0);
	}

	svc_lcd_puti(6, 2, svc_otp_get_time_remaining());
	char label[3];
	svc_otp_get_label(PRIV(app)->item_current, label);
	if(label[0]==0) {
		svc_lcd_puts(0, "------");
		svc_lcd_puts(8, "--");
	}
	else {
		svc_lcd_puts(8, label);
		int32_t t = svc_otp_get_token(PRIV(app)->item_current);
		if(t >= 0) {
			svc_lcd_puti(0, 6, t);
		}
		else {
			svc_lcd_puts(0, "------");
		}
	}
}
