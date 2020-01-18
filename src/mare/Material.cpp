#include "mare/Material.hpp"
#include "mare/Application.hpp"

namespace mare
{
Material::Material(const char *directory) : m_shader(Application::GenShader(directory))
{
    m_shader->use();
}
} // namespace mare