#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"
#include "common/svc/svc.h"
#include "common/svc/otp/oath.h"
#include <zmq.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <mbedtls/aes.h>
#include <mbedtls/sha1.h>

void *ctx;
void *insock;
void *outsock;

extern void beep_set_freq(unsigned short f);
extern void beep_startup();

void hal_backlight_set(uint8_t state) {
	char s[2] = "L0";
	s[1] += !!state;
	zmq_send(insock, s, 2, 0);
}

void hal_beep(uint16_t freq) {
	beep_set_freq(freq);
}

static void cat_seg(char *s, uint8_t seg) {
	switch(seg) {
		case HAL_LCD_SEG_24H :
			strcat(s, "24H");
		break;
		
		case HAL_LCD_SEG_BARS :
			strcat(s, "BARS");
		break;
		
		case HAL_LCD_SEG_BELL :
			strcat(s, "BELL");
		break;
		
		case HAL_LCD_SEG_COLON :
			strcat(s, "COLON");
		break;
		
		case HAL_LCD_SEG_LAP :
			strcat(s, "LAP");
		break;
		
		case HAL_LCD_SEG_PM :
			strcat(s, "PM");
		break;
		
		default : { //normal digit
			char t[] = "DIG0_A";
			t[3] += (seg>>4)&0xf;
			t[5] += seg&0xf;
			strcat(s, t);
		}
	}
}

void hal_lcd_seg_set(uint8_t seg, bool state) {
	char s[20] = "S0";
	
	s[1] += state==true;
	cat_seg(s, seg);
	
	zmq_send(insock, s, strlen(s), 0);
}
void hal_lcd_seg_set_blink(uint8_t seg, bool state) {
	char s[20] = "s0";
	
	s[1] += state==true;
	cat_seg(s, seg);
	
	zmq_send(insock, s, strlen(s), 0);
}

void hal_lcd_dig_set(uint8_t dig, uint16_t segments) {
	uint8_t i = 0;
	char t[] = "S0DIG0_A";
	t[5] = '0'+dig;
	while(i < 16) {
		t[7] = 'A'+i;
		t[1] = '0'+!!(segments & (1<<i));
		zmq_send(insock, t, strlen(t), 0);
		i++;
	}
}

void hal_lcd_dig_set_blink(uint8_t dig, uint8_t blink) {
	uint8_t i = 0;
	char t[] = "s0DIG0_A";
	t[5] = '0'+dig;
	t[1] = '0'+!!(blink);
	while(i < 16) {
		t[7] = 'A'+i;
		zmq_send(insock, t, strlen(t), 0);
		i++;
	}
}

void hal_lcd_dig_set_blink_mask(uint16_t mask) {
	uint8_t i = 0;
	while(i < 10) {
		hal_lcd_dig_set_blink(i, mask&(1<<i));
		i++;
	}
	
}

void hal_lcd_clear(void) {
	zmq_send(insock, "C", 1, 0);
}

static volatile int aux_timer_run = 0;
void hal_aux_timer_set(uint8_t running) {
	aux_timer_run = running;
}

static void *aux_timer_thread(void *arg) {
	void *sock = zmq_socket(ctx, ZMQ_PUSH);
	zmq_connect(sock, "ipc://aswemu-out");
	
	while(1) {
		usleep(1000000/128);
		if(aux_timer_run) {
			char *c = "A";
			zmq_send(sock, c, 1, 0);
		}
	}
	return 0;
}

int main(void) {
	ctx = zmq_ctx_new();
	insock = zmq_socket(ctx, ZMQ_PUB);
	zmq_connect(insock, "ipc://aswemu-in");
	
	outsock = zmq_socket(ctx, ZMQ_PULL);
	zmq_bind(outsock, "ipc://aswemu-out");
	char secret[] = {1,2,3};

	int32_t out = 5;
	oath_totp_generate (secret,
		    3,
		    1462981437, //now
		    30,
		    0, 6, &out);
	printf("=%d\n", out);
	
	beep_startup();
	svc_init();
	pthread_t thr;
	pthread_create(&thr, 0, aux_timer_thread, 0);
	
	uint8_t x=0;
	uint16_t i = 0;
	char rxbuf[32];
	while(1) {
		zmq_recv(outsock, rxbuf, sizeof(rxbuf), 0);
		svc_main_proc_event_t ev = 0;
		switch(rxbuf[0]) {
			case 'W' :
				ev = SVC_MAIN_PROC_EVENT_WDT;
			break;
			
			case 'K' :
				switch(rxbuf[1]) {
					case 'U' :
						ev = SVC_MAIN_PROC_EVENT_KEY_UP;
					break;
					
					case 'D' :
						ev = SVC_MAIN_PROC_EVENT_KEY_DOWN;
					break;
					
					case 'E' :
						ev = SVC_MAIN_PROC_EVENT_KEY_ENTER;
					break;
					
				}
			break;
			case 'L' :
				switch(rxbuf[1]) {
					case 'U' :
						ev = SVC_MAIN_PROC_EVENT_KEY_UP_LONG;
					break;
					
					case 'D' :
						ev = SVC_MAIN_PROC_EVENT_KEY_DOWN_LONG;
					break;
					
					case 'E' :
						ev = SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG;
					break;
					
				}
			break;
			
			case 'A' :
				ev = SVC_MAIN_PROC_EVENT_AUX_TIMER;
			break;
			
		}
		svc_main_proc(ev);
	}
	return 0;
}

void hal_rtc_get(hal_rtc_timedate_t *result) {
	struct timespec tp;
	struct tm tm;
	clock_gettime(CLOCK_REALTIME, &tp);
	localtime_r(&tp.tv_sec, &tm);
	result->h = tm.tm_hour;
	result->m = tm.tm_min;
	result->s = tm.tm_sec;
	
	result->dom = tm.tm_mday;
	result->dow = (tm.tm_wday+6)%7;
	result->month = tm.tm_mon;
	result->year = tm.tm_year + 1900;
}

void hal_rtc_set_date(hal_rtc_timedate_t *d) {
}

void hal_rtc_set_time(hal_rtc_timedate_t *d) {
}

static int16_t rtc_cal_value;

void hal_rtc_cal_set(int16_t cal) {
	printf("rtc cal set %d\n", cal);
	rtc_cal_value = cal;
}

int16_t hal_rtc_cal_get(void) {
	return rtc_cal_value;
}

uint8_t hal_compass_read(hal_compass_result_t *out) {
	out->x = 1;
	out->y = 2;
	out->z = 3;
	return 0;
}

void hal_compass_set_power(uint8_t on) {
	
}

static mbedtls_aes_context actx;

void hal_aes_init(void) {
	mbedtls_aes_init(&actx);
	hal_aes_clear_key();
}

void hal_aes_set_key(uint8_t *key) {
	mbedtls_aes_setkey_dec(&actx, key, 128);
}

void hal_aes_clear_key(void) {
	static const uint8_t zerokey[16] = {0};
	mbedtls_aes_setkey_dec(&actx, zerokey, 128);
}

void hal_aes_decrypt(uint8_t *dest, uint8_t *src) {
	mbedtls_aes_crypt_ecb(&actx, MBEDTLS_AES_DECRYPT, src, dest);
}

void hal_sha1(const uint8_t *input, uint16_t len, uint8_t *output) {
	mbedtls_sha1(input, len, output);
}

void hal_lcd_set_mode(hal_lcd_mode_t m) {
	
}

void hal_lcd_set_blink(uint8_t st) {
	
}
