#include "mare/Material.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
Material::Material(const char *directory) : m_shader(Renderer::API->GenShader(directory))
{
    m_shader->use();
}
} // namespace mare