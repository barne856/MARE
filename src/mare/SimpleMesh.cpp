#include "mare/SimpleMesh.hpp"

// MARE
#include "mare/Renderer.hpp"

namespace mare
{
SimpleMesh::SimpleMesh()
{
    render_state = Renderer::API->GenRenderState();
    render_state->set_draw_method(DrawMethod::TRIANGLES);
}
SimpleMesh::~SimpleMesh() {}
void SimpleMesh::render(const Layer *layer, Material *material)
{
    Renderer::API->render_simple_mesh(layer, this, material);
}
void SimpleMesh::render(const Layer *layer, Material *material, const glm::mat4 &parent_model)
{
    Renderer::API->render_simple_mesh(layer, this, material, parent_model);
}
void SimpleMesh::render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models)
{
    Renderer::API->render_simple_mesh(layer, this, material, parent_model, instance_count, models);
}
} // namespace mare