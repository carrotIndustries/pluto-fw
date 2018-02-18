#include "app.h"
#include "platform.h"

static void menu_exit(void) {
	app_set_view(app_current, 0);
}

static void lang_set(uint8_t choice, void *ud) {
	*(PRIV(app_current)->lang) = choice;
}

static void base_set(uint8_t choice, void *ud) {
	*(PRIV(app_current)->base) = choice;
}

static uint8_t lang_get(void *ud) {
	return *(PRIV(app_current)->lang);
}

static uint8_t base_get(void *ud) {
	return *(PRIV(app_current)->base);
}

static hal_rtc_timedate_t tt = {.h=13, .m=37, .s=42, .year=2016, .month=6, .dom=3};

static int32_t time_get(void *ud) {
	return tt.s + tt.m*100L + tt.h*10000L;
}

static void time_set(uint8_t dig, int8_t dir, void *user_data) {
	switch(dig) {
		case 5:
		case 3:
		case 1:
			dir *= 10;
		break;
	}
	switch(dig) {
		case 5 :
		case 4 :
			tt.h = CLAMP(tt.h+dir, 0, 23);
		break ;

		case 3 :
		case 2 :
			tt.m = CLAMP(tt.m+dir, 0, 59);
		break ;

		case 1 :
		case 0 :
			tt.s = CLAMP(tt.s+dir, 0, 59);
		break ;

		default :
			return;
	}
}

static void adj_enter(void *ud) {
	hal_rtc_get(&tt);
}

static void adj_time_leave(void *ud) {
	hal_rtc_set_time(&tt);
	svc_seconds_since_last_set_reset();
}

static void adj_date_leave(void *ud) {
	hal_rtc_set_date(&tt);
}

static const svc_menu_item_adj_t menu_item_time = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.text = " time",
	.header = "ti",
	.digits = 6,
	.handler_get = time_get,
	.handler_set = time_set,
	.handler_enter = adj_enter,
	.handler_leave = adj_time_leave
};

static int32_t date_get(void *ud) {
	return tt.dom + tt.month*100L + tt.year*10000L;
}

static void date_set(uint8_t dig, int8_t di, void *user_data) {
	int16_t dir = di;
	switch(dig) {
		case 5:
		case 3:
		case 1:
			dir *= 10;
		break;

		case 6:
			dir *= 100;
		break;

		case 7:
			dir *= 1000;
		break;
	}
	switch(dig) {
		case 7 :
		case 6 :
		case 5 :
		case 4 :
			tt.year = CLAMP(tt.year+dir, 2000, 9999);
		break ;

		case 3 :
		case 2 :
			tt.month = CLAMP(tt.month+dir, 1, 12);
		break ;

		case 1 :
		case 0 :
			tt.dom = CLAMP(tt.dom+dir, 1, 31);
		break ;

		default :
			return;
	}
}

static const svc_menu_item_adj_t menu_item_date = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.text = " date",
	.header = "da",
	.digits = 8,
	.handler_get = date_get,
	.handler_set = date_set,
	.handler_enter = adj_enter,
	.handler_leave = adj_date_leave
};

static const svc_menu_item_choice_t menu_item_lang = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = "lang",
	.choice_pos = 4,
	.n_choices = 3,
	.choices = {
		"en",
		"de",
		"fr",
	},
	.handler_set = lang_set,
	.handler_get = lang_get
};

static const svc_menu_item_choice_t menu_item_base = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = "base",
	.choice_pos = 4,
	.n_choices = 3,
	.choices = {
		"de",
		"he",
		"bi",
	},
	.handler_set = base_set,
	.handler_get = base_get,
};

static int16_t cal_value = 0;
static uint8_t cal_sign;

static int32_t cal_get(void *ud) {
	return cal_value;
}

static void cal_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	cal_value += inc;
	cal_value = CLAMP(cal_value, 0, 240);
}

static uint8_t cal_sign_get(void *ud) {
	return !!cal_sign;
}

static void cal_leave(void *ud) {
	int16_t va = cal_value;
	if(cal_sign) {
		va *= -1;
	}
	hal_rtc_cal_set(va);
}

static void cal_sign_set(uint8_t va, void *ud) {
	cal_sign = va;
	cal_leave(0);
}

static void cal_enter(void *ud) {
	int16_t va = hal_rtc_cal_get();
	if(va >= 0) {
		cal_value = va;
		cal_sign = 0;
	}
	else {
		cal_value = -va;
		cal_sign = 1;
	}
}

static const svc_menu_item_adj_t menu_item_cal = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "ca",
	.text = " cal",
	.digits = 4,
	.handler_get = cal_get,
	.handler_set = cal_set,
	.handler_enter = cal_enter,
	.handler_leave = cal_leave,
};

static const svc_menu_item_choice_t menu_item_cal_sign = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = "scal",
	.choice_pos = 4,
	.n_choices = 2,
	.choices = {
		"po",
		"ne",
	},
	.handler_set = cal_sign_set,
	.handler_get = cal_sign_get,
};

static void acal_enter(void *ud) {
	app_set_view(app_current, 2);
}

static const svc_menu_item_text_t menu_item_acal = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "acal",
	.handler = acal_enter
};

static void adjh_enter(void *ud) {
	app_set_view(app_current, 3);
}

static const svc_menu_item_text_t menu_item_adj = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = " adj",
	.handler = adjh_enter
};

static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_base,
	(void*)&menu_item_acal,
	(void*)&menu_item_time,
	(void*)&menu_item_date,
	(void*)&menu_item_lang,
	(void*)&menu_item_adj,
	(void*)&menu_item_cal,
	(void*)&menu_item_cal_sign
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "cf",
	.header_pos = 8
};

static void main_conf(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_menu_run(&menu, &(PRIV(app)->conf_state), event);
}

static void menu_enter(uint8_t view, const app_t *app) {
	PRIV(app)->conf_state.item_current = 0;
}

/* this is needed for automatically go back to display time - adapt, if changed! */
const uint8_t app_time_display_view = 0;

static app_view_t views[] = {
	{
		.main = app_app_time_display_main,
		.enter = app_app_time_display_enter,
	},
	{
		.enter = menu_enter,
		.main = main_conf
	},
	{
		.enter = app_app_time_acal_enter,
		.main = app_app_time_acal_main,
	},
	{
		.main = app_app_time_adj_main,
	}
};

static svc_lang_t SECTION_INFOMEM lang;
static base_t SECTION_INFOMEM base;

static priv_t priv = {.view_current = 0, .base = &base, .lang = &lang};

const app_t app_app_time = {
	.n_views = ARRAY_SIZE(views),
	.priv = (app_priv_t*)(&priv),
	.views = views
};
