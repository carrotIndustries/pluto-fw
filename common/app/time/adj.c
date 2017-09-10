#include "app.h"

void app_app_time_adj_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	hal_rtc_timedate_t td;
	hal_rtc_get(&td);
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		if(td.h < 23) {
			td.h++;
			hal_rtc_set_time(&td);
		}
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		if(td.h > 0) {
			td.h--;
			hal_rtc_set_time(&td);
		}
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
		app_set_view(app_current, 1);
	}
	svc_lcd_puts(8, "ad");
	hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1); 
	hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, 1);
	svc_lcd_puti_fast(0, 2, td.h);
	svc_lcd_puti_fast(2, 2, td.m);
	svc_lcd_puti_fast(4, 2, td.s);
}
