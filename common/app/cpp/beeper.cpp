extern "C" {
#include "common/hal/hal.h"
#include "common/svc/beep.h"
}

#include "beeper.hpp"

void Beeper::beep(){
    svc_beep_timed(1000, 550);
};