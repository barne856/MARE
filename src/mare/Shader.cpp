#include "mare/Shader.hpp"
#include "mare/Entities/Camera.hpp"
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
    std::unordered_map<std::string, Referenced<Shader>> ShaderProgram::shader_cache_ = std::unordered_map<std::string, Referenced<Shader>>();

    ShaderProgram::ShaderProgram(const char *directory)
    {
        if (!shader_cache_.count(std::string(directory)))
        {
            // create shader and cache the location
            shader_ = Renderer::gen_shader(directory);
            shader_cache_.insert({std::string(directory), shader_});
        }
        else
        {
            shader_ = shader_cache_[std::string(directory)];
        }
    }

    void ComputeProgram::dispatch_compute(uint32_t x, uint32_t y, uint32_t z)
    {
        compute();
        Renderer::dispatch_compute(x, y, z);
    }
    void ComputeProgram::barrier(BarrierType type)
    {
        shader_->barrier(type);
    }

    void Material::upload_camera(Camera *camera, bool suppress_warnings)
    {
        shader_->upload_mat4("projection", camera->projection(), suppress_warnings);
        shader_->upload_mat4("view", camera->view(), suppress_warnings);
    }
    void Material::upload_mesh_model_matrix(Mesh *mesh, bool suppress_warnings)
    {
        shader_->upload_mat4("model", mesh->get_model(), suppress_warnings);
    }
    void Material::upload_mesh_model_matrix(Mesh *mesh, glm::mat4 parent_model, bool suppress_warnings)
    {
        shader_->upload_mat4("model", parent_model * mesh->get_model(), suppress_warnings);
    }
    void Material::upload_mesh_normal_matrix(Mesh *mesh, bool suppress_warnings)
    {
        shader_->upload_mat3("normal_matrix", glm::mat3(mesh->get_normal()), suppress_warnings);
    }
    void Material::upload_mesh_normal_matrix(Mesh *mesh, glm::mat4 parent_model, bool suppress_warnings)
    {
        shader_->upload_mat3("normal_matrix", glm::transpose(glm::inverse(glm::mat3(parent_model * (mesh->get_model())))), suppress_warnings);
    }
    void Material::upload_mesh_instance_matrices(Buffer<glm::mat4> *models, bool suppress_warnings)
    {
        shader_->upload_storage("model_instances", models, suppress_warnings);
    }
    void Material::upload_mesh(Mesh *mesh, bool suppress_warnings)
    {
        upload_mesh_model_matrix(mesh, suppress_warnings);
        upload_mesh_normal_matrix(mesh, suppress_warnings);
    }
    void Material::upload_mesh(Mesh *mesh, glm::mat4 parent_model, bool suppress_warnings)
    {
        upload_mesh_model_matrix(mesh, parent_model, suppress_warnings);
        upload_mesh_normal_matrix(mesh, parent_model, suppress_warnings);
    }
    void Material::upload_mesh(Mesh *mesh, glm::mat4 parent_model, Buffer<glm::mat4> *models, bool suppress_warnings)
    {
        upload_mesh_model_matrix(mesh, parent_model, suppress_warnings);
        upload_mesh_normal_matrix(mesh, parent_model, suppress_warnings);
        upload_mesh_instance_matrices(models, suppress_warnings);
    }
} // namespace mare