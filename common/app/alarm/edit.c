#include "app.h"

static void menu_exit(void) {
	PRIV(app_current)->edit_menu_state.item_current = 0;
	app_set_view(app_current, 0);
}

static int32_t time_get(void *ud) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	return al.m + al.h*100;
}

static void time_set(uint8_t dig, int8_t dir, void *user_data) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	switch(dig) {
		case 3:
		case 1:
			dir *= 10;
		break;
	}
	switch(dig) {
		case 3 :
		case 2 :
			al.h = CLAMP(al.h+dir, 0, 23);
		break ;

		case 1 :
		case 0 :
			al.m = CLAMP(al.m+dir, 0, 59);
		break ;

		default :
			return;
	}
	svc_alarm_set_time(PRIV(app_current)->alarm_current, al.h, al.m);
}

static void time_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8,"ti");
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	svc_lcd_puti(0, 2, al.h);
	svc_lcd_puti(2, 2, al.m);
	hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
	svc_lcd_puti(6, 2, PRIV(app_current)->alarm_current);
};

static void draw_current(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puti(6, 2, PRIV(app_current)->alarm_current);
}

static const svc_menu_item_adj_t menu_item_time = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.text = "",
	.header = "ti",
	.digits = 4,
	.handler_get = time_get,
	.handler_set = time_set,
	.handler_draw = time_draw,
};

static uint8_t enable_get(void *ud) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	return al.enable;
}

static void enable_set(uint8_t choice, void *ud) {
	svc_alarm_set_enable(PRIV(app_current)->alarm_current, choice);
}

static const svc_menu_item_choice_t menu_item_enable = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = "  en",
	.choice_pos = 4,
	.n_choices = 2,
	.choices = {
		"of",
		"on",
	},
	.handler_set = enable_set,
	.handler_get = enable_get,
	.handler_draw = draw_current,
};

static void days_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	svc_lcd_puti(6, 2, PRIV(app_current)->alarm_current);
	svc_lcd_puts(8, "da");
	//mo
	hal_lcd_seg_set(HAL_LCD_SEG(0, 4), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(0, 5), al.days & (1<<0));

	//tu
	hal_lcd_seg_set(HAL_LCD_SEG(0, 2), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(0, 1), al.days & (1<<1));

	//we
	hal_lcd_seg_set(HAL_LCD_SEG(1, 4), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(1, 5), al.days & (1<<2));

	//th
	hal_lcd_seg_set(HAL_LCD_SEG(1, 2), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(1, 1), al.days & (1<<3));

	//fr
	hal_lcd_seg_set(HAL_LCD_SEG(2, 4), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(2, 5), al.days & (1<<4));

	//sa
	hal_lcd_seg_set(HAL_LCD_SEG(2, 2), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(2, 1), al.days & (1<<5));

	//su
	hal_lcd_seg_set(HAL_LCD_SEG(3, 4), 1);
	hal_lcd_seg_set(HAL_LCD_SEG(3, 5), al.days & (1<<6));
}

static void days_enter(void *ud) {
	app_set_view(app_current, 2);
}

static const svc_menu_item_text_t menu_item_days = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler = days_enter,
	.handler_draw = days_draw
};

static uint8_t melody_get(void *ud) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	return al.melody;
}

static void melody_set(uint8_t choice, void *ud) {
	svc_alarm_set_melody(PRIV(app_current)->alarm_current, choice);
}

static void melody_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_alarm_t al;
	svc_alarm_get(PRIV(app_current)->alarm_current, &al);
	svc_lcd_putsn(4, 2, svc_melodies[al.melody].title);
	svc_lcd_puti(6, 2, PRIV(app_current)->alarm_current);
}

static svc_menu_item_choice_t menu_item_melody = {
	.type = SVC_MENU_ITEM_T_CHOICE,
	.text = " mel",
	.choice_pos = 4,
	.n_choices = 0,
	.choices = {""},
	.handler_set = melody_set,
	.handler_get = melody_get,
	.handler_draw = melody_draw
};

static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_time,
	(void*)&menu_item_enable,
	(void*)&menu_item_days,
	(void*)&menu_item_melody
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "ae",
	.header_pos = 8
};

void app_app_alarm_edit_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	menu_item_melody.n_choices = svc_melodies_n;
	svc_menu_run(&menu, &(PRIV(app)->edit_menu_state), event);
}
