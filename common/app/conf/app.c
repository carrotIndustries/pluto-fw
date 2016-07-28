#include "app.h"

static void conf_exit(void *ud) {
	PRIV(app_current)->st.item_current = 0;
	app_exit();
}

static uint8_t keybeep_get(void *ud) {
	return svc_beep_key_get_enable();
}

static void keybeep_set(uint8_t choice, void *ud) {
	svc_beep_key_set_enable(choice);
}

static const svc_menu_item_choice_t menu_item_keybeep = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " keb",
	.choice_pos = 4,
	.n_choices = 2,
	.choices = {
		"of",
		"on",
	},
	.handler_set = keybeep_set,
	.handler_get = keybeep_get
};

static uint8_t hourbeep_get(void *ud) {
	return svc_beep_hour_get_enable();
}

static void hourbeep_set(uint8_t choice, void *ud) {
	svc_beep_hour_set_enable(choice);
}

static const svc_menu_item_choice_t menu_item_hourbeep = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " hrb",
	.choice_pos = 4,
	.n_choices = 2,
	.choices = {
		"of",
		"on",
	},
	.handler_set = hourbeep_set,
	.handler_get = hourbeep_get
};

static int32_t keybeep_freq_get(void *ud) {
	return svc_beep_key_get_freq();
}

static void keybeep_freq_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_beep_key_get_freq();
	va = CLAMP(va+inc, 400, 9999);
	svc_beep_key_set_freq(va);
}


static const svc_menu_item_adj_t menu_item_keybeep_freq = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "kf",
	.text = " kef",
	.digits = 4,
	.handler_get = keybeep_freq_get,
	.handler_set = keybeep_freq_set,
};

static int32_t keybeep_duration_get(void *ud) {
	return svc_beep_key_get_duration();
}

static void keybeep_duration_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_beep_key_get_duration();
	va = CLAMP(va+inc, 1, 50);
	svc_beep_key_set_duration(va);
}


static const svc_menu_item_adj_t menu_item_keybeep_duration = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "kd",
	.text = " ked",
	.digits = 2,
	.handler_get = keybeep_duration_get,
	.handler_set = keybeep_duration_set,
};

static int32_t hourbeep_freq_get(void *ud) {
	return svc_beep_hour_get_freq();
}

static void hourbeep_freq_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_beep_hour_get_freq();
	va = CLAMP(va+inc, 400, 9999);
	svc_beep_hour_set_freq(va);
}

static const svc_menu_item_adj_t menu_item_hourbeep_freq = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "hf",
	.text = " hrf",
	.digits = 4,
	.handler_get = hourbeep_freq_get,
	.handler_set = hourbeep_freq_set,
};

static int32_t hourbeep_duration_get(void *ud) {
	return svc_beep_hour_get_duration();
}

static void hourbeep_duration_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_beep_hour_get_duration();
	va = CLAMP(va+inc, 1, 100);
	svc_beep_hour_set_duration(va);
}


static const svc_menu_item_adj_t menu_item_hourbeep_duration = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "hd",
	.text = " hrd",
	.digits = 2,
	.handler_get = hourbeep_duration_get,
	.handler_set = hourbeep_duration_set,
};

static void hourbeep_test(void *ud) {
	svc_beep_timed(svc_beep_hour_get_freq(), svc_beep_hour_get_duration());
}

static const svc_menu_item_text_t menu_item_hourbeep_test = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = " hrt",
	.handler = hourbeep_test
};

static uint8_t hourbeep_quiet_get(void *ud) {
	return svc_beep_hour_quiet_get_enable();
}

static void hourbeep_quiet_set(uint8_t choice, void *ud) {
	svc_beep_hour_quiet_set_enable(choice);
}

static const svc_menu_item_choice_t menu_item_hourbeep_quiet = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " hrq",
	.choice_pos = 4,
	.n_choices = 2,
	.choices = {
		"of",
		"on",
	},
	.handler_set = hourbeep_quiet_set,
	.handler_get = hourbeep_quiet_get
};

static int32_t hourbeep_quiet_get_time(uint8_t start) {
	svc_beep_hour_quiet_t bq;
	svc_beep_hour_quiet_get_time(start, &bq);
	return bq.m + (bq.h * 100);
}

static int32_t hourbeep_quiet_get_start_time(void *ud) {
	return hourbeep_quiet_get_time(0);
}

static int32_t hourbeep_quiet_get_stop_time(void *ud) {
	return hourbeep_quiet_get_time(1);
}

