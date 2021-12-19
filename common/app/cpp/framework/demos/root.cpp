#include "root.hpp"

std::vector<ComponentData*> root(State* state, const std::vector<int>& params){
    auto demoComponent = new ComponentData(demoB, {});
    return {demoComponent};
}