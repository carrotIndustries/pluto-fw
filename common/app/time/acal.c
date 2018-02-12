#include "app.h"

static void menu_exit(void) {
	app_set_view(app_current, 1);
}

#define MEAS_INVALID 127
static int8_t meas_delta = MEAS_INVALID;

static void cal_meas(void *ud) {
	hal_rtc_timedate_t td;
	hal_rtc_get(&td);
	if(td.s > 30) {
		meas_delta = td.s-60;
	}
	else {
		meas_delta = td.s;
	}
	if(ABS(meas_delta) > 20) {
		meas_delta = MEAS_INVALID;
	}
}

static void meas_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8, "me");
	if(meas_delta == MEAS_INVALID) {
		svc_lcd_puts(0, " err");
	}
	else {
		svc_lcd_puti_signed(1, 3, meas_delta);
	}
	hal_rtc_timedate_t td;
	hal_rtc_get(&td);
	svc_lcd_puti(4, 2, td.s);
}

static const svc_menu_item_text_t menu_item_meas = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text="",
	.handler_draw = meas_draw,
	.handler = cal_meas
};

static void ssls_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8, "sl");
	svc_lcd_puti(0, 6, svc_seconds_since_last_set_get()/100);
}

static const svc_menu_item_text_t menu_item_ssls = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text="",
	.handler_draw = ssls_draw,
};

#define PPM_OVER 32767

static int16_t get_ppm() { //actually, 10ppb
	int32_t x = (100000000L*meas_delta)/(int32_t)svc_seconds_since_last_set_get();
	if(ABS(x) >= 32767)
		x = PPM_OVER;
	return x;
}

static void ppm_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8, "pm");
	if(meas_delta == MEAS_INVALID) {
		svc_lcd_puts(0, " err");
		return;
	}
	int16_t ppm =  get_ppm();
	if(ppm == PPM_OVER) {
		svc_lcd_puts(0, "over");
		return;
	}
	if(ABS(ppm) > 24000) {
		svc_lcd_putc(7, 'e');
	}
	svc_lcd_puti_signed(0, 6, ppm);
}

static const svc_menu_item_text_t menu_item_ppm = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text="",
	.handler_draw = ppm_draw,
};

void store_cal(void *ud) {
	int16_t ppm = get_ppm();
	if(meas_delta == MEAS_INVALID || ABS(ppm) > 24000)
		return;

	int16_t real_ppm = ppm/100;
	if((ABS(ppm) % 100) > 50) {
		if(ppm > 0)
			real_ppm++;
		else
			real_ppm--;
	}
	int16_t cal = hal_rtc_cal_get();
	hal_rtc_cal_set(cal-real_ppm);
}

static const svc_menu_item_text_t menu_item_cal = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text=" calst",
	.handler = store_cal,
};

void do_adj(void *ud) {
	if(meas_delta == MEAS_INVALID)
		return;

	svc_rtc_adj_sec(-meas_delta);
}

static const svc_menu_item_text_t menu_item_adj = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text=" adj",
	.handler = do_adj,
};


static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_meas,
	(void*)&menu_item_ssls,
	(void*)&menu_item_ppm,
	(void*)&menu_item_cal,
	(void*)&menu_item_adj
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "ac",
	.header_pos = 8
};

void app_app_time_acal_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_menu_run(&menu, &(PRIV(app)->acal_state), event);
}

void app_app_time_acal_enter(uint8_t view, const app_t *app) {
	PRIV(app)->acal_state.item_current = 0;
}
