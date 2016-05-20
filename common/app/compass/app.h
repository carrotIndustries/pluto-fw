#pragma once
#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	int16_t min;
	int16_t max;
} minmax_t;

typedef struct {
	APP_PRIV_COMMON
	svc_menu_state_t main_menu_state;
	svc_menu_state_t cal_menu_state;
	minmax_t cal_x;
	minmax_t cal_y;
	minmax_t cal_z;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))

void app_app_compass_cal_main(uint8_t view, const app_t *app, svc_main_proc_event_t event);
void app_app_compass_cal_enter(uint8_t view, const app_t *app);
