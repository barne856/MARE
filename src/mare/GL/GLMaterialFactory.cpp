#include "mare/GL/GLMaterialFactory.hpp"
#include "mare/GL/Materials/GLBasicMaterial.hpp"

namespace mare
{
BasicMaterial *GLMaterialFactory::GenBasicMaterial()
{
    return new GLBasicMaterial();
}
Material* GLMaterialFactory::GenMaterial(const char* directory)
{
    return new GLMaterial(directory);
}
} // namespace mare