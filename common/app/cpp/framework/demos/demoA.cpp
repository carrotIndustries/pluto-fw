#include "demoA.hpp"
volatile int x = 0;
State demoA(){
    auto test = useState<1, int>(7);
    auto alarm = useState<1,bool>(false);
    State s;
    
    const char value = alarm ? '1' : '0' ;
        s.text[0] = value;
        s.text[1] = value;
        s.text[2] = value;
        s.text[3] = value;
        s.text[4] = value;
        s.text[5] = value;
        s.text[6] = value;
        s.text[7] = value;
        s.text[8] = value;
        s.text[9] = value;

        s.render();
        return s;
}