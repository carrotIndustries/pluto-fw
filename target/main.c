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

void clk_init(void) {
	CSCTL0 = CSKEY; /* password */

	PCONF(J, 4, (FUNC1 | IN)); /* LFXT pin */
	PCONF(J, 5, (FUNC1 | IN)); /* LFXT pin */

	CSCTL4 |= HFXTOFF; /* turn off HFXT */
	CSCTL1 = DCORSEL | DCOFSEL_4; /* DCO: 16 Mhz */
	CSCTL3 = DIVA__1 | DIVS__4 | DIVM__2; //SMCLK = 16MHz/4=4MHz, MCLK=16MHz/2=8MHz
	while(SFRIFG1&OFIFG) {
		CSCTL5 &= ~(HFXTOFFG|LFXTOFF);
		SFRIFG1 &= ~OFIFG;
	}

	CSCTL0_H = 0; /* lock access */
}

/* insert events with debugger by manipulating this variable */
static volatile uint8_t fake_event = 0;

int main(void)
{
	PM5CTL0 &= ~LOCKLPM5;
	wdt_clear();
	clk_init();
	io_init();
	rtc_init();
	lcd_init();
	button_init();
	aux_timer_init();
	/* if last reboot reason was BOR, disable beep */
	uint16_t reset_reason = hal_debug_read(0);
	if(reset_reason == SYSRSTIV_BOR)
		beep_init(0);
	else
		beep_init(1);
	hal_compass_init();
	hal_backlight_set(0);
	svc_init();

	__nop();
	__eint();

	/* print reset reason on screen if there is some */
	if(reset_reason) {
		svc_lcd_puts(8, "RS");
		svc_lcd_putix(4, 2, reset_reason&0xFF);
		LPM3;
	}

	while(1) {
		svc_main_proc_event_t ev = 0;
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
		if(fake_event) {
			ev |= fake_event;
			fake_event = 0;
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
