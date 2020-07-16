// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

namespace mare {
SimpleMesh::SimpleMesh()
    : geometry_buffer_count(0), vertex_render_count(0), index_render_count(0) {}
SimpleMesh::~SimpleMesh() { Renderer::destroy_mesh_render_states(this); }
void SimpleMesh::render(Camera *camera, Material *material) {
  Renderer::render_simple_mesh(camera, this, material);
}
void SimpleMesh::render(Camera *camera, Material *material,
                        Transform *parent_transform) {
  Renderer::render_simple_mesh(camera, this, material, parent_transform);
}
void SimpleMesh::render(Camera *camera, Material *material,
                        Transform *parent_transform,
                        unsigned int instance_count,
                        Buffer<Transform> *models) {
  Renderer::render_simple_mesh(camera, this, material, parent_transform,
                               instance_count, models);
}
void SimpleMesh::bind(Material *material) {
  Renderer::bind_mesh_render_state(this, material);
}
void SimpleMesh::add_geometry_buffer(
    Referenced<Buffer<float>> geometry_buffer) {
  Renderer::push_mesh_geometry_buffer(this, geometry_buffer);
}
std::vector<Referenced<Buffer<float>>> SimpleMesh::get_geometry_buffers() {
  return geometry_buffers;
}
void SimpleMesh::set_index_buffer(Referenced<Buffer<uint32_t>> index_buffer) {
  Renderer::set_mesh_index_buffer(this, index_buffer);
}
Referenced<Buffer<uint32_t>> SimpleMesh::get_index_buffer() {
  return index_buffer;
}
size_t SimpleMesh::render_count() const {
  if (index_buffer) {
    return index_render_count;
  } else {
    return vertex_render_count;
  }
}
void SimpleMesh::invalidate_render_state_cache() {
  Renderer::destroy_mesh_render_states(this);
  render_states.clear();
}
bool SimpleMesh::is_indexed() const { return static_cast<bool>(index_buffer); }
void SimpleMesh::set_draw_method(DrawMethod method) { draw_method_ = method; }
DrawMethod SimpleMesh::get_draw_method() { return draw_method_; }
// swap the rendered buffer to the next buffer if vertex data is multibuffered
void SimpleMesh::swap_buffers() {
  for (auto &buffer : geometry_buffers) {
    buffer->swap_buffer();
  }
}
unsigned int SimpleMesh::get_render_index() {
  return geometry_buffers[0]->buffer_index() *
         static_cast<unsigned int>(vertex_render_count);
}
void SimpleMesh::wait_buffers() {
  for (auto &buffer : geometry_buffers) {
    buffer->wait_buffer();
  }
}
void SimpleMesh::lock_buffers() {
  for (auto &buffer : geometry_buffers) {
    buffer->lock_buffer();
  }
}

void CompositeMesh::render(Camera *camera, Material *material) {
  for (auto &mesh : meshes_) {
    mesh->render(camera, material, this);
  }
}

void CompositeMesh::render(Camera *camera, Material *material,
                           Transform *parent_transform) {
  for (auto &mesh : meshes_) {
    Transform trans{};
    trans.set_transformation_matrix(
        parent_transform->get_transformation_matrix() *
        get_transformation_matrix());
    mesh->render(camera, material, &trans);
  }
}

void CompositeMesh::render(Camera *camera, Material *material,
                           Transform *parent_transform,
                           unsigned int instance_count,
                           Buffer<Transform> *models) {
  for (auto &mesh : meshes_) {
    Transform trans{};
    trans.set_transformation_matrix(
        parent_transform->get_transformation_matrix() *
        get_transformation_matrix());
    mesh->render(camera, material, &trans, instance_count, models);
  }
}

void CompositeMesh::push_mesh(Referenced<Mesh> mesh) {
  meshes_.push_back(mesh);
}

void CompositeMesh::pop_mesh() { meshes_.pop_back(); }

void CompositeMesh::clear() { meshes_.clear(); }

InstancedMesh::InstancedMesh(unsigned int max_instances)
    : instance_count_(0), instance_transforms_(nullptr), mesh_(nullptr),
      max_instances_(max_instances) {
  instance_transforms_ = Renderer::gen_buffer<Transform>(
      nullptr, max_instances * sizeof(Transform), BufferType::READ_WRITE);
}

void InstancedMesh::set_mesh(Referenced<Mesh> mesh) { mesh_ = mesh; }

void InstancedMesh::push_instance(Transform model) {
  (*instance_transforms_)[instance_count_] = model;
  instance_count_++;
}

void InstancedMesh::pop_instance() { instance_count_--; }

void InstancedMesh::clear_instances() { instance_count_ = 0; }

void InstancedMesh::flush_instances(Transform *models, uint32_t offset,
                                    uint32_t count) {
  instance_transforms_->flush(models, offset, count * sizeof(Transform));
}

Transform &InstancedMesh::operator[](unsigned int i) {
  return (*instance_transforms_)[i];
}

Transform InstancedMesh::operator[](unsigned int i) const {
  return (*instance_transforms_)[i];
}

void InstancedMesh::render(Camera *camera, Material *material) {
  mesh_->render(camera, material, this, instance_count_,
                instance_transforms_.get());
}

void InstancedMesh::render(Camera *camera, Material *material,
                           Transform *parent_transform) {
  Transform trans{};
  trans.set_transformation_matrix(
      parent_transform->get_transformation_matrix() *
      get_transformation_matrix());
  mesh_->render(camera, material, &trans, instance_count_,
                instance_transforms_.get());
}

void InstancedMesh::render(Camera *camera, Material *material,
                           Transform *parent_transform,
                           unsigned int instance_count,
                           Buffer<Transform> *models) {
  // rendering an instanced mesh of instanced meshes is a bad idea. It will not
  // reduce draw calls and it will read from the models buffer in a very
  // ineffiecnt way, only render instances of simple meshes or composite meshes
  // consisting only of simple meshes in their mesh trees.
  for (unsigned int i = 0; i < instance_count; i++) {
    mesh_->render(camera, material, &(*models)[i]);
  }
}

Buffer<Transform> *InstancedMesh::get_instance_models() {
  return instance_transforms_.get();
}

Referenced<Buffer<Transform>>
InstancedMesh::swap_instance_models(Referenced<Buffer<Transform>> models) {
  models.swap(instance_transforms_);
  return models;
}

void InstancedMesh::set_instance_models(Referenced<Buffer<Transform>> models) {
  instance_transforms_ = models;
}

void InstancedMesh::set_instance_render_count(unsigned int count) {
  instance_count_ = std::min(max_instances_, count);
}

} // namespace mare