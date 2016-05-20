#include "chro.h"
#include "aux_timer.h"
#include <string.h>



static svc_chro_t ch;
static svc_chro_t ch_hold;
svc_chro_state_t state = SVC_CHRO_STATE_STOP;

static void svc_chro_inc(svc_chro_t *ch) {
	ch->subsec++;
	if(ch->subsec == 128) {
		ch->subsec = 0;
		ch->sec++;
		if(ch->sec == 60) {
			ch->sec = 0;
			ch->min++;
			if(ch->min == 100) {
				state = SVC_CHRO_STATE_STOP;
			}
		}
	}
}

void svc_aux_timer_chro_handler(void) {
	if(state == SVC_CHRO_STATE_RUN || state == SVC_CHRO_STATE_HOLD) {
		svc_chro_inc(&ch);
		svc_aux_timer_set_call_main();
	}
	else {
		svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_CHRO, 0);
	}
}

void svc_chro_start(void) {
	svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_CHRO, 1);
	state = SVC_CHRO_STATE_RUN;
}

void svc_chro_stop(void) {
	state = SVC_CHRO_STATE_STOP;
}

void svc_chro_hold(void) {
	state = SVC_CHRO_STATE_HOLD;
	memcpy(&ch_hold, &ch, sizeof(svc_chro_t));
}

void svc_chro_reset(void) {
	state = SVC_CHRO_STATE_STOP;
	ch.min = 0;
	ch.sec = 0;
	ch.subsec = 0;
}

void svc_chro_get(svc_chro_t *out) {
	if(state == SVC_CHRO_STATE_HOLD) {
		memcpy(out, &ch_hold, sizeof(svc_chro_t));
	}
	else {
		memcpy(out, &ch, sizeof(svc_chro_t));
	}
}

svc_chro_state_t svc_chro_get_state(void) {
	return state;
}
