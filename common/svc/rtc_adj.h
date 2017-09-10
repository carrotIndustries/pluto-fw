#pragma once
#include <stdint.h>

void svc_rtc_adj_sec(int8_t s);
int8_t svc_rtc_adj_get_sec(void);

void svc_rtc_adj_process(void);
uint8_t svc_rtc_adj_get_pending(void);
