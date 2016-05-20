#include "mag3110.h"
#include "io.h"
#include "i2c.h"
#include <msp430.h>

#define DEVICE_ADDR (0x0e<<1)

void hal_mag3110_init(void) {
	PCONF(J, 7, OUL); /* MAG PWR */
	PCONF(J, 6, IN); /* MAG int1 */
	hal_mag3110_set_power(0);
}

void hal_mag3110_set_power(uint8_t on) {
	if(on) {
		PJOUT |= BIT7;
		hal_i2c_init();
		while(hal_mag3110_reg_write(HAL_MAG3110_REG_CTRL_REG1, HAL_MAG3110_REG_CTRL_REG1_AC | HAL_MAG3110_REG_CTRL_REG1_OS(0) | HAL_MAG3110_REG_CTRL_REG1_DR(5)))
			;
		hal_mag3110_reg_write(HAL_MAG3110_REG_CTRL_REG2, HAL_MAG3110_REG_CTRL_REG2_RAW|HAL_MAG3110_REG_CTRL_REG2_AUTO_MRST_EN);
	}
	else {
		hal_i2c_deinit();
		PJOUT &= ~BIT7;
	}
}

uint8_t hal_mag3110_reg_read(hal_mag3110_reg_addr_t addr, uint8_t *nak) {
	uint8_t n=0;
	uint8_t b=0;
	n |= hal_i2c_write_byte(1, 0, DEVICE_ADDR); //device addr write, start
	if(n)
		goto out;
	n |= hal_i2c_write_byte(0, 0, addr); //reg addr
	if(n)
		goto out;
	n |= hal_i2c_write_byte(1, 0, DEVICE_ADDR|1); //device addr read, restart
	if(n)
		goto out;
	b = hal_i2c_read_byte(1, 1); // stop, nak
	out:
	if(nak) {
		*nak = n;
	}
	return b;
}

void hal_mag3110_reg_read_burst(hal_mag3110_reg_addr_t addr, uint8_t *buf, uint8_t len, uint8_t *nak) {
	uint8_t n=0;
	/* uint8_t b=0; */
	n |= hal_i2c_write_byte(1, 0, DEVICE_ADDR); //device addr write, start
	if(n)
		goto out;
	n |= hal_i2c_write_byte(0, 0, addr); //reg addr
	if(n)
		goto out;
	n |= hal_i2c_write_byte(1, 0, DEVICE_ADDR|1); //device addr read, restart
	if(n)
		goto out;
	for(uint8_t i = 0; i<len; i++) {
		uint8_t last = (i == len-1);
		buf[i] = hal_i2c_read_byte(last, last); // stop, nak
	}
	out:
	if(nak) {
		*nak = n;
	}
}

uint8_t hal_mag3110_reg_write(hal_mag3110_reg_addr_t addr, uint8_t value) {
	uint8_t n=0;
	n |= hal_i2c_write_byte(1, 0, DEVICE_ADDR); //device addr write, start
	if(n)
		goto out;
	n |= hal_i2c_write_byte(0, 0, addr); //reg addr
	if(n)
		goto out;
	n |= hal_i2c_write_byte(0, 1, value); //reg value
	if(n)
		goto out;
		
	out :
	return n;
}

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
} out_t;

uint8_t hal_mag3110_read(hal_compass_result_t *out) {
	uint8_t buf[6];
	uint8_t nak;
	hal_mag3110_reg_read_burst(HAL_MAG3110_REG_OUT_X_MSB, buf, 6, &nak);
	if(nak) {
		return 1;
	}
	out_t *p = (void*)out;
	p->x = (buf[0]<<8) | buf[1];
	p->y = (buf[2]<<8) | buf[3];
	p->z = (buf[4]<<8) | buf[5];
	return 0;
}
