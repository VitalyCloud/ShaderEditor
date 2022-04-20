//
//  Layer.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#ifndef Layer_hpp
#define Layer_hpp

#include "Events/Event.hpp"

namespace Engine {
    
class Layer {
public:
    virtual ~Layer() = default;
    
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    
    virtual void OnUpdate() {}
    virtual void OnUIRender() {}
    
    virtual void OnEvent(Event& e) {}
};

}


#endif /* Layer_hpp */
