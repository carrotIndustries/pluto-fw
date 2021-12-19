extern "C" {
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"
}
#include "beeper.hpp"

enum Button {
    Light,
    Mode,
    Alarm
};

class Casio {
    int counter;
public:
    Casio();
    void onTick();
    void onShortPress(const Button& b);
    void onLongPress(const Button& b);
    void onAuxTimer();
    ~Casio();
};