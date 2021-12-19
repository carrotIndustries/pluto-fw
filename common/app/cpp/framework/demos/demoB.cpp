#include "demoB.hpp"

std::vector<ComponentData*> zeroes(State* state, const std::vector<int>& params){
    auto mode = useGlobalState<3,bool>(false);
        state->text[0] = mode ? '1' : '0';
        state->text[1] = '0';
        state->text[2] = '0';
        state->text[3] = '1';
        state->text[4] = '2';

    return {};
}

std::vector<ComponentData*> ones(State* state, const std::vector<int>& params){
    auto mode = useGlobalState<3,bool>(false);
        state->text[0] = mode ? '1' : '0';
        state->text[1] = '1';
        state->text[2] = '1';
        state->text[3] = '0';
        state->text[4] = '2';

    return {};
}

std::vector<ComponentData*> showAlarm(State* state, const std::vector<int>& params){
    auto mode = useGlobalState<2,bool>(false);
    int position = params[0];
    state->text[position] = mode ? '1' : '0';

    return {};
}

std::vector<ComponentData*> demoB(State* state, const std::vector<int>& params){
    volatile int x = 0;
    auto test = useState<int>(7);
    auto alarm = useGlobalState<1,bool>(false);

    
    const char value = '2' ;
    state->text[0] = value;
    state->text[1] = value;
    state->text[2] = value;
    state->text[3] = value;
    state->text[4] = value;
    state->text[5] = value;
    state->text[6] = value;
    state->text[7] = value;
    state->text[8] = value;
    state->text[9] = value;
    state->backlight = true;

    return { new ComponentData(alarm ? zeroes : ones, {}), new ComponentData(showAlarm, {1}), new ComponentData(showAlarm, {2}) };
}