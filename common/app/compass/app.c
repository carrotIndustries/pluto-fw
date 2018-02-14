#include "app.h"

static void axis_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	intptr_t axis = (intptr_t)user_data;
	hal_compass_result_t r;
	if(svc_compass_read_cal(&r)) {
		svc_lcd_puts(1, "err");
	}
	else {
		int16_t v = 0;
		switch(axis) {
			case 0:
				v = r.x;
			break;

			case 1:
				v = r.y;
			break;

			case 2:
				v = r.z;
			break;
		}
		svc_lcd_puti_signed(0, 6, CLAMP_ABS(v, 99999));
	}
	svc_lcd_putc(7, 'x'+axis);
};

static const svc_menu_item_text_t menu_item_x = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)0
};
static const svc_menu_item_text_t menu_item_y = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)1
};
static const svc_menu_item_text_t menu_item_z = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)2
};

static void heading_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	hal_compass_result_t r;
	if(svc_compass_read_cal(&r)) {
		svc_lcd_puts(1, "err");
	}
	else {
		int16_t heading = atani(-r.y, -r.x);
		svc_lcd_puti(0, 4, CLAMP_ABS(heading, 9999));
	}
	svc_lcd_putc(7, 'h');
};

static const svc_menu_item_text_t menu_item_heading = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = heading_draw,
};

static void menu_exit(void) {
	PRIV(app_current)->main_menu_state.item_current = 0;
	//app_set_view(app_current, 0);
	app_exit();
}

static void menu_cal_enter(void *ud) {
	PRIV(app_current)->main_menu_state.item_current = 0;
	/* priv_t *p = PRIV(app_current); */
	app_set_view(app_current, 1);
}

static const svc_menu_item_text_t menu_item_cal = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = " cal",
	.handler = menu_cal_enter
};

static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_heading,
	(void*)&menu_item_x,
	(void*)&menu_item_y,
	(void*)&menu_item_z,
	(void*)&menu_item_cal
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "co",
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
		.main = app_app_compass_cal_main,
		.enter = app_app_compass_cal_enter
	}
};

const app_t app_app_compass = {
	.n_views = 2,
	.priv = (app_priv_t*)(&priv),
	.views = views
};
