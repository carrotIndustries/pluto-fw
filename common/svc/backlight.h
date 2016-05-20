#pragma once
#include <stdint.h>
#include "main.h"

void svc_backlight_brightness_set(uint8_t brightness);
uint8_t svc_backlight_brightness_get(void);

void svc_backlight_timeout_set(uint8_t timeout);
uint8_t svc_backlight_timeout_get(void);

void svc_backlight_process(svc_main_proc_event_t ev);
