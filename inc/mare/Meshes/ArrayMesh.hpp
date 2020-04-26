#ifndef ARRAYMESH
#define ARRAYMESH

#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

#include <iostream>

namespace mare
{

class ArrayMesh : public SimpleMesh
{
public:
    ArrayMesh(DrawMethod method, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texture_coords, std::vector<uint32_t> *indices)
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
        Scoped<Buffer<float>> vb = Renderer::API->GenBuffer<float>(&vertex_data[0], vertex_data.size()*sizeof(float));
        vb->set_format({{Attribute::POSITON_3D, "position"},
                                    {Attribute::NORMAL, "normals"},
                                    {Attribute::TEXTURE_MAP, "texture_coords"}});
        add_geometry_buffer(std::move(vb));
        if (indices)
        {
            Scoped<Buffer<uint32_t>> ib = Renderer::API->GenBuffer<uint32_t>(&(indices->front()), indices->size()*sizeof(uint32_t));
            set_index_buffer(std::move(ib));
        }
        set_draw_method(method);
    }
};

} // namespace mare

#endif