#pragma once
#include <vector>
#include "common/app/cpp/framework/createComponent.hpp"
#include "common/app/cpp/framework/state.hpp"

std::vector<ComponentData*> demoB(State* state, const std::vector<int>& params);

#include "common/app/cpp/framework/hooks/useState.hpp"
#include "common/app/cpp/framework/hooks/useGlobalState.hpp"
