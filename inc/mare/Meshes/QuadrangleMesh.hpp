#ifndef QUADRANGLEMESH
#define QUADRANGLEMESH

#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
    class QuadrangleMesh : public SimpleMesh
    {
    public:
        QuadrangleMesh()
        {
            std::vector<float> data;
            set_draw_method(DrawMethod::TRIANGLES);

            data.push_back(-0.5f);
            data.push_back(-0.5f);

            data.push_back(0.0f);
            data.push_back(0.0f);

            data.push_back(0.5f);
            data.push_back(-0.5f);

            data.push_back(1.0f);
            data.push_back(0.0f);

            data.push_back(0.5f);
            data.push_back(0.5f);

            data.push_back(1.0f);
            data.push_back(1.0f);

            data.push_back(0.5f);
            data.push_back(0.5f);

            data.push_back(1.0f);
            data.push_back(1.0f);

            data.push_back(-0.5f);
            data.push_back(0.5f);

            data.push_back(0.0f);
            data.push_back(1.0f);

            data.push_back(-0.5f);
            data.push_back(-0.5f);

            data.push_back(0.0f);
            data.push_back(0.0f);

            Scoped<Buffer<float>> geometry_buffer = Renderer::API->GenBuffer<float>(&data[0], data.size() * sizeof(float));
            geometry_buffer->set_format({{Attribute::POSITION_2D, "position"},
                                         {Attribute::TEXTURE_MAP, "texcoords"}});

            add_geometry_buffer(std::move(geometry_buffer));
        }
        QuadrangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4)
        {
            std::vector<float> verts;
            std::vector<float> tex_coords;
            set_draw_method(DrawMethod::TRIANGLES);

            verts.push_back(v1[0]);
            verts.push_back(v1[1]);
            verts.push_back(v2[0]);
            verts.push_back(v2[1]);
            verts.push_back(v3[0]);
            verts.push_back(v3[1]);
            verts.push_back(v3[0]);
            verts.push_back(v3[1]);
            verts.push_back(v4[0]);
            verts.push_back(v4[1]);
            verts.push_back(v1[0]);
            verts.push_back(v1[1]);

            tex_coords.push_back(0.0f);
            tex_coords.push_back(0.0f);
            tex_coords.push_back(1.0f);
            tex_coords.push_back(0.0f);
            tex_coords.push_back(1.0f);
            tex_coords.push_back(1.0f);
            tex_coords.push_back(0.0f);
            tex_coords.push_back(1.0f);
            tex_coords.push_back(0.0f);
            tex_coords.push_back(0.0f);

            Scoped<Buffer<float>> vertex_buffer = Renderer::API->GenBuffer<float>(&verts[0], verts.size() * sizeof(float));
            Scoped<Buffer<float>> tex_coord_buffer = Renderer::API->GenBuffer<float>(&tex_coords[0], tex_coords.size() * sizeof(float));
            vertex_buffer->set_format({{Attribute::POSITION_2D, "position"}});
            tex_coord_buffer->set_format({{Attribute::TEXTURE_MAP, "texcoords"}});

            add_geometry_buffer(std::move(vertex_buffer));
            add_geometry_buffer(std::move(tex_coord_buffer));
        }
    };
} // namespace mare

#endif