#include "apps.h"

const app_t *app_current = &app_app_time;
uint8_t app_view_changed = 1;
static const app_t *app_current_next = &app_app_time;

void app_launch(const app_t *app) {
	app_current_next = app;
}

void app_exit(void) {
	app_launch(&app_app_launcher);
}

void app_current_update(void) {
	if(app_current_next != app_current) {
		app_view_changed = 1;
	}
	app_current = app_current_next;
}

void app_set_view(const app_t *app, uint8_t view) {
	app->priv->view_current = view;
	app_view_changed = 1;
}
