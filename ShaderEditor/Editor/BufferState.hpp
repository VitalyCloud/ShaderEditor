//
//  BufferState.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 01.05.2022.
//

#ifndef BufferState_hpp
#define BufferState_hpp

namespace Editor {

#define BIT(x) (1 << x)

template<typename T>
class BufferState {
public:
    using StateType = T;
    
    void Set(StateType state) {
        State |= (uint8_t)state;
    }
    
    void Reset() {
        State = 0;
    }
    
    template<typename StateType>
    bool CheckIf(StateType state) const {
        if(State & (uint8_t) state)
            return true;
        return false;
    }
    
    template<typename StateType, typename... Args>
    bool CheckIf(StateType state, Args... args) const {
        if(CheckIf(state))
            return true;
        else
            return CheckIf(args...);
        return false;
    }
    
private:
    uint8_t State = 0;
};

}

#endif /* BufferState_hpp */
