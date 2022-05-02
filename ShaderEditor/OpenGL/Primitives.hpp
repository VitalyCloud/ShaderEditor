//
//  Primitives.hpp
//  ShaderEditor
//
//  Created by Vitaly Cloud on 02.05.2022.
//

#ifndef Primitives_hpp
#define Primitives_hpp

namespace OpenGL {

enum class Primitive {
    Points = 0,
    
    Lines,
    LineStrip,
    LineLoop,
    
    Triangles,
    TriangleStrip,
    TrinagleFan
};



}



#endif /* Primitives_hpp */
