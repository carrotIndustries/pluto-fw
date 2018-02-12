#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	APP_PRIV_COMMON
	svc_menu_state_t st;
	uint16_t distance;
	uint8_t debug_item_current;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))

static void speed_exit(void) {
	PRIV(app_current)->st.item_current = 0;
	app_exit();
}

static void distance_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8,"di");
	svc_lcd_puti(0, 4, PRIV(app_current)->distance);
};

static int32_t distance_get(void *ud) {
	return PRIV(app_current)->distance;
}

static void distance_set(uint8_t dig, int8_t dir, void *user_data) {
	int16_t inc = dir*ipow(10, dig);
	PRIV(app_current)->distance = CLAMP(PRIV(app_current)->distance+inc, 0, 9999);
}

static const svc_menu_item_adj_t menu_item_distance = {
	.type = SVC_MENU_ITEM_T_ADJ,
	.text = "",
	.header = "di",
	.digits = 4,
	.handler_get =  distance_get,
	.handler_set =  distance_set,
	.handler_draw = distance_draw,
};

static void time_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8, "ti");
	svc_chro_t ch;
	svc_chro_get(1, &ch);
	svc_chro_state_t st = svc_chro_get_state(1);
	svc_lcd_puti(0, 2, ch.min);
	svc_lcd_puti(2, 2, ch.sec);
	uint16_t ss = (ch.subsec*100)/128;
	svc_lcd_puti(4, 2, ss);
	hal_lcd_seg_set(HAL_LCD_SEG_COLON, 1);
	hal_lcd_seg_set_blink(HAL_LCD_SEG_COLON, st==SVC_CHRO_STATE_RUN);
};

static void time_startstop(void *ud) {
	svc_chro_state_t st = svc_chro_get_state(1);
	if(st == SVC_CHRO_STATE_RUN) {
		svc_chro_stop(1);
	}
	else if(st == SVC_CHRO_STATE_STOP) {
		svc_chro_reset(1);
		svc_chro_start(1);
	}
}

static const svc_menu_item_text_t menu_item_time = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text="",
	.handler_draw = time_draw,
	.handler = time_startstop,
};

static void speed_draw(svc_menu_state_t *state, svc_menu_item_unknown_t *item, void *user_data) {
	svc_lcd_puts(8, "km");
	svc_chro_t ch;
	svc_chro_get(1, &ch);
	svc_chro_convert(&ch);
	uint16_t ss = svc_chro_get_subsec_total(&ch);
	uint32_t speed = 0;
	if(ss > 0) {
		speed = (PRIV(app_current)->distance*36000L)/ss;
	}
	svc_lcd_puti(0, 6, speed);
}

static const svc_menu_item_text_t menu_item_kmh = {
	.type = SVC_MENU_ITEM_T_TEXT,
	.text="",
	.handler_draw = speed_draw,
	.handler = time_startstop
};

static const svc_menu_item_text_t *menu_items[] = {
	(void*)&menu_item_distance,
	(void*)&menu_item_time,
	(void*)&menu_item_kmh
};

static const svc_menu_t menu = {
	.n_items = ARRAY_SIZE(menu_items),
	.items = (void*)menu_items,
	.handler_exit = speed_exit,
	.header = "s ",
	.header_pos = 8
};

static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	svc_menu_run(&menu, &(PRIV(app)->st), event);
}

static app_view_t views[] = {
	{
		.main = main
	},
};

static priv_t priv = {.distance = 200};

const app_t app_app_speed = {
	.n_views = 1,
	.priv = (app_priv_t*)(&priv),
	.views = views
};
