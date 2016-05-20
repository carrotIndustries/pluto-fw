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

void svc_chro_start(void);
void svc_chro_stop(void);
void svc_chro_hold(void);
void svc_chro_reset(void);
void svc_chro_get(svc_chro_t *out);
svc_chro_state_t svc_chro_get_state(void);

void svc_aux_timer_chro_handler(void);

