#include <msp430.h>
#include "rtc.h"
#include "common/svc/util.h"
#include "common/hal/hal.h"

static volatile hal_rtc_timedate_t timedate_l;

#define BEFORE_LEAPSECOND 0
#define DURING_LEAPSECOND 1
#define AFTER_LEAPSECOND 2

static volatile int leapSecond_status = BEFORE_LEAPSECOND;

#define RTC_LOCK WITH(RTCCTL0_H = RTCKEY_H, RTCCTL0_H = 0)

void rtc_init(void)
{
	RTC_LOCK {
		RTCCTL1 = RTCMODE;
		RTCCTL0_L = RTCRDYIE_L;
	}
}

void hal_rtc_get(hal_rtc_timedate_t *result)
{
	RTC_LOCK {
		RTCCTL0_L &= ~RTCRDYIE_L; /* lock the interrupt for readout */
		*result = timedate_l;
		RTCCTL0_L |= RTCRDYIE_L;
	}
}

void hal_rtc_set_time(hal_rtc_timedate_t *time)
{
	RTC_LOCK {
		RTCCTL0_L &= ~RTCRDYIE_L; /* lock the interrupt for readout */
		RTCCTL1 |= RTCHOLD;
		RTCHOUR = time->h;
		RTCMIN = time->m;
		RTCSEC = time->s;
		RTCCTL1 &= ~RTCHOLD;
		RTCCTL0_L |= RTCRDYIE_L;
	}
}

/* calculate day of week with monday=0 */
static int wday(int year, int month, int day)
{
	int adjustment, mm, yy;

	adjustment = (14 - month) / 12;
	mm = month + 12 * adjustment - 2;
	yy = year - adjustment;
	return ((day + (13 * mm - 1) / 5 +
	        yy + yy / 4 - yy / 100 + yy / 400) + 6) % 7;
}

void hal_rtc_set_date(hal_rtc_timedate_t *date)
{
	RTC_LOCK {
		RTCCTL0_L &= ~RTCRDYIE; /* lock the interrupt for readout */
		RTCCTL1 |= RTCHOLD;
		RTCDAY = date->dom;
		RTCMON = date->month;
		RTCYEAR = date->year;
		RTCDOW = wday(date->year, date->month, date->dom);
		RTCCTL1 &= ~RTCHOLD;
		RTCCTL0_L |= RTCRDYIE;
	}
}

void hal_rtc_cal_set(int16_t cal)
{
	RTC_LOCK {
		if(cal < 0)
			RTCOCAL = (-1 * cal) & 0xff;
		else
			RTCOCAL = RTCOCALS | (cal & 0xff);
	}
}

int16_t hal_rtc_cal_get(void)
{
	int16_t r = RTCOCAL & 0xff;
	if(!(RTCOCAL & RTCOCALS)) {
		r *= -1;
	}
	return r;
}

void __attribute__((interrupt ((RTC_VECTOR)))) RTC_ISR(void)
{
	RTCIV = 0;
	if(RTCCTL1 & RTCRDY) {
		
		// test for leap second
		if (RTCHOUR == 0 &&
			RTCMIN == 0 &&
			RTCSEC == 0 &&
			RTCDAY == 1 &&
			RTCMON == 1 &&
			RTCYEAR == 2017 &&
			leapSecond_status == BEFORE_LEAPSECOND) {			
			leapSecond_status = DURING_LEAPSECOND;

			//set date + time back one second
			RTC_LOCK {
				RTCCTL0_L &= ~RTCRDYIE_L;
				RTCCTL1 |= RTCHOLD;
				RTCHOUR = 23;
				RTCMIN = 59;
				RTCSEC = 59;
				RTCDAY = 31;
				RTCMON = 12;
				RTCYEAR = 2016;
				RTCDOW = wday(2016, 12, 31);
				RTCCTL1 &= ~RTCHOLD;
				RTCCTL0_L |= RTCRDYIE_L;
			};
		}
		
		timedate_l.h = RTCHOUR;
		timedate_l.m = RTCMIN;
		timedate_l.s = RTCSEC;

		timedate_l.dow = RTCDOW;
		timedate_l.dom = RTCDAY;
		timedate_l.month = RTCMON;
		timedate_l.year = RTCYEAR;
		
		if (RTCHOUR == 0 &&
			RTCMIN == 0 &&
			RTCSEC == 0 &&
			RTCDAY == 1 &&
			RTCMON == 1 &&
			RTCYEAR == 2017 &&
			leapSecond_status == DURING_LEAPSECOND) {			
			leapSecond_status = AFTER_LEAPSECOND;
		}
		
		if (leapSecond_status == DURING_LEAPSECOND) {
			timedate_l.s = 60;
		}

		if (RTCHOUR == 0 &&
			RTCMIN == 0 &&
			RTCSEC == 1 &&
			RTCDAY == 1 &&
			RTCMON == 1 &&
			RTCYEAR == 2017 &&
			leapSecond_status == AFTER_LEAPSECOND) {			
			leapSecond_status = BEFORE_LEAPSECOND;	/* reset status, in case time is reset to before leapsecond again */
		}
	}
}
