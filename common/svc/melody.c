#include "melody.h"
#include "common/hal/hal.h"
#include "aux_timer.h"
#include "platform.h"

static const svc_melody_note_t *note_cur = 0;
static const svc_melody_note_t *note_start = 0;
static uint16_t note_timer;
static uint8_t repeat;
static uint8_t repeat_delay;

static uint8_t SECTION_INFOMEM default_melody = 0;

uint8_t svc_default_melody_get(void) {
	return default_melody;
}

void svc_default_melody_set(uint8_t melody_idx) {
	default_melody = melody_idx;
}

void svc_melody_play_repeat(uint8_t melody, uint8_t rep) {
	note_cur = svc_melodies[melody].notes;
	note_start = svc_melodies[melody].notes;
	note_timer = 0;
	repeat = (!rep)?255:rep;
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
			if(repeat != 255) { /* repeat==255 -> repeat infinite */
				repeat--;
			}
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

static uint8_t SECTION_INFOMEM alarm_repetitions = 10;

uint8_t svc_melody_alarm_repetitions_get(void) {
	return alarm_repetitions;
}

void svc_melody_alarm_repetitions_set(uint8_t repetitions) {
	alarm_repetitions = repetitions;
}
