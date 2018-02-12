#include "init.h"
#include "alarm.h"
#include "countdown.h"
#include "common/hal/hal.h"

void svc_init(void) {
	svc_alarm_init();
	svc_countdown_init();
	hal_aes_init();
}
