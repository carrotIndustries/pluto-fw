#include "app.h"
/*
static const svc_menu_item_adj_t menu_item_cal = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.header = "ca",
	.text = " cal",
	.digits = 4,
	.handler_get = cal_get,
	.handler_set = cal_set,
};*/

static void menu_exit(void) {
	PRIV(app_current)->edit_menu_state.item_current = 0;
	app_set_view(app_current, 0);
}

static int32_t time_get(void *ud) {
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
	return cd.s + cd.m*100L + cd.h*10000L;
}

static void time_set(uint8_t dig, int8_t dir, void *user_data) {
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);

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
			cd.h = CLAMP(cd.h+dir, 0, 23);
		break ;

		case 3 :
		case 2 :
			cd.m = CLAMP(cd.m+dir, 0, 59);
		break ;

		case 1 :
		case 0 :
			cd.s = CLAMP(cd.s+dir, 0, 59);
		break ;

		default :
			return;
	}
	svc_countdown_set_time(PRIV(app_current)->countdown_current, cd.h, cd.m, cd.s);
}

static void time_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8,"ti");
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
	svc_lcd_puti(0, 2, cd.h);
	svc_lcd_puti(2, 2, cd.m);
	svc_lcd_puti(4, 2, cd.s);
	hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
	svc_lcd_puti(6, 2, PRIV(app_current)->countdown_current);
};

static const svc_menu_item_adj_t menu_item_time = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.text = "",
	.header = "ti",
	.digits = 6,
	.handler_get = time_get,
	.handler_set = time_set,
	.handler_draw = time_draw,
};

static void start_stop_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8,"cd");
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
	svc_lcd_puti(6, 2, PRIV(app_current)->countdown_current);
	if(cd.state == SVC_COUNTDOWN_STATE_STOP) {
		svc_lcd_puts(0, "start");
	}
	else {
		svc_lcd_puts(0, "stop");
	}
}

static void start_stop(void *ud) {
	PRIV(app_current)->edit_menu_state.item_current = 0;
	app_set_view(app_current, 0);
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
	if(cd.state == SVC_COUNTDOWN_STATE_STOP) {
		svc_countdown_start(PRIV(app_current)->countdown_current);
	}
	else {
		svc_countdown_stop(PRIV(app_current)->countdown_current);
	}
}

static const svc_menu_item_text_t menu_item_start_stop = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler = start_stop,
	.handler_draw = start_stop_draw,
};

static void melody_set(uint8_t choice, void *ud) {
	svc_countdown_set_melody(PRIV(app_current)->countdown_current, choice);
}

static uint8_t melody_get(void *ud) {
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
	return cd.melody;
}

static void melody_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_countdown_t cd;
	svc_countdown_get(PRIV(app_current)->countdown_current, &cd);
	svc_lcd_putsn(4, 2, svc_melodies[cd.melody].title);
	svc_lcd_puti(6, 2, PRIV(app_current)->countdown_current);
}

static svc_menu_item_choice_t menu_item_melody = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " mel",
	.choice_pos = 4,
	.n_choices = 0,
	.choices = {""},
	.handler_set = melody_set,
	.handler_get = melody_get,
	.handler_draw = melody_draw,
};

static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_start_stop,
	(void*)&menu_item_time,
	(void*)&menu_item_melody
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "ce",
	.header_pos = 8
};

void app_app_countdown_edit_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	menu_item_melody.n_choices = svc_melodies_n;
	svc_menu_run(&menu, &(PRIV(app)->edit_menu_state), event);
}
