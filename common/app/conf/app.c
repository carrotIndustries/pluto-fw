#include "app.h"

static void conf_exit(void) {
	PRIV(app_current)->st.item_current = 0;
	app_exit();
}

/* settings for keybeep ******************************************************/
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
	.handler_set = keybeep_freq_set
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
	.handler_set = keybeep_duration_set
};


/* general beep enable *******************************************************/
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


/* settings for hourbeep *****************************************************/
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
	.handler_set = hourbeep_freq_set
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
	.handler_set = hourbeep_duration_set
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
	.handler_set = hourbeep_quiet_set_interval
};


/* settings for backlight ****************************************************/
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
	.handler_set = backlight_timeout_set
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
	.handler_set = backlight_brightness_set
};


/* settings for lcd **********************************************************/
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
	.handler_set = lcd_contrast_set
};


/* default melody (which will always be selected first) **********************/
static uint8_t melody_default_get(void *ud) {
	return svc_default_melody_get();
}

static void melody_default_set(uint8_t choice, void *ud) {
	svc_default_melody_set(choice);

	for(uint8_t i=0; i<svc_alarms_n; i++) {
		svc_alarm_set_melody(i, choice);
	}
	for(uint8_t i=0; i<svc_countdowns_n; i++) {
		svc_countdown_set_melody(i, choice);
	}
}

static void melody_default_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_putsn(4, 2, svc_melodies[svc_default_melody_get()].title);
	svc_lcd_puti(6, 2, svc_default_melody_get());
}

static svc_menu_item_choice_t menu_item_default_melody = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " mel",
	.choice_pos = 4,
	.n_choices = 0,
	.choices = {""},
	.handler_set = melody_default_set,
	.handler_get = melody_default_get,
	.handler_draw = melody_default_draw
};


/* repetitions for alarms ****************************************************/
static int32_t alarm_repetitions_get(void *ud) {
	return svc_melody_alarm_repetitions_get();
}

static void alarm_repetitions_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	int16_t va = svc_melody_alarm_repetitions_get();
	svc_melody_alarm_repetitions_set(CLAMP(va+inc, 0, 200));
}

static const svc_menu_item_adj_t menu_item_alarm_repetitions = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "ar",
	.text = "arep",
	.digits = 3,
	.handler_get = alarm_repetitions_get,
	.handler_set = alarm_repetitions_set
};


/* idle time in seconds before going back to display time ********************/
static int32_t time_to_return_home_get(void *ud) {
	return svc_menu_timetohome_get();
}

static void time_to_return_home_set(uint8_t dig, int8_t dir, void *user_data) {
	if(!svc_menu_timetohome_get()) {
		svc_menu_timetohome_set(svc_menu_timetohome_min_get());
	}
	else {
		int16_t inc = dir*ipow(10, dig);
		int16_t va = svc_menu_timetohome_get();
		svc_menu_timetohome_set(CLAMP(va+inc, svc_menu_timetohome_min_get()-1, 200));
	}
}

static const svc_menu_item_adj_t menu_item_time_to_return_home = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "th",
	.text = " t2ho",
	.digits = 3,
	.handler_get = time_to_return_home_get,
	.handler_set = time_to_return_home_set
};


/* debug view ****************************************************************/
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
	(void*)&menu_item_default_melody,
	(void*)&menu_item_alarm_repetitions,
	(void*)&menu_item_time_to_return_home,
	(void*)&menu_item_debug
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = conf_exit,
	.header = "cf",
	.header_pos = 8
};

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	menu_item_default_melody.n_choices = svc_melodies_n;
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
