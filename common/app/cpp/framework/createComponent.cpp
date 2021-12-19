#include "createComponent.hpp"

ComponentData* current = nullptr;
ComponentData* parent = nullptr;

ComponentData::ComponentData(const componentFunction function, const std::vector<int>& params ): lastParams(params), lastResult(), function(function), state(new State()),hookCounter(0){
    render();
}

ComponentData::~ComponentData(){
    for(ComponentData* old : lastResult){
        delete old;
    }
    for(HookData& data : hookData){
        (*(data.destroy))(data.data);
    }
}

/**
 * If the current postion already has a value:
 *   Returns the pointer to the data and increments the counter
 * If there is no value:
 *   Does nothing. Use setHookData.
*/
void* ComponentData::getHookData(){
    if(hookCounter < hookData.size()){
        return hookData[hookCounter++].data;
    }
    return nullptr;
}

void ComponentData::setHookData(void* data, void(*destroy)(const void*)){
    hookData.push_back({data,destroy}); // This is quite fragile, rethink
    hookCounter++;
}

void ComponentData::render(){
    //Reset hook counter
    hookCounter = 0;

    auto lastParent = parent;
    auto lastCurrent = current;
    current = this;

    auto result = (*(current->function))(state.get(), current->lastParams);

    for(ComponentData* old : lastResult){
        delete old;
    }

    current->lastResult = result;

    current = lastCurrent;
    parent = lastParent;
}