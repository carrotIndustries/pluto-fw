#pragma once

#include <stdint.h>

#define BTN_ALARM 1 << 2
#define BTN_MODE 1 << 1
#define BTN_LIGHT 1
#define BTN_ALL (BTN_ALARM | BTN_MODE | BTN_LIGHT)

void button_init(void);
uint8_t get_button_press(uint8_t button_mask);
uint8_t get_button_rpt(uint8_t button_mask);
uint8_t get_button_short(uint8_t button_mask);
uint8_t get_button_long(uint8_t button_mask);
uint8_t get_button_common(uint8_t button_mask);
