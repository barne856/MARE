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
        std::vector<float> verts;
        set_draw_method(DrawMethod::TRIANGLES);

        verts.push_back(-0.5f);
        verts.push_back(-0.5f);
        verts.push_back(0.5f);
        verts.push_back(-0.5f);
        verts.push_back(0.5f);
        verts.push_back(0.5f);
        verts.push_back(0.5f);
        verts.push_back(0.5f);
        verts.push_back(-0.5f);
        verts.push_back(0.5f);
        verts.push_back(-0.5f);
        verts.push_back(-0.5f);

        Scoped<Buffer<float>> vertex_buffer = Renderer::API->GenBuffer<float>(&verts[0], verts.size()*sizeof(float));
        vertex_buffer->set_format({{Attribute::POSITION_2D, "position"}});

        add_geometry_buffer(std::move(vertex_buffer));
    }
    QuadrangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4)
    {
        std::vector<float> verts;
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

        Scoped<Buffer<float>> vertex_buffer = Renderer::API->GenBuffer<float>(&verts[0], verts.size()*sizeof(float));
        vertex_buffer->set_format({{Attribute::POSITION_2D, "position"}});

        add_geometry_buffer(std::move(vertex_buffer));
    }
};
} // namespace mare

#endif