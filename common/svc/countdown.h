#pragma once
#include <stdint.h>

typedef enum {
	SVC_COUNTDOWN_STATE_STOP = 0,
	SVC_COUNTDOWN_STATE_RUN
} svc_countdown_state_t;

#define SVC_COUNTDOWN_COMMON \
	int8_t h; \
	int8_t m; \
	int8_t s; \
	svc_countdown_state_t state; \
	uint8_t melody; \

typedef struct {
	SVC_COUNTDOWN_COMMON
} svc_countdown_t;

extern const uint8_t svc_countdowns_n;

void svc_countdown_init(void);
void svc_countdown_get(uint8_t index, svc_countdown_t *out);
void svc_countdown_set_time(uint8_t index, uint8_t h, uint8_t m, uint8_t s);
void svc_countdown_start(uint8_t index);
void svc_countdown_stop(uint8_t index);
void svc_countdown_process(void);
uint8_t svc_countdown_get_n_running(void);
uint8_t svc_countdown_get_pending(void);
void svc_countdown_clear_pending(void);
void svc_countdown_set_melody(uint8_t index, uint8_t melody);
void svc_countdown_draw_popup(void);
