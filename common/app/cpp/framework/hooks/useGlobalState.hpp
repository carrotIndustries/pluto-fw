#pragma once
#include <concepts>
#include <vector>
//#include <algorithm>
//#include "common/app/cpp/framework/demos/demoA.hpp"
#include "common/app/cpp/framework/createComponent.hpp"
#include "common/app/cpp/framework/eventQueue.hpp"
#include "common/app/cpp/framework/Event.hpp"

#define MAX_AFFECTED_COMPONENTS 8

template<std::copyable ValueType>
class GlobalActionValue {
    const ValueType value;
    ValueType& realValue;
    ComponentData* (&components)[8];
public:
    GlobalActionValue(ValueType& realValue, ComponentData* (&components)[8]): value(realValue), realValue(realValue),components(components){}
    operator const ValueType&() const { return value; }
    void set(const ValueType& newValue){
        if(realValue != newValue){
            realValue = newValue;
            for(auto component: components){
                if(component){
                eventQueue.push(StateEvent(component));
                }
            }
        }
    }
    const ValueType& get() const{
        return value; 
    }
};

template<const unsigned char identifier, std::copyable ValueType>
GlobalActionValue<ValueType> useGlobalState(const ValueType&& defaultValue){
    static ValueType value(defaultValue);
    static ComponentData* affectedComponents[MAX_AFFECTED_COMPONENTS] = {nullptr};

    if(current){
    void* data = current->getHookData();
    if(!data){
        bool found = false;  //Only for debugging
        for(int i = 0; i < MAX_AFFECTED_COMPONENTS; i++){
            if(!affectedComponents[i]){
                affectedComponents[i] = current;
                current->setHookData(&(affectedComponents[i]),  [](const void* x) { *((ComponentData**)x) = nullptr; });
                found = true;
                break;
            }
        }
        if(!found){
            volatile int x = 5;
        }
        
    }
    }
   
    GlobalActionValue<ValueType> av(value, affectedComponents);
    return av;
}