#pragma once
#include <optional>

extern "C" {
#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"
}

struct State {
    std::optional<bool> pm;
    std::optional<bool> twentyfour;
    std::optional<bool> alarm;
    std::optional<bool> bell;
    std::optional<bool> lap;
    std::optional<bool> colon;
    std::optional<bool> dot;
    std::optional<char> text[10];

    std::optional<bool> backlight;
    std::optional<bool> beeper;

    void realize();
    /** Layer the other state on top of this */
    void layer(const State& other);
};