#include <stdint.h>
#include <msp430.h>
#include "hal/aes/aes256.h"
#include "mbedtls/sha1.h"

void hal_sha1(const uint8_t *input, uint16_t len, uint8_t *output) {
	mbedtls_sha1(input, len, output);
}

void hal_aes_init(void) {
	AES256_reset(AES256_BASE);
}

void hal_aes_set_key(uint8_t *key) {
	AES256_setDecipherKey(AES256_BASE, key, AES256_KEYLENGTH_128BIT);
}

void hal_aes_clear_key(void) {
	static const uint8_t zerokey[16] = {0};
	hal_aes_set_key((void*)zerokey);
}

void hal_aes_decrypt(uint8_t *dest, uint8_t *src) {
	AES256_decryptData(AES256_BASE, src, dest);
}
