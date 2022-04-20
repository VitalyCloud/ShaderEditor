//
//  Layer.h
//  ShaderEditor
//
//  Created by Vitaly Cloud on 20.04.2022.
//

#ifndef Layer_h
#define Layer_h

namespace Editor {
    
class Layer {
public:
    virtual ~Layer() = default;
    
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    
    virtual void OnUpdate() {}
    virtual void OnUIRender() {}
};

}


#endif /* Layer_h */
