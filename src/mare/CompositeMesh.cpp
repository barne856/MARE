#include "mare/CompositeMesh.hpp"
#include "mare/Application.hpp"

namespace mare
{

 CompositeMesh::~CompositeMesh()
 {
     for(auto& mesh : m_meshes)
     {
         if(mesh)
         {
             delete mesh;
         }
     }
 }

void CompositeMesh::render(Layer* layer, Material *material)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(layer, material, transform);
    }
}

void CompositeMesh::render(Layer* layer, Material *material, glm::mat4 parent_model)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(layer, material, parent_model * transform);
    }
}

void CompositeMesh::render(Layer* layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4>* models)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(layer, material, parent_model * transform, instance_count, models);
    }
}

} // namespace mare