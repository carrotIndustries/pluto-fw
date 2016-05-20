#pragma once
#include <stdint.h>

void hal_i2c_init(void);
void hal_i2c_deinit(void);
uint8_t hal_i2c_write_byte(uint8_t send_start, uint8_t send_stop, uint8_t byte);
uint8_t hal_i2c_read_byte( uint8_t nack , uint8_t send_stop );
