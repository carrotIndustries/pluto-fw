#include "app.h"
#include "../../svc/svc.h"

void app_app_time_display_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_rtc_timedate_t td;
	hal_rtc_get(&td);
	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_set_view(app, 1);
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		PRIV(app)->display_date = !PRIV(app)->display_date;
	}
	else if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		app_exit();
	}
	if(PRIV(app)->display_date) {
		hal_lcd_seg_set(HAL_LCD_SEG_COLON, 0); 
		svc_lcd_puti(0, 4, td.year);
		svc_lcd_puti(4, 2, td.month);
		svc_lcd_puti(6, 2, td.dom);
	}
	else {
		switch(*(PRIV(app)->base)) {
			case BASE_DEC :
				hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1); 
				hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, 1); 
				svc_lcd_puti_fast(0, 2, td.h);
				svc_lcd_puti_fast(2, 2, td.m);
				svc_lcd_puti_fast(4, 2, td.s);
				svc_lcd_puti_fast(6, 2, td.dom);
			break;
			
			case BASE_HEX :
				hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1); 
				hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, 1); 
				svc_lcd_putix(0, 2, td.h);
				svc_lcd_putix(2, 2, td.m);
				svc_lcd_putix(4, 2, td.s);
				svc_lcd_putix(6, 2, td.dom);
			break;
			
			case BASE_BIN :
				hal_lcd_seg_set(HAL_LCD_SEG(1, 6), 1);
				hal_lcd_seg_set(HAL_LCD_SEG(2, 6), 1);
				hal_lcd_seg_set(HAL_LCD_SEG(3, 6), 1);
				hal_lcd_seg_set(HAL_LCD_SEG(3, 1), td.h&(1<<0));
				hal_lcd_seg_set(HAL_LCD_SEG(3, 5), td.h&(1<<1));
				hal_lcd_seg_set(HAL_LCD_SEG(2, 1), td.h&(1<<2));
				hal_lcd_seg_set(HAL_LCD_SEG(2, 5), td.h&(1<<3));
				hal_lcd_seg_set(HAL_LCD_SEG(1, 1), td.h&(1<<4));
				
				hal_lcd_seg_set(HAL_LCD_SEG(3, 2), td.m&(1<<0));
				hal_lcd_seg_set(HAL_LCD_SEG(3, 4), td.m&(1<<1));
				hal_lcd_seg_set(HAL_LCD_SEG(2, 2), td.m&(1<<2));
				hal_lcd_seg_set(HAL_LCD_SEG(2, 4), td.m&(1<<3));
				hal_lcd_seg_set(HAL_LCD_SEG(1, 2), td.m&(1<<4));
				hal_lcd_seg_set(HAL_LCD_SEG(1, 4), td.m&(1<<5));
				
				hal_lcd_seg_set(HAL_LCD_SEG(5, 0), td.s>(60/8));
				hal_lcd_seg_set(HAL_LCD_SEG(5, 1), td.s>((60/8)*2));
				hal_lcd_seg_set(HAL_LCD_SEG(5, 2), td.s>((60/8)*3));
				hal_lcd_seg_set(HAL_LCD_SEG(5, 3), td.s>((60/8)*4));
				hal_lcd_seg_set(HAL_LCD_SEG(4, 3), td.s>((60/8)*5));
				hal_lcd_seg_set(HAL_LCD_SEG(4, 4), td.s>((60/8)*6));
				hal_lcd_seg_set(HAL_LCD_SEG(4, 5), td.s>((60/8)*7));
				hal_lcd_seg_set(HAL_LCD_SEG(4, 0), td.s>((60/8)*8));
				
				svc_lcd_puti_fast(6, 2, td.dom);
			break;
			
		}
	}
	svc_lcd_puts(8, svc_dow_to_string(td.dow, *(PRIV(app)->lang)));
}
