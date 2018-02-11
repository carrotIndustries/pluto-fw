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

static uint8_t beep_enable_get(void *ud) {
	return hal_beep_get_enabled();
}

static void beep_enable_set(uint8_t choice, void *ud) {
	hal_beep_set_enabled(choice);
}

static const svc_menu_item_choice_t menu_item_beep_enable = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " ben",
	.choice_pos = 4,
	.n_choices = 2,
	.choices = {
		"of",
		"on",
	},
	.handler_set = beep_enable_set,
	.handler_get = beep_enable_get
};

static int32_t hourbeep_quiet_get_interval(void *ud) {
	uint8_t s, e;

	svc_beep_hour_quiet_get_interval(&s, &e);

	return (int32_t)((s * 100) + e);
}

static void hourbeep_quiet_set_interval(uint8_t dig, int8_t dir, void *user_data) {
	uint8_t s, e;

	svc_beep_hour_quiet_get_interval(&s, &e);

	switch(dig) {
	case 3 :
	case 1 :
		dir *= 10;
	}

	switch(dig) {
	case 3 :
	case 2 :
		s = CLAMP(s + dir, 0, 23);
		break;

	case 1 :
	case 0 :
		e = CLAMP(e + dir, 0, 23);
		break;
	default :
		return;
	}

	svc_beep_hour_quiet_set_interval(s, e);
}

static const svc_menu_item_adj_t menu_item_hourbeep_quiet_interval = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "qi",
	.text = " hiq",
	.digits = 4,
	.handler_get = hourbeep_quiet_get_interval,
	.handler_set = hourbeep_quiet_set_interval,
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

static uint8_t lcd_contrast = 15;

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
	(void*)&menu_item_beep_enable,
	(void*)&menu_item_hourbeep,
	(void*)&menu_item_hourbeep_freq,
	(void*)&menu_item_hourbeep_duration,
	(void*)&menu_item_hourbeep_test,
	(void*)&menu_item_hourbeep_quiet,
	(void*)&menu_item_hourbeep_quiet_interval,
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
