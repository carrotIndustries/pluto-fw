#pragma once
#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	APP_PRIV_COMMON
	svc_menu_state_t main_menu_state;
	uint8_t item_current;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))

void app_app_otp_items_main(uint8_t view, const app_t *app, svc_main_proc_event_t event);
