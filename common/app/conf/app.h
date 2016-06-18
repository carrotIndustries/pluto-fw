#pragma once
#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	APP_PRIV_COMMON
	svc_menu_state_t st;
	uint8_t debug_item_current;
	uint16_t debug_value;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))
void app_app_conf_debug_main(uint8_t view, const app_t *app, svc_main_proc_event_t event);
