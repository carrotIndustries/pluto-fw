#include "compass.h"

static uint8_t timeout; //if 0, compass is off

typedef struct {
	int16_t x0;
	int16_t y0;
	int16_t z0;
	
	uint8_t gx;
	uint8_t gy;
	uint8_t gz;
} cal_t;

static cal_t gcal = {.gx = 1, .gy = 1, .gz=1};

uint8_t svc_compass_read(hal_compass_result_t *out) {
	if(timeout == 0) {
		hal_compass_set_power(1);
	}
	timeout = SVC_COMPASS_TIMEOUT*4;
	return hal_compass_read(out);
}

uint8_t svc_compass_read_cal(hal_compass_result_t *out) {
	hal_compass_result_t t;
	if(svc_compass_read(&t)) {
		return 1;
	}
	out->x = (t.x-gcal.x0)*gcal.gx;
	out->y = (t.y-gcal.y0)*gcal.gy;
	out->z = (t.z-gcal.z0)*gcal.gz;
	
	return 0;
}

void svc_compass_set_cal(svc_compass_cal_in_t *cal) {
	gcal.x0 = cal->x0;
	gcal.y0 = cal->y0;
	gcal.z0 = cal->z0;
	
	gcal.gx = 32000/cal->sx;
	gcal.gy = 32000/cal->sy;
	gcal.gz = 32000/cal->sz;
}

void svc_compass_process(void) {
	if(timeout) {
		timeout--;
	}
	else {
		hal_compass_set_power(0);
	}
}
