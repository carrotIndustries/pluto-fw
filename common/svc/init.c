#include "init.h"
#include "alarm.h"
#include "common/hal/hal.h"

void svc_init(void) {
	svc_alarm_init();
	hal_aes_init();
}
