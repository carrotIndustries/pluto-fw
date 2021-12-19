extern "C" {
#include <msp430.h>
#include "hal/wdt.h"
#include "hal/io.h"
#include "hal/button.h"
#include "hal/lcd.h"
#include "hal/rtc.h"
#include "hal/beepled.h"
#include "hal/aux_timer.h"
#include "hal/compass.h"
#include "hal/aes/aes256.h"
#include "common/hal/lcd_segments.h"
#include "common/hal/hal.h"
#include "common/svc/svc.h"
#include "common/svc/otp/oath.h"
}

extern "C" void clk_init(void) {
	CSCTL0 = CSKEY; /* password */

	PCONF(J, 4, (FUNC1 | IN)); /* LFXT pin */
	PCONF(J, 5, (FUNC1 | IN)); /* LFXT pin */

	//XTS = 0; // turn xt1 in lf mode (default)
	
	CSCTL4 |= HFXTOFF; /* turn off HFXT */
	CSCTL1 = DCORSEL | DCOFSEL_4; /* DCO: 16 Mhz */
	CSCTL3 = DIVA__1 | DIVS__4 | DIVM__2; //SMCLK = 16MHz/4=4MHz, MCLK=16MHz/2=8MHz
	while(SFRIFG1&OFIFG) {
		CSCTL5 &= ~(HFXTOFFG|LFXTOFF);
		SFRIFG1 &= ~OFIFG;
	}

	CSCTL0_H = 0; /* lock access */
}

inline svc_main_proc_event_t operator|=(svc_main_proc_event_t& a, const svc_main_proc_event_t& b)
{
    a = static_cast<svc_main_proc_event_t>(static_cast<unsigned char>(a) | static_cast<int>(b));
	return a;
}

extern "C" int main(void)
{
	PM5CTL0 &= ~LOCKLPM5;
	wdt_clear();
	clk_init();
	io_init();
	rtc_init();
	lcd_init();
	button_init();
	aux_timer_init();

	uint16_t reset_reason = hal_debug_read(0);
	if(reset_reason) {
		beep_init(0);
	}
	else {
		beep_init(1);
	}

	//hal_compass_init();
	hal_backlight_set(1);
	svc_init();

	__nop();
	__eint();

	/* print reset reason on screen if there is some */
	// if(reset_reason) {
	// 	svc_lcd_puts(8, "RE");
	// 	svc_lcd_putix(4, 2, reset_reason&0xFF);
	// 	hal_lcd_update();
	// 	while(!(get_button_short(BTN_ALARM))) {
	// 		wdt_clear();
	// 		LPM3;
	// 	}
	// }

	int counter = 0;
	while(1) {

		svc_main_proc_event_t ev = SVC_MAIN_PROC_NO_EVENT;

		// counter = ++counter % 60;
		// unsigned char value = counter / 1;
		// svc_lcd_puts(8, "RE");
		// svc_lcd_putix(4, 2, value&0xFF);
		// hal_lcd_update();
		// while(1) {
		// 	int v = 1;
		// 	int t = v+1;
		// }
		if(get_button_short(BTN_LIGHT)) {
			ev |= SVC_MAIN_PROC_EVENT_KEY_UP;
		}
		if(get_button_short(BTN_MODE)) {
			ev |= SVC_MAIN_PROC_EVENT_KEY_DOWN;
		}
		if(get_button_short(BTN_ALARM)) {
			ev |= SVC_MAIN_PROC_EVENT_KEY_ENTER;
		}
		if(get_button_long(BTN_LIGHT)) {
			ev |= SVC_MAIN_PROC_EVENT_KEY_UP_LONG;
		}
		if(get_button_long(BTN_MODE)) {
			ev |= SVC_MAIN_PROC_EVENT_KEY_DOWN_LONG;
		}
		if(get_button_long(BTN_ALARM)) {
			ev |= SVC_MAIN_PROC_EVENT_KEY_ENTER_LONG;
		}
		if(tick_event) {
			ev |= SVC_MAIN_PROC_EVENT_TICK;
		}
		if(aux_timer_event) {
			ev |= SVC_MAIN_PROC_EVENT_AUX_TIMER;
		}
		if(ev) {
			P9OUT |= BIT5;
			wdt_clear();
			svc_main_proc(ev);
			hal_lcd_update();
			P9OUT &= ~BIT5;
		}
		if(ev & SVC_MAIN_PROC_EVENT_TICK) {
			tick_event = 0;
		}
		if(ev & SVC_MAIN_PROC_EVENT_AUX_TIMER) {
			aux_timer_event = 0;
		}

		LPM3;
	}
	return 0;
}