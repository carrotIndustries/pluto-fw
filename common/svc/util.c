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

uint32_t atani(int32_t x, int32_t y) {
   // Fast XY vector to integer degree algorithm - Jan 2011 www.RomanBlack.com
   // Converts any XY values including 0 to a degree value that should be
   // within +/- 1 degree of the accurate value without needing
   // large slow trig functions like ArcTan() or ArcCos().
   // NOTE! at least one of the X or Y values must be non-zero!
   // This is the full version, for all 4 quadrants and will generate
   // the angle in integer degrees from 0-360.
   // Any values of X and Y are usable including negative values provided
   // they are between -1456 and 1456 so the 16bit multiply does not overflow.

   unsigned char negflag;
   unsigned char tempdegree;
   unsigned char comp;
   uint32_t degree;     // this will hold the result
   uint32_t ux;
   uint32_t uy;

   // Save the sign flags then remove signs and get XY as unsigned ints
   negflag = 0;
   if(x < 0)
   {
      negflag += 0x01;    // x flag bit
      x = (0 - x);        // is now +
   }
   ux = x;                // copy to unsigned var before multiply
   if(y < 0)
   {
      negflag += 0x02;    // y flag bit
      y = (0 - y);        // is now +
   }
   uy = y;                // copy to unsigned var before multiply

   // 1. Calc the scaled "degrees"
   if(ux > uy)
   {
      degree = (uy * 45) / ux;   // degree result will be 0-45 range
      negflag += 0x10;    // octant flag bit
   }
   else
   {
      degree = (ux * 45) / uy;   // degree result will be 0-45 range
   }

   // 2. Compensate for the 4 degree error curve
   comp = 0;
   tempdegree = degree;    // use an unsigned char for speed!
   if(tempdegree > 22)      // if top half of range
   {
      if(tempdegree <= 44) comp++;
      if(tempdegree <= 41) comp++;
      if(tempdegree <= 37) comp++;
      if(tempdegree <= 32) comp++;  // max is 4 degrees compensated
   }
   else    // else is lower half of range
   {
      if(tempdegree >= 2) comp++;
      if(tempdegree >= 6) comp++;
      if(tempdegree >= 10) comp++;
      if(tempdegree >= 15) comp++;  // max is 4 degrees compensated
   }
   degree += comp;   // degree is now accurate to +/- 1 degree!

   // Invert degree if it was X>Y octant, makes 0-45 into 90-45
   if(negflag & 0x10) degree = (90 - degree);

   // 3. Degree is now 0-90 range for this quadrant,
   // need to invert it for whichever quadrant it was in
   if(negflag & 0x02)   // if -Y
   {
      if(negflag & 0x01)   // if -Y -X
            degree = (180 + degree);
      else        // else is -Y +X
            degree = (180 - degree);
   }
   else    // else is +Y
   {
      if(negflag & 0x01)   // if +Y -X
            degree = (360 - degree);
   }
   return degree;
}
