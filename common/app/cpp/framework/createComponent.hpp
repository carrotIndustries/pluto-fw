#pragma once

#include "state.hpp"
#include <vector>
#include <map>
#include <memory>

class ComponentData;

typedef std::vector<ComponentData*> (*componentFunction)(State*, const std::vector<int>&);

struct HookData {
    void* data;
    void(*destroy)(const void*);
};

class ComponentData{
    ComponentData(const ComponentData& other) = delete;
public:
    ComponentData(const componentFunction function, const std::vector<int>& params );
    ~ComponentData();
    void render();

    const std::vector<int> lastParams;
    std::vector<ComponentData*> lastResult;
    const componentFunction function;
    std::shared_ptr<State> state;

    unsigned int hookCounter;
    std::vector<HookData> hookData;

    void* getHookData();
    void setHookData(void* data, void(*destroy)(const void*));
};

extern ComponentData* current;
extern ComponentData* parent;

// ComponentData* create(const componentFunction function, const std::vector<int> params);