// MARE
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
SimpleMesh::SimpleMesh()
    : geometry_buffer_count(0), vertex_render_count(0), index_render_count(0)
{
}
SimpleMesh::~SimpleMesh()
{
    Renderer::API->destroy_mesh_render_states(this);
}
void SimpleMesh::render(Layer *layer, Material *material)
{
    Renderer::API->render_simple_mesh(layer, this, material);
}
void SimpleMesh::render(Layer *layer, Material *material, glm::mat4 &parent_model)
{
    Renderer::API->render_simple_mesh(layer, this, material, parent_model);
}
void SimpleMesh::render(Layer *layer, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    Renderer::API->render_simple_mesh(layer, this, material, parent_model, instance_count, models);
}
void SimpleMesh::bind(Material *material)
{
    Renderer::API->bind_mesh_render_state(this, material);
}
void SimpleMesh::add_geometry_buffer(Scoped<Buffer<float>> geometry_buffer)
{
    Renderer::API->push_mesh_geometry_buffer(this, std::move(geometry_buffer));
}
void SimpleMesh::set_index_buffer(Scoped<Buffer<uint32_t>> index_buffer)
{
    Renderer::API->set_mesh_index_buffer(this, std::move(index_buffer));
}
size_t SimpleMesh::render_count() const
{
    if (index_buffer)
    {
        return index_render_count;
    }
    else
    {
        return vertex_render_count;
    }
}
void SimpleMesh::invalidate_render_state_cache()
{
    render_states.clear();
}
bool SimpleMesh::is_indexed() const { return static_cast<bool>(index_buffer); }
void SimpleMesh::set_draw_method(DrawMethod method) { draw_method_ = method; }
DrawMethod SimpleMesh::get_draw_method() { return draw_method_; }
// swap the rendered buffer to the next buffer if vertex data is multibuffered
void SimpleMesh::swap_buffers()
{
    for (auto &buffer : geometry_buffers)
    {
        buffer->swap_buffer();
    }
}
unsigned int SimpleMesh::get_render_index()
{
    return geometry_buffers[0]->buffer_index() * static_cast<unsigned int>(vertex_render_count);
}
void SimpleMesh::wait_buffers()
{
    for (auto &buffer : geometry_buffers)
    {
        buffer->wait_buffer();
    }
}
void SimpleMesh::lock_buffers()
{
    for (auto &buffer : geometry_buffers)
    {
        buffer->lock_buffer();
    }
}

void CompositeMesh::render(Layer *layer, Material *material)
{
    for (auto &mesh : meshes_)
    {
        mesh->render(layer, material, transform_);
    }
}

void CompositeMesh::render(Layer *layer, Material *material, glm::mat4 &parent_model)
{
    for (auto &mesh : meshes_)
    {
        mesh->render(layer, material, parent_model * transform_);
    }
}

void CompositeMesh::render(Layer *layer, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    for (auto &mesh : meshes_)
    {
        mesh->render(layer, material, parent_model * transform_, instance_count, models);
    }
}

void CompositeMesh::push_mesh(Scoped<Mesh> mesh)
{
    meshes_.push_back(std::move(mesh));
}

void CompositeMesh::pop_mesh()
{
    meshes_.pop_back();
}

void CompositeMesh::clear()
{
    meshes_.clear();
}

InstancedMesh::InstancedMesh(unsigned int max_instances)
    : instance_count_(0), instance_transforms_(nullptr), mesh_(nullptr), max_instances_(max_instances)
{
    instance_transforms_ = Renderer::API->GenBuffer<glm::mat4>(nullptr, max_instances * sizeof(glm::mat4), BufferType::READ_WRITE);
    instance_transforms_->set_format({{Attribute::STORAGE, "model_instances"}});
}

void InstancedMesh::set_mesh(Scoped<Mesh> mesh)
{
    mesh_ = std::move(mesh);
}

void InstancedMesh::push_instance(glm::mat4 model)
{
    (*instance_transforms_)[instance_count_] = model;
    instance_count_++;
}

void InstancedMesh::pop_instance()
{
    instance_count_--;
}

void InstancedMesh::clear_instances()
{
    instance_count_ = 0;
}

void InstancedMesh::flush_instances(std::vector<glm::mat4> *models, size_t offset)
{
    instance_transforms_->flush(&(models->front()), offset, models->size() * sizeof(glm::mat4));
}

glm::mat4 &InstancedMesh::operator[](unsigned int i)
{
    return (*instance_transforms_)[i];
}

glm::mat4 InstancedMesh::operator[](unsigned int i) const
{
    return (*instance_transforms_)[i];
}

void InstancedMesh::render(Layer *layer, Material *material)
{
    mesh_->render(layer, material, transform_, instance_count_, instance_transforms_.get());
}

void InstancedMesh::render(Layer *layer, Material *material, glm::mat4 &parent_model)
{
    mesh_->render(layer, material, parent_model * transform_, instance_count_, instance_transforms_.get());
}

void InstancedMesh::render(Layer *layer, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    // rendering an instanced mesh of instanced meshes is a bad idea. It will not reduce draw calls and it will
    // read from the models buffer in a very ineffiecnt way, only render instances of simple meshes or composite meshes consisting only of simple meshes in their mesh trees.
    for (unsigned int i = 0; i < instance_count; i++)
    {
        mesh_->render(layer, material, (*models)[i]);
    }
}

} // namespace mare