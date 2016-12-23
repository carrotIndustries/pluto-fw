#pragma once
#include <stdint.h>
#include "common/hal/hal.h"

#define SVC_COMPASS_TIMEOUT 5

typedef struct {
	int16_t x0;
	int16_t y0;
	int16_t z0;
	
	uint16_t sx;
	uint16_t sy;
	uint16_t sz;
} svc_compass_cal_in_t;

uint8_t svc_compass_read(hal_compass_result_t *out);
void svc_compass_set_cal(svc_compass_cal_in_t *cal);
uint8_t svc_compass_read_cal(hal_compass_result_t *out);
void svc_compass_process(void);


