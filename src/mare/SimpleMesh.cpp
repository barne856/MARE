#include "mare/SimpleMesh.hpp"

// MARE
#include "mare/Application.hpp"

namespace mare
{
template <typename T>
SimpleMesh<T>::SimpleMesh()
    : draw_method(DrawMethod::TRIANGLES), index_buffer(nullptr), vertex_buffers(nullptr)
{
    render_state = Application::GenRenderState<T>();
}
template <typename T>
SimpleMesh<T>::~SimpleMesh()
{
    if (index_buffer)
    {
        delete index_buffer;
    }
    if (vertex_buffers)
    {
        delete[] vertex_buffers;
    }
    delete render_state;
}
template <typename T>
void SimpleMesh<T>::render(Material *material)
{
    Application::render_mesh(this, material);
}
template <typename T>
void SimpleMesh<T>::render(Material *material, glm::mat4 parent_model)
{
    Application::render_mesh(this, material, parent_model);
}
template <typename T>
void SimpleMesh<T>::render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    Application::render_mesh(this, material, parent_model, instance_count, models);
}

template class SimpleMesh<float>;
template class SimpleMesh<glm::vec2>;
template class SimpleMesh<glm::vec3>;
template class SimpleMesh<glm::vec4>;
} // namespace mare