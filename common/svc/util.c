#include "util.h"

static const char *dow_en[] = {
	"mo",
	"tu",
	"we",
	"th",
	"fr",
	"sa",
	"su",
};

static const char *dow_de[] = {
	"mo",
	"di",
	"mi",
	"do",
	"fr",
	"sa",
	"so",
};

static const char *dow_fr[] = {
	"lu",
	"ma",
	"me",
	"je",
	"ve",
	"sa",
	"di",
};

const char *svc_dow_to_string(uint8_t dow, svc_lang_t lang) {
	switch(lang) {
		case SVC_LANG_DE :
			return dow_de[dow];
		
		case SVC_LANG_EN :
			return dow_en[dow];
		
		case SVC_LANG_FR :
			return dow_fr[dow];
			
		default :
			return dow_en[dow];
	}
}

int ipow(int base, int exp) {
	int result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}

void* secure_memset(void *v, int c, size_t n) {
    volatile char *p = v;
    while (n--) *p++ = c;
    return v;
}
