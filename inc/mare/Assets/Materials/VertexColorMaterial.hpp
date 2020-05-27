#ifndef VERTEXCOLORMATERIAL
#define VERTEXCOLORMATERIAL

// MARE
#include "mare/Shader.hpp"

namespace mare
{
    class VertexColorMaterial : public Material
    {
        public:
        VertexColorMaterial() : Material("./inc/mare/Assets/Shaders/VertexColor") {}
    };
}

#endif