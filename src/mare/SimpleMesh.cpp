#include "mare/SimpleMesh.hpp"

// MARE
#include "mare/Renderer.hpp"

namespace mare
{
SimpleMesh::SimpleMesh()
    : index_buffer(nullptr), vertex_buffers(nullptr)
{
    render_state = Renderer::API->GenRenderState();
    render_state->set_draw_method(DrawMethod::TRIANGLES);
}
SimpleMesh::~SimpleMesh()
{
    if (index_buffer)
    {
        delete[] index_buffer;
    }
    if (vertex_buffers)
    {
        delete[] vertex_buffers;
    }
    delete render_state;
}
void SimpleMesh::render(Layer* layer, Material *material)
{
    Renderer::API->render_simple_mesh(layer, this, material);
}
void SimpleMesh::render(Layer* layer, Material *material, glm::mat4 parent_model)
{
    Renderer::API->render_simple_mesh(layer, this, material, parent_model);
}
void SimpleMesh::render(Layer* layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    Renderer::API->render_simple_mesh(layer, this, material, parent_model, instance_count, models);
}
} // namespace mare