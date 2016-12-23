#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	APP_PRIV_COMMON
	svc_menu_state_t st;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))

static void launch(void *papp) {
	app_t *app = papp;
	app_launch(app);
}

static const svc_menu_item_text_t menu_item0 = {
	.text = " time",
	.handler = launch,
	.user_data = (void *)&app_app_time,
};
static const svc_menu_item_text_t menu_item1 = {
	.text = "chro",
	.handler = launch,
	.user_data = (void *)&app_app_chro,
};
static const svc_menu_item_text_t menu_item2 = {
	.text = "conf",
	.handler = launch,
	.user_data = (void *)&app_app_conf,
};
static const svc_menu_item_text_t menu_item3 = {
	.text = " alarm",
	.handler = launch,
	.user_data = (void *)&app_app_alarm,
};
static const svc_menu_item_text_t menu_item4 = {
	.text = "ctdn",
	.handler = launch,
	.user_data = (void *)&app_app_countdown,
};
static const svc_menu_item_text_t menu_item5 = {
	.text = " play",
	.handler = launch,
	.user_data = (void *)&app_app_play,
};
static const svc_menu_item_text_t menu_item6 = {
	.text = " compa",
	.handler = launch,
	.user_data = (void *)&app_app_compass,
};
static const svc_menu_item_text_t menu_item7 = {
	.text = "   otp",
	.handler = launch,
	.user_data = (void *)&app_app_otp,
};
static const svc_menu_item_text_t menu_item8 = {
	.text = "speed",
	.handler = launch,
	.user_data = (void *)&app_app_speed,
};

static const svc_menu_item_text_t *menu_items[] = {
	(void*)&menu_item0,
	(void*)&menu_item1,
	(void*)&menu_item2,
	(void*)&menu_item3,
	(void*)&menu_item4,
	(void*)&menu_item5,
	(void*)&menu_item6,
	(void*)&menu_item7,
	(void*)&menu_item8,
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.item_up = (void*)&menu_item0,
	.header = "la",
	.header_pos = 8
};


static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_menu_run(&menu, &(PRIV(app)->st), event);
}

static app_view_t view = {
	.main = main
};

static priv_t priv = {0};

const app_t app_app_launcher = {
	.n_views = 1,
	.priv = (app_priv_t*)(&priv),
	.views = &view
};
