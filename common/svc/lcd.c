#include "svc.h"
#include "maps.h"
#include "common/hal/hal.h"
#include <ctype.h>

static const svc_lcd_map_t *digit_maps[] = {
	//big digits
	&svc_lcd_map_ad,
	&svc_lcd_map_normal,
	&svc_lcd_map_ad,
	&svc_lcd_map_normal,
	
	//second digits
	&svc_lcd_map_normal,
	&svc_lcd_map_normal,
	
	//upper right
	&svc_lcd_map_adg,
	&svc_lcd_map_normal,
	
	//upper center
	&svc_lcd_map_dig8,
	&svc_lcd_map_dig9
};


static const uint8_t idx_from_char(char c) {
	// 0...9 a...z - /
	c = tolower(c);
	if(isdigit(c)) {
		return c - '0';
	}
	else if(c >= 'a' && c <= 'z') {
		return 10 + c - 'a';
	}
	else if(c == '-') {
		return 36;
	}
	else if(c == '/') {
		return 37;
	}
	return 0;
}

static uint16_t svc_lcd_map_get(const svc_lcd_map_t *map, uint8_t idx) {
	if(idx >= map->length) {
		return 0;
	}
	if(map->type == SVC_LCD_MAP_T_8) {
		const uint8_t *p = map->map;
		return p[idx];
	}
	else if(map->type == SVC_LCD_MAP_T_16) {
		const uint16_t *p = map->map;
		return p[idx];
	}
	return 0;
}

void svc_lcd_putc(uint8_t dig, char c) {
	if(dig >= ARRAY_SIZE(digit_maps)) {
		return;
	}
	const svc_lcd_map_t *map = digit_maps[dig];
	uint16_t segments = 0;
	if(c != ' ') {
		segments = svc_lcd_map_get(map, idx_from_char(c));
	}
	hal_lcd_dig_set(dig, segments);
}

void svc_lcd_puts(uint8_t dig, const char *s) {
	while(*s) {
		svc_lcd_putc(dig++, *s++);
	}
}

void svc_lcd_puti(uint8_t dig, uint8_t len, uint32_t value) {
	dig += len-1;
	while(len--) {
		svc_lcd_putc(dig, '0'+(value%10));
		value /= 10;
		dig--;
	}
}

void svc_lcd_puti_signed(uint8_t dig, uint8_t len, int32_t value) {
	if(value<0) {
		value *= -1;
		svc_lcd_putc(dig, '-');
	}
	else {
		svc_lcd_putc(dig, ' ');
	}
	svc_lcd_puti(dig+1, len-1, value);
}

static char hex2ascii(uint8_t v) {
	if(v<10) {
		return '0'+v;
	}
	else {
		return 'a'+v-10;
	}
}

void svc_lcd_putix(uint8_t dig, uint8_t len, uint16_t value) {
	dig += len-1;
	while(len--) {
		svc_lcd_putc(dig, hex2ascii(value&0xf));
		value >>= 4;
		dig--;
	}
}
