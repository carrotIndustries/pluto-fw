#include "chro.h"
#include "aux_timer.h"
#include <string.h>

#define N_CHRO 2
uint8_t svc_chro_n = N_CHRO;

typedef struct {
	svc_chro_t t;
	svc_chro_t t_hold;
	svc_chro_state_t state;
} svc_chro_item_t;

static svc_chro_item_t chros[N_CHRO];
static uint8_t n_running;

static void svc_chro_inc(svc_chro_item_t *it) {
	svc_chro_t *ch = &(it->t);
	ch->subsec++;
	if(ch->subsec == 128) {
		ch->subsec = 0;
		ch->sec++;
		if(ch->sec == 60) {
			ch->sec = 0;
			ch->min++;
			if(ch->min == 100) {
				it->state = SVC_CHRO_STATE_STOP;
				if(n_running>0) {
					n_running--;
				}
			}
		}
	}
}

void svc_aux_timer_chro_handler(void) {
	for(uint8_t i = 0; i<svc_chro_n; i++) {
		svc_chro_item_t *it = &(chros[i]);
		if(it->state == SVC_CHRO_STATE_RUN || it->state == SVC_CHRO_STATE_HOLD) {
			svc_chro_inc(it);
			svc_aux_timer_set_call_main();
		}
	}
	if(n_running == 0) {
		svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_CHRO, 0);
	}
}

void svc_chro_start(uint8_t index) {
	svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_CHRO, 1);
	if(chros[index].state == SVC_CHRO_STATE_STOP) {
		n_running++;
	}
	chros[index].state = SVC_CHRO_STATE_RUN;
}

void svc_chro_stop(uint8_t index) {
	if((n_running > 0) && ((chros[index].state == SVC_CHRO_STATE_RUN) || (chros[index].state == SVC_CHRO_STATE_HOLD))) {
		n_running--;
	}
	chros[index].state = SVC_CHRO_STATE_STOP;
}

void svc_chro_hold(uint8_t index) {
	chros[index].state = SVC_CHRO_STATE_HOLD;
	memcpy(&(chros[index].t_hold), &(chros[index].t), sizeof(svc_chro_t));
}

void svc_chro_reset(uint8_t index) {
	chros[index].state = SVC_CHRO_STATE_STOP;
	chros[index].t.min = 0;
	chros[index].t.sec = 0;
	chros[index].t.subsec = 0;
}

void svc_chro_get(uint8_t index, svc_chro_t *out) {
	if(chros[index].state == SVC_CHRO_STATE_HOLD) {
		memcpy(out, &(chros[index].t_hold), sizeof(svc_chro_t));
	}
	else {
		memcpy(out, &(chros[index].t), sizeof(svc_chro_t));
	}
}

void svc_chro_convert(svc_chro_t *ch) {
	ch->subsec = (ch->subsec*100)/128;
}

uint16_t svc_chro_get_subsec_total(svc_chro_t *ch) {
	return ch->subsec + ch->sec*100 + ch->min*60*100; 
}

svc_chro_state_t svc_chro_get_state(uint8_t index) {
	return chros[index].state;
}

uint8_t svc_chro_get_any_running(void) {
	return n_running>0;
}
