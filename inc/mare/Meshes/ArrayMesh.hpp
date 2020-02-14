#ifndef ARRAYMESH
#define ARRAYMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

#include <iostream>

namespace mare
{

class ArrayMesh : public SimpleMesh
{
public:
    ArrayMesh(DrawMethod method, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texture_coords, std::vector<unsigned int> *indices)
    {
        if (normals.size() != vertices.size())
        {
            std::cerr << "normals must be the same size as vertices" << std::endl;
            return;
        }

        if (texture_coords.size() != vertices.size())
        {
            std::cerr << "texture_coords must be the same size as vertices" << std::endl;
            return;
        }

        // interleave data
        std::vector<float> vertex_data{};
        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertex_data.push_back(vertices[i][0]);
            vertex_data.push_back(vertices[i][1]);
            vertex_data.push_back(normals[i][0]);
            vertex_data.push_back(normals[i][1]);
            vertex_data.push_back(normals[i][2]);
            vertex_data.push_back(texture_coords[i][0]);
            vertex_data.push_back(texture_coords[i][1]);
        }
        Buffer<float>* vb = Renderer::API->GenFloatBuffer(&vertex_data);
        vb->set_format({{LinalgDataType::VEC3, "position"},
                                    {LinalgDataType::VEC3, "normals"},
                                    {LinalgDataType::VEC2, "texture_coords"}});
        render_state->set_vertex_buffer(vb);
        if (indices)
        {
            Buffer<unsigned int>* ib = Renderer::API->GenIndexBuffer(indices);
            render_state->set_index_buffer(ib);
        }
    }
};

} // namespace mare

#endif