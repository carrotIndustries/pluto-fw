#pragma once
#include "Event.hpp"
#include <variant>
#include "common/app/cpp/framework/hooks/useState.hpp"
#include "common/app/cpp/framework/hooks/useGlobalState.hpp"
#include "common/app/cpp/framework/demos/root.hpp"
#include "common/app/cpp/framework/state.hpp"
#include "common/app/cpp/framework/eventQueue.hpp"
#include "common/app/cpp/framework/createComponent.hpp"

void trigger(const Event& event);

void processEvent(const Event& event);

void workQueue();