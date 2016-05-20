#include "aux_timer.h"
#include "beep.h"
#include "melody.h"
#include "chro.h"
#include "common/hal/hal.h"

static svc_aux_timer_required_t aux_timer_req = 0;
static uint8_t aux_timer_call_main = 0;

void svc_aux_timer_set_required(svc_aux_timer_required_t which, uint8_t state) {
	if(state) {
		aux_timer_req |= which;
	}
	else {
		aux_timer_req &= ~which;
	}
	hal_aux_timer_set(!!aux_timer_req);
}

void svc_aux_timer_handler(void) {
	if(aux_timer_req & SVC_AUX_TIMER_REQUIRED_BEEP) {
		svc_aux_timer_beep_handler();
	}
	if(aux_timer_req & SVC_AUX_TIMER_REQUIRED_MELODY) {
		svc_aux_timer_melody_handler();
	}
	if(aux_timer_req & SVC_AUX_TIMER_REQUIRED_CHRO) {
		svc_aux_timer_chro_handler();
	}
}

void svc_aux_timer_set_call_main(void) {
	aux_timer_call_main = 1;
}

uint8_t svc_aux_timer_get_call_main(void) {
	uint8_t r = aux_timer_call_main;
	aux_timer_call_main = 0;
	return r;
}
