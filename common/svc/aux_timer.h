#pragma once
#include <stdint.h>

typedef enum {
	SVC_AUX_TIMER_REQUIRED_BEEP   = (1<<0),
	SVC_AUX_TIMER_REQUIRED_MELODY = (1<<1),
	SVC_AUX_TIMER_REQUIRED_CHRO   = (1<<2),
} svc_aux_timer_required_t;

void svc_aux_timer_set_required(svc_aux_timer_required_t which, uint8_t state);
svc_aux_timer_required_t svc_aux_timer_get_required(void);
void svc_aux_timer_handler(void);
void svc_aux_timer_set_call_main(void);
uint8_t svc_aux_timer_get_call_main(void);
