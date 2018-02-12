#include "app.h"

static void minmax_init(minmax_t *mm) {
	mm->min = INT16_MAX;
	mm->max = INT16_MIN;
}

static void minmax_update(minmax_t *mm, int16_t value) {
	if(value < mm->min) {
		mm->min = value;
	}
	if(value > mm->max) {
		mm->max = value;
	}
}

static int16_t minmax_get_center(minmax_t *mm) {
	return (((int32_t)(mm->max))+((int32_t)(mm->min)))/2;
}

static int16_t minmax_get_span(minmax_t *mm) {
	if(mm->max < mm->min) {
		return 0;
	}
	else {
		return mm->max - mm->min;
	}
}

static void axis_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	priv_t *p = PRIV(app_current);
	intptr_t axis = ((intptr_t)user_data)&3;
	uint8_t span = !!(((intptr_t)user_data)&4);
	int16_t (*fp)(minmax_t *mm) = 0;
	if(span) {
		fp = minmax_get_span;
		svc_lcd_putc(6, '1');
	}
	else {
		fp = minmax_get_center;
	}
	int16_t v = 0;
	switch(axis) {
		case 0:
			v = fp(&(p->cal_x));
		break;

		case 1:
			v = fp(&(p->cal_y));
		break;

		case 2:
			v = fp(&(p->cal_z));
		break;
	}
	svc_lcd_puti_signed(0, 6, CLAMP_ABS(v, 99999));
	svc_lcd_putc(7, 'x'+axis);
};

static const svc_menu_item_text_t menu_item_x_c = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)0
};
static const svc_menu_item_text_t menu_item_y_c = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)1
};
static const svc_menu_item_text_t menu_item_z_c = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)2
};

static const svc_menu_item_text_t menu_item_x_s = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)(0|4)
};
static const svc_menu_item_text_t menu_item_y_s = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)(1|4)
};
static const svc_menu_item_text_t menu_item_z_s = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "",
	.handler_draw = axis_draw,
	.user_data = (void*)(2|4)
};

static void menu_exit(void) {
	PRIV(app_current)->cal_menu_state.item_current = 0;
	app_set_view(app_current, 0);
}

static void menu_store(void *ud) {
	priv_t *pr = PRIV(app_current);
	svc_compass_cal_in_t ci = {
		.x0 = minmax_get_center(&(pr->cal_x)),
		.y0 = minmax_get_center(&(pr->cal_y)),
		.z0 = minmax_get_center(&(pr->cal_z)),

		.sx = minmax_get_span(&(pr->cal_x)),
		.sy = minmax_get_span(&(pr->cal_y)),
		.sz = minmax_get_span(&(pr->cal_z)),
	};
	svc_compass_set_cal(&ci);
}

static const svc_menu_item_text_t menu_item_store = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text = "store",
	.handler = menu_store
};


static const svc_menu_item_unknown_t *menu_items[] = {
	(void*)&menu_item_x_c,
	(void*)&menu_item_y_c,
	(void*)&menu_item_z_c,
	(void*)&menu_item_x_s,
	(void*)&menu_item_y_s,
	(void*)&menu_item_z_s,
	(void*)&menu_item_store
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = menu_exit,
	.header = "cc",
	.header_pos = 8
};

void app_app_compass_cal_main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	priv_t *p = PRIV(app);
	hal_compass_result_t r;
	if(!svc_compass_read(&r)) {
		minmax_update(&(p->cal_x), r.x);
		minmax_update(&(p->cal_y), r.y);
		minmax_update(&(p->cal_z), r.z);
	}
	svc_menu_run(&menu, &(PRIV(app)->cal_menu_state), event);
	return;
}

void app_app_compass_cal_enter(uint8_t view, const app_t *app) {
	priv_t *p = PRIV(app);
	minmax_init(&(p->cal_x));
	minmax_init(&(p->cal_y));
	minmax_init(&(p->cal_z));
}
