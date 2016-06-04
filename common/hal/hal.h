#pragma once
#include <stdint.h>
#include <stdbool.h>

#define HAL_LCD_SEG(dig, seg) ((uint8_t)((((dig)&0xf)<<4)|((seg))))

void hal_lcd_seg_set(uint8_t seg, bool state);
void hal_lcd_seg_set_blink(uint8_t seg, bool state);
void hal_lcd_dig_set(uint8_t dig, uint16_t segments);
void hal_lcd_dig_set_blink(uint8_t dig, uint8_t blink);
void hal_lcd_dig_set_blink_mask(uint16_t mask);
void hal_lcd_clear(void);
void hal_lcd_update(void);

typedef enum {
	HAL_LCD_MODE_IMMEDIATE,
	HAL_LCD_MODE_BUFFERED
} hal_lcd_mode_t;
void hal_lcd_set_mode(hal_lcd_mode_t mode);

void hal_lcd_set_blink(uint8_t st);

void hal_backlight_set(uint8_t bright);
void hal_beep(uint16_t freq);

typedef struct {
	uint8_t h;
	uint8_t m;
	uint8_t s;
	
	uint8_t dow;
	uint8_t dom;
	uint8_t month;
	uint16_t year;
} hal_rtc_timedate_t;

void hal_rtc_get(hal_rtc_timedate_t *result);
void hal_rtc_set_time(hal_rtc_timedate_t *time);
void hal_rtc_set_date(hal_rtc_timedate_t *date);
void hal_rtc_cal_set(int16_t cal);
int16_t hal_rtc_cal_get(void);
void hal_aux_timer_set(uint8_t running);

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} hal_compass_result_t;

void hal_compass_set_power(uint8_t on);
uint8_t hal_compass_read(hal_compass_result_t *out);

void hal_aes_set_key(uint8_t *key);
void hal_aes_clear_key(void);
void hal_aes_init(void);
void hal_aes_decrypt(uint8_t *dest, uint8_t *src);
void hal_sha1(const uint8_t *input, uint16_t len, uint8_t *output);
