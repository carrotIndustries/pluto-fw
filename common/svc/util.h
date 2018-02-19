#pragma once
#include <stdint.h>
#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define INC_MOD(x, mod) do { \
	if((x) == (mod)-1) { \
		(x) = 0; \
	} \
	else { \
		(x)++; \
	} \
	} while (0)

#define DEC_MOD(x, mod) do { \
	if((x) == 0) { \
		(x) = (mod)-1; \
	} \
	else { \
		(x)--; \
	} \
	} while (0)

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define ABS(a)     (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define CLAMP_ABS(x, lim) (CLAMP((x), (-lim), (lim)))

//http://blog.sujeet.me/2014/05/block-encompassing-macros-in-c.html
#define MERGE__(a,b)  a##b
#define LABEL__(a,b) MERGE__(a, b)
#define UNIQUE(var_name) LABEL__(var_name, __LINE__)

#define WITH(enter, leave) \
	uint8_t UNIQUE(i);    \
	for(UNIQUE(i)=0, (enter); UNIQUE(i)<1; (leave), UNIQUE(i)++) \

typedef enum {
	SVC_LANG_EN,
	SVC_LANG_DE,
	SVC_LANG_FR,
} svc_lang_t;

const char *svc_dow_to_string(uint8_t dow, svc_lang_t lang);
int ipow(int base, int exp);
void* secure_memset(void *v, int c, size_t n);
uint32_t atani(int32_t x, int32_t y);
