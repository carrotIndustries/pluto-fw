#pragma once
#include <stdint.h>

void svc_seconds_since_last_set_reset();
void svc_seconds_since_last_set_process();
uint32_t svc_seconds_since_last_set_get();
