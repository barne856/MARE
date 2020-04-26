#include "mare/Material.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
Material::Material(const char *directory)
{
    shader_ = Renderer::API->GenShader(directory);
    shader_->use();
}
} // namespace mare