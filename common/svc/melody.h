#pragma once
#include <stdint.h>

typedef struct {
	uint16_t freq;
	uint16_t duration;
} svc_melody_note_t;

typedef struct {
	char title[2];
	const svc_melody_note_t *notes;
} svc_melody_t;

extern const svc_melody_t svc_melodies[];
extern const uint8_t svc_melodies_n;

uint8_t svc_default_melody_get(void);
void svc_default_melody_set(uint8_t melody_idx);
void svc_melody_play_repeat(uint8_t melody, uint8_t rep);
void svc_melody_play(uint8_t melody);
void svc_melody_stop(void);
void svc_aux_timer_melody_handler(void);
uint8_t svc_melody_alarm_repetitions_get(void);
void svc_melody_alarm_repetitions_set(uint8_t repetitions);
