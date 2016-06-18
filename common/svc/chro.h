#pragma once
#include <stdint.h>

typedef struct {
	uint8_t min;
	uint8_t sec;
	uint8_t subsec;
} svc_chro_t;

typedef enum {
	SVC_CHRO_STATE_STOP = 0,
	SVC_CHRO_STATE_HOLD,
	SVC_CHRO_STATE_RUN
} svc_chro_state_t;

void svc_chro_start(uint8_t index);
void svc_chro_stop(uint8_t index);
void svc_chro_hold(uint8_t index);
void svc_chro_reset(uint8_t index);
void svc_chro_get(uint8_t index, svc_chro_t *out);
svc_chro_state_t svc_chro_get_state(uint8_t index);
uint8_t svc_chro_get_any_running(void);
void svc_chro_convert(svc_chro_t *ch);
uint16_t svc_chro_get_subsec_total(svc_chro_t *ch);

void svc_aux_timer_chro_handler(void);

extern uint8_t svc_chro_n;
