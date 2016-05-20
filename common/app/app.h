#pragma once
#include <stdint.h>
#include "common/svc/svc.h"

struct app_u;
typedef struct app_u app_t;

typedef struct {
	void (*main)(uint8_t view, const app_t *app, svc_main_proc_event_t event);
	void (*enter)(uint8_t view, const app_t *app);
	void (*leave)(uint8_t view, const app_t *app);
} app_view_t;

#define APP_PRIV_COMMON uint8_t view_current;

typedef struct {
	APP_PRIV_COMMON
} app_priv_t;

struct app_u {
	char *name;
	app_priv_t *priv;
	void (*enter)(const app_t *app);
	void (*leave)(const app_t *app);
	app_view_t *views;
	uint8_t n_views;
};

extern const app_t *app_current;
extern uint8_t app_view_changed;

void app_launch(const app_t *app);
void app_exit(void);
void app_current_update(void);
void app_set_view(const app_t *app, uint8_t view);
