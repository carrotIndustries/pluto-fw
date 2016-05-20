#pragma once
#include <stdint.h>
#include "common/hal/hal.h"

#define SVC_COMPASS_TIMEOUT 5

uint8_t svc_compass_read(hal_compass_result_t *out);
void svc_compass_process(void);


