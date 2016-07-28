#pragma once
#include <stdint.h>

typedef struct {
	uint8_t h;
	uint8_t m;
} svc_beep_hour_quiet_t;

void svc_beep_timed(uint16_t freq, uint16_t duration);
void svc_aux_timer_beep_handler(void);

uint16_t svc_beep_key_get_freq(void);
uint16_t svc_beep_key_get_duration(void);
void svc_beep_key_set_freq(uint16_t f);
void svc_beep_key_set_duration(uint16_t d);
void svc_beep_key_set_enable(uint8_t e);
uint8_t svc_beep_key_get_enable(void);
void svc_beep_key(void);
void svc_beep_hour(void);

void svc_beep_hour_set_enable(uint8_t e);
uint8_t svc_beep_hour_get_enable(void);
uint16_t svc_beep_hour_get_freq(void);
uint16_t svc_beep_hour_get_duration(void);
void svc_beep_hour_set_freq(uint16_t f);
void svc_beep_hour_set_duration(uint16_t d);

uint8_t svc_beep_hour_quiet_get_enable(void);
void svc_beep_hour_quiet_set_enable(uint8_t e);
void svc_beep_hour_quiet_get_time(uint8_t start, svc_beep_hour_quiet_t *out);
void svc_beep_hour_quiet_set_time(uint8_t start, uint8_t h, uint8_t m);
