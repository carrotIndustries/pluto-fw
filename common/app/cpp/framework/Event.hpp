#pragma once
#include "createComponent.hpp"
#include <variant>

// enum EventType {
//     AlarmDown,
//     AlarmUp,
//     LightDown,
//     LightUp,
//     ModeDown,
//     ModeUp,
//     Tick,
//     AuxTimer
// };

class ModeDownEvent {};
class ModeUpEvent {};
class LightDownEvent {};
class LightUpEvent {};
class AlarmDownEvent {};
class AlarmUpEvent {};
class TickEvent {};
class AuxTimerEvent {};
class StateEvent {
    public:
    StateEvent(ComponentData* component);
    ComponentData* component;
};

typedef std::variant<ModeDownEvent, ModeUpEvent, LightDownEvent, LightUpEvent, AlarmDownEvent, AlarmUpEvent, TickEvent, AuxTimerEvent, StateEvent> Event;