static void hourbeep_quiet_set_time(uint8_t start, int8_t dig, uint8_t dir) {
	svc_beep_hour_quiet_t bq;
	svc_beep_hour_quiet_get_time(start, &bq);
	switch (dig) {
	case 1:
		dir *= 10;
		break;
	}

	switch(dig) {
	case 3 :
	case 2 :
		ADD_MOD(bq.h, dir, 24);
		break;

	case 1 :
	case 0 :
		ADD_MOD(bq.m, dir, 60);
		break;
	default :
		return;
	}

	svc_beep_hour_quiet_set_time(start, bq.h, bq.m);
}

static void hourbeep_quiet_set_start_time(uint8_t dig, int8_t dir, void *ud) {
	hourbeep_quiet_set_time(0, dig, dir);
}

static void hourbeep_quiet_set_stop_time(uint8_t dig, int8_t dir, void *ud) {
	hourbeep_quiet_set_time(1, dig, dir);
}

static const svc_menu_item_adj_t menu_item_hourbeep_quiet_begin = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "qb",
	.text = " hqb",
	.digits = 4,
	.handler_get = hourbeep_quiet_get_start_time,
	.handler_set = hourbeep_quiet_set_start_time,
};

static const svc_menu_item_adj_t menu_item_hourbeep_quiet_end = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "qn",
	.text = " hqn",
	.digits = 4,
	.handler_get = hourbeep_quiet_get_stop_time,
	.handler_set = hourbeep_quiet_set_stop_time,
};

static int32_t backlight_timeout_get(void *ud) {
	return svc_backlight_timeout_get();
}

static void backlight_timeout_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_backlight_timeout_get();
	va = CLAMP(va+inc, 1, 10);
	svc_backlight_timeout_set(va);
}


static const svc_menu_item_adj_t menu_item_backlight_timeout = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "bt",
	.text = "bti",
	.digits = 2,
	.handler_get = backlight_timeout_get,
	.handler_set = backlight_timeout_set,
};

static int32_t backlight_brightness_get(void *ud) {
	return svc_backlight_brightness_get();
}

static void backlight_brightness_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_backlight_brightness_get();
	va = CLAMP(va+inc, 0, 10);
	svc_backlight_brightness_set(va);
}


static const svc_menu_item_adj_t menu_item_backlight_brightness = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "bb",
	.text = "bbri",
	.digits = 2,
	.handler_get = backlight_brightness_get,
	.handler_set = backlight_brightness_set,
};

static uint8_t lcd_contrast;

static int32_t lcd_contrast_get(void *ud) {
	return lcd_contrast;
}

static void lcd_contrast_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	lcd_contrast = CLAMP(lcd_contrast+inc, 0, 15);
	hal_lcd_set_contrast(lcd_contrast);
}


static const svc_menu_item_adj_t menu_item_lcd_contrast = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "lc",
	.text = " con",
	.digits = 2,
	.handler_get = lcd_contrast_get,
	.handler_set = lcd_contrast_set,
};

static const svc_menu_item_text_t menu_item_up = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "----up",
	.handler = conf_exit
};

static void debug_enter(void *ud) {
	app_set_view(app_current, 1);
}

static const svc_menu_item_text_t menu_item_debug = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "dbg",
	.handler = debug_enter
};

static const svc_menu_item_text_t *menu_items[] = {
	(void*)&menu_item_keybeep,
	(void*)&menu_item_keybeep_freq,
	(void*)&menu_item_keybeep_duration,
	(void*)&menu_item_hourbeep,
	(void*)&menu_item_hourbeep_freq,
	(void*)&menu_item_hourbeep_duration,
	(void*)&menu_item_hourbeep_test,
	(void*)&menu_item_hourbeep_quiet,
	(void*)&menu_item_hourbeep_quiet_begin,
	(void*)&menu_item_hourbeep_quiet_end,
	(void*)&menu_item_backlight_timeout,
	(void*)&menu_item_backlight_brightness,
	(void*)&menu_item_lcd_contrast,
	(void*)&menu_item_debug,
	(void*)&menu_item_up,
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.item_up = (void*)&menu_item_up,
	.header = "cf",
	.header_pos = 8
};


static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_menu_run(&menu, &(PRIV(app)->st), event);
}

static app_view_t views[] = {
	{
		.main = main
	},
	{
		.main = app_app_conf_debug_main,
	}
};

static priv_t priv = {0};

const app_t app_app_conf = {
	.n_views = 2,
	.priv = (app_priv_t*)(&priv),
	.views = views
};
