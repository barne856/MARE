#ifndef VERTEXCOLORMATERIAL
#define VERTEXCOLORMATERIAL

// MARE
#include "Shader.hpp"

namespace mare {
/**
 * @brief A simple Material that will color a Mesh using vertex colors provided
 * in a Vertex Buffer of the Mesh.
 *
 */
class VertexColorMaterial : public Material {
public:
  VertexColorMaterial() : Material("./MARE/res/Shaders/VertexColor") {}
};
} // namespace mare

#endif