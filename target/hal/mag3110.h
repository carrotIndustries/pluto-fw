#pragma once
#include <stdint.h>
#include "common/hal/hal.h"

typedef enum {
	HAL_MAG3110_REG_DR_STATUS = 0x00,
	HAL_MAG3110_REG_OUT_X_MSB = 0x01,
	HAL_MAG3110_REG_OUT_X_LSB = 0x02,
	HAL_MAG3110_REG_OUT_Y_MSB = 0x03,
	HAL_MAG3110_REG_OUT_Y_LSB = 0x04,
	HAL_MAG3110_REG_OUT_Z_MSB = 0x05,
	HAL_MAG3110_REG_OUT_Z_LSB = 0x06,
	HAL_MAG3110_REG_WHO_AM_I  = 0x07,
	HAL_MAG3110_REG_SYSMOD    = 0x08,
	HAL_MAG3110_REG_OFF_X_MSB = 0x09,
	HAL_MAG3110_REG_OFF_X_LSB = 0x0a,
	HAL_MAG3110_REG_OFF_Y_MSB = 0x0b,
	HAL_MAG3110_REG_OFF_Y_LSB = 0x0c,
	HAL_MAG3110_REG_OFF_Z_MSB = 0x0d,
	HAL_MAG3110_REG_OFF_Z_LSB = 0x0e,
	HAL_MAG3110_REG_DIE_TEMP  = 0x0f,
	HAL_MAG3110_REG_CTRL_REG1 = 0x10,
	HAL_MAG3110_REG_CTRL_REG2 = 0x11,
} hal_mag3110_reg_addr_t;

#define HAL_MAG3110_REG_SYSMOD_STANDBY     0
#define HAL_MAG3110_REG_SYSMOD_ACTIVE_RAW  1
#define HAL_MAG3110_REG_SYSMOD_ACTIVE_CORR 2

#define HAL_MAG3110_REG_CTRL_REG1_AC (1<<0)
#define HAL_MAG3110_REG_CTRL_REG1_TM (1<<1)
#define HAL_MAG3110_REG_CTRL_REG1_FR (1<<2)
#define HAL_MAG3110_REG_CTRL_REG1_OS(x) (((x)&3)<<3)
#define HAL_MAG3110_REG_CTRL_REG1_DR(x) (((x)&7)<<5)

#define HAL_MAG3110_REG_CTRL_REG2_RAW (1<<5)
#define HAL_MAG3110_REG_CTRL_REG2_AUTO_MRST_EN (1<<7)

void hal_mag3110_init(void);
void hal_mag3110_set_power(uint8_t on);
uint8_t hal_mag3110_reg_read(hal_mag3110_reg_addr_t addr, uint8_t *nak);
uint8_t hal_mag3110_reg_write(hal_mag3110_reg_addr_t addr, uint8_t value);
void hal_mag3110_reg_read_burst(hal_mag3110_reg_addr_t addr, uint8_t *buf, uint8_t n, uint8_t *nak);

uint8_t hal_mag3110_read(hal_compass_result_t *out);
