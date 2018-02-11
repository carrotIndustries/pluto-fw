#pragma once
#include <stdint.h>

extern const uint8_t svc_otp_n_items;

void svc_otp_unlock(uint16_t pin);
void svc_otp_lock(void);
uint8_t svc_otp_is_locked(void);
void svc_otp_set_time(uint32_t time);
void svc_otp_get_label(uint8_t index, char *out);
int32_t svc_otp_get_token(uint8_t index);
void svc_otp_process(void);
uint8_t svc_otp_get_time_remaining(void) ;
