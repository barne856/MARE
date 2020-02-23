#include "mare/CompositeMesh.hpp"

namespace mare
{
void CompositeMesh::render(const Layer *layer, Material *material)
{
    for (auto &mesh : meshes_)
    {
        mesh->render(layer, material, transform_);
    }
}

void CompositeMesh::render(const Layer *layer, Material *material, const glm::mat4 &parent_model)
{
    for (auto &mesh : meshes_)
    {
        mesh->render(layer, material, parent_model * transform_);
    }
}

void CompositeMesh::render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models)
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

} // namespace mare