#pragma once
#include <stdint.h>

typedef enum {
	SVC_LCD_MAP_T_8 = 0,
	SVC_LCD_MAP_T_16 = 1,
} svc_lcd_map_type_t;

typedef struct {
	svc_lcd_map_type_t type;
	const void *map;
	uint8_t length;
} svc_lcd_map_t;

void svc_lcd_putc(uint8_t dig, char c);
void svc_lcd_puts(uint8_t dig, const char *s);
void svc_lcd_putsn(uint8_t dig, uint8_t len, const char *s);
void svc_lcd_puti(uint8_t dig, uint8_t len, uint32_t value);
void svc_lcd_puti_fast(uint8_t dig, uint8_t len, uint8_t value);
void svc_lcd_puti_signed(uint8_t dig, uint8_t len, int32_t value);
void svc_lcd_putix(uint8_t dig, uint8_t len, uint16_t value);
void svc_lcd_blink_disable(void);
void svc_lcd_blink_process(void);
void svc_lcd_force_redraw(void);
uint8_t svc_lcd_get_force_redraw(void);
