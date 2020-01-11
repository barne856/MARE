#include "mare/GL/GLMaterialFactory.hpp"
#include "mare/GL/Materials/GLBasicMaterial.hpp"

namespace mare
{
BasicMaterial *GLMaterialFactory::GenBasicMaterial()
{
    return new GLBasicMaterial();
}
} // namespace mare