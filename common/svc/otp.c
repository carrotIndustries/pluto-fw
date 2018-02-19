#include "otp.h"
#include "util.h"
#include "platform.h"
#include "common/hal/hal.h"
#include "common/svc/otp/oath.h"

typedef struct {
	char label[2];
	uint8_t secret_len;
	uint8_t secret[32]; //2 AES blocks
} otp_item_t;

#define N_ITEMS 7
const uint8_t svc_otp_n_items = N_ITEMS;

static otp_item_t SECTION_INFOMEM otp_store[N_ITEMS] = {
	{.label = {0x66, 0x6d},
	 .secret_len = 0x5,
	 .secret= {0xeb, 0xc5, 0xdb, 0x1d, 0x5d, 0x87, 0x95, 0x84, 0x76, 0x81, 0xa7, 0x54, 0x5c, 0x34, 0x37, 0x5b, 0xc6, 0x1a, 0x1d, 0x9f, 0x71, 0xaa, 0xbe, 0xa, 0xb8, 0x9c, 0xcc, 0xcc, 0xde, 0xab, 0xf, 0x0}}};

static uint32_t time_counter;
static uint8_t locked = 1;
static uint8_t timeout;
static uint8_t g_timeout = 20;

void svc_otp_unlock(uint16_t pin) {
	timeout = g_timeout;
	uint8_t hash[20];
	hal_sha1((uint8_t*)&pin, 2, hash);
	hal_aes_init();
	hal_aes_set_key(hash);
	secure_memset(hash, 0, sizeof(hash));
	locked = 0;
}

void svc_otp_lock(void) {
	hal_aes_clear_key();
	locked = 1;
}

uint8_t svc_otp_is_locked(void) {
	return locked;
}

void svc_otp_get_label(uint8_t index, char *out) {
	otp_item_t *it = &(otp_store[index]);
	out[0] = it->label[0];
	out[1] = it->label[1];
	out[2] = 0;
}

int32_t svc_otp_get_token(uint8_t index) {
	timeout = g_timeout;
	otp_item_t *it = &(otp_store[index]);
	uint8_t secret[32];
	hal_aes_decrypt(secret, it->secret);
	hal_aes_decrypt(secret+16, it->secret+16);
	int32_t out = -1;
	oath_totp_generate ((const char *)secret,
		it->secret_len,
		time_counter, //now
		30,
		0, 6, &out);
	secure_memset(secret, 0, sizeof(secret));
	return out;
}

void svc_otp_set_time(uint32_t time) {
	time_counter = time;
}

uint8_t svc_otp_get_time_remaining(void) {
	return 29-(time_counter % 30UL);
}

void svc_otp_process(void) {
	time_counter++;
	if(timeout) {
		timeout--;
	}
	else {
		svc_otp_lock();
	}
}
