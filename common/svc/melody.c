#include "melody.h"
#include "common/hal/hal.h"
#include "aux_timer.h"

static const svc_melody_note_t *note_cur = 0;
static const svc_melody_note_t *note_start = 0;
static uint16_t note_timer;
static uint8_t repeat;
static uint8_t repeat_delay;


void svc_melody_play_repeat(uint8_t melody, uint8_t rep) {
	note_cur = svc_melodies[melody];
	note_start = svc_melodies[melody];
	note_timer = 0;
	repeat = rep;
	repeat_delay = 0;
	svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_MELODY, 1);
}

void svc_melody_play(uint8_t melody) {
	svc_melody_play_repeat(melody, 1);
}

void svc_melody_stop(void) {
	svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_MELODY, 0);
}

void svc_aux_timer_melody_handler(void) {
	if(repeat_delay) {
		repeat_delay--;
		return;
	}

	if(note_cur) {
		if(note_cur->duration == 0) {
			hal_beep(0);
			repeat--;
			if(repeat) {
				note_cur = note_start;
				repeat_delay = 255;
			}
			else {
				svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_MELODY, 0);
			}
		}
		else {
			hal_beep(note_cur->freq);
			note_timer++;
			if(note_timer >= note_cur->duration) {
				note_cur++;
				note_timer = 0;
			}
		}
	}
	else {
		hal_beep(0);
		svc_aux_timer_set_required(SVC_AUX_TIMER_REQUIRED_MELODY, 0);
	}
}
