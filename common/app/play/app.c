#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
	APP_PRIV_COMMON
	uint8_t item_current;
} priv_t;

#define PRIV(a) ((priv_t*)((a)->priv))


static void main(uint8_t view, const app_t *app, svc_main_proc_event_t event) {
	hal_lcd_clear();
	if(event & SVC_MAIN_PROC_EVENT_KEY_UP) {
		INC_MOD(PRIV(app)->item_current, svc_melodies_n+1);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_DOWN) {
		DEC_MOD(PRIV(app)->item_current, svc_melodies_n+1);
	}
	else if (event & SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG) {
		app_exit();
	}
	svc_lcd_puts(8, "pl");
	if(PRIV(app)->item_current == svc_melodies_n) {
		svc_lcd_puts(0, "----up");
		if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
			PRIV(app)->item_current = 0;
			app_exit();
		}
	}
	else {
		svc_lcd_puts(0, " pla");
		svc_lcd_puti(4, 2, PRIV(app)->item_current);
		if(event & SVC_MAIN_PROC_EVENT_KEY_ENTER) {
			svc_melody_play(PRIV(app)->item_current);
		}
	}
}

static app_view_t view = {
	.main = main
};

static priv_t priv = {0, 0};

const app_t app_app_play = {
	.n_views = 1,
	.priv = (app_priv_t*)(&priv),
	.views = &view
};
