#include "Shader.hpp"
#include "Entities/Camera.hpp"
#include "Meshes.hpp"
#include "Renderer.hpp"

namespace mare {
std::unordered_map<std::string, Referenced<Shader>>
    ShaderProgram::shader_cache_ =
        std::unordered_map<std::string, Referenced<Shader>>();

ShaderProgram::ShaderProgram(const char *directory) {
  if (!shader_cache_.count(std::string(directory))) {
    // create shader and cache the location
    shader_ = Renderer::gen_shader(directory);
    shader_cache_.insert({std::string(directory), shader_});
  } else {
    shader_ = shader_cache_[std::string(directory)];
  }
}

void ComputeProgram::dispatch_compute(uint32_t x, uint32_t y, uint32_t z) {
  compute();
  Renderer::dispatch_compute(x, y, z);
}
void ComputeProgram::barrier(BarrierType type) { shader_->barrier(type); }

void Material::upload_camera(Camera *camera, bool suppress_warnings) {
  shader_->upload_mat4("projection", camera->get_projection(),
                       suppress_warnings);
  shader_->upload_mat4("view", camera->get_view_matrix(), suppress_warnings);
}
void Material::upload_mesh_model_matrix(Mesh *mesh, bool suppress_warnings) {
  shader_->upload_mat4("model", mesh->get_transformation_matrix(),
                       suppress_warnings);
}
void Material::upload_mesh_model_matrix(Mesh *mesh, Transform *parent_transform,
                                        bool suppress_warnings) {
  shader_->upload_mat4(
      "model", ((*parent_transform) * (*mesh)).get_transformation_matrix(),
      suppress_warnings);
}
void Material::upload_mesh_normal_matrix(Mesh *mesh, bool suppress_warnings) {
  shader_->upload_mat3("normal_matrix", glm::mat3(mesh->get_normal_matrix()),
                       suppress_warnings);
}
void Material::upload_mesh_normal_matrix(Mesh *mesh,
                                         Transform *parent_transform,
                                         bool suppress_warnings) {
  shader_->upload_mat3(
      "normal_matrix",
      glm::transpose(glm::inverse(glm::mat3(
          ((*parent_transform) * (*mesh)).get_transformation_matrix()))),
      suppress_warnings);
}
void Material::upload_mesh_instance_matrices(Buffer<Transform> *models,
                                             bool suppress_warnings) {
  shader_->upload_storage("model_instances", models, suppress_warnings);
}
void Material::upload_mesh(Mesh *mesh, bool suppress_warnings) {
  upload_mesh_model_matrix(mesh, suppress_warnings);
  upload_mesh_normal_matrix(mesh, suppress_warnings);
}
void Material::upload_mesh(Mesh *mesh, Transform* parent_transform,
                           bool suppress_warnings) {
  upload_mesh_model_matrix(mesh, parent_transform, suppress_warnings);
  upload_mesh_normal_matrix(mesh, parent_transform, suppress_warnings);
}
void Material::upload_mesh(Mesh *mesh, Transform* parent_transform,
                           Buffer<Transform> *models, bool suppress_warnings) {
  upload_mesh_model_matrix(mesh, parent_transform, suppress_warnings);
  upload_mesh_normal_matrix(mesh, parent_transform, suppress_warnings);
  upload_mesh_instance_matrices(models, suppress_warnings);
}
} // namespace mare