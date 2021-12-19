#pragma once
#include <vector>
#include "common/app/cpp/framework/createComponent.hpp"
#include "common/app/cpp/framework/state.hpp"

std::vector<ComponentData*> root(State* state, const std::vector<int>& params);

#include "common/app/cpp/framework/demos/demoB.hpp"