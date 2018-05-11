#include "app.h"

static uint16_t pin;

static int32_t pin_get(void *ud) {
	return pin;
}

static void pin_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	pin = CLAMP(((int16_t)pin)+inc, 0L, 9999L);
}

static void pin_leave(void *ud) {
	svc_otp_unlock(pin);
	pin=0;
}

static void draw_lock(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	if(svc_otp_is_locked()) {
		svc_lcd_putc(7, 'l');
	}
	else {
		svc_lcd_putc(7, 'u');
	}
}

static const svc_menu_item_adj_t menu_item_unlock = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.text = "uloc",
	.header = "pi",
	.digits = 4,
	.handler_get = pin_get,
	.handler_set = pin_set,
	.handler_leave = pin_leave,
	.handler_draw = draw_lock
};

static void menu_exit(void) {
	PRIV(app_current)->main_menu_state.item_current = 0;
	//app_set_view(app_current, 0);
	app_exit();
}

static void menu_lock(void *ud) {
	svc_otp_lock();
}

static const svc_menu_item_text_t menu_item_lock = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = " loc",
	.handler = menu_lock,
	.handler_draw = draw_lock
};

static void menu_enter_items(void *ud) {
	if(!svc_otp_is_locked()) {
		app_set_view(app_current, 1);
	}
}

static const svc_menu_item_text_t menu_item_items = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "items",
	.handler = menu_enter_items,
	.handler_draw = draw_lock
};

static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_unlock,
	(void*)&menu_item_items,
	(void*)&menu_item_lock
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "ot",
	.header_pos = 8
};

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_menu_run(&menu, &(PRIV(app)->main_menu_state), event);
}


static priv_t priv = {0};

static app_view_t views[] = {
	{
		.main = main
	},
	{
		.main = app_app_otp_items_main
	}
};

const app_t app_app_otp = {
	.n_views = 1,
	.priv = (app_priv_t*)(&priv),
	.views = views
};
