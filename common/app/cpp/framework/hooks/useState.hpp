#pragma once
#include <concepts>
#include <vector>
//#include <algorithm>
//#include "common/app/cpp/framework/demos/demoA.hpp"
#include "common/app/cpp/framework/createComponent.hpp"
#include "common/app/cpp/framework/eventQueue.hpp"
#include "common/app/cpp/framework/Event.hpp"

template<std::copyable ValueType>
class ActionValue {
    const ValueType value;
    ValueType* const realValue;
    const ComponentData* component;
public:
    ActionValue(ValueType* realValue, ComponentData* component): value(*realValue), realValue(realValue),component(component){}
    operator const ValueType&() const { return value; }
    void set(const ValueType& newValue){
        if(*realValue != newValue){
            *realValue = newValue;
            eventQueue.push(StateEvent(component));
        }
    }
    const ValueType& get() const{
        return value; 
    }
};

template<std::copyable ValueType>
ActionValue<ValueType> useState(const ValueType&& defaultValue){
    ValueType* data = (ValueType*)current->getHookData();
    if(!data){
        data = new ValueType(defaultValue);
        current->setHookData(data,  [](const void* x) { static_cast<const ValueType*>(x)->~ValueType(); });
    }
   
    ActionValue av(data, current);
    return av;
}