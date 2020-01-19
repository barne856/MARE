#ifndef ARRAYMESH
#define ARRAYMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{

template <typename T>
class ArrayMesh : public SimpleMesh<T>
{
public:
    ArrayMesh(DrawMethod method, Buffer<T> *vertices, Buffer<T> *normals = nullptr, Buffer<unsigned int> *indices = nullptr)
    {
        draw_method = method;

        if (normals)
        {
            vertex_buffer = Application::GenBuffer<T>(2);
            vertex_buffer[0]->create(vertices);
            vertex_buffer[0]->set_format({{glm_to_shader_type<T>(), "position"}});
            vertex_buffer[1]->create(normals);
            vertex_buffer[1]->set_format({{glm_to_shader_type<T>(), "normals"}});

            render_state->create();
            render_state->add_vertex_buffer(vertex_buffer[0]);
            render_state->add_vertex_buffer(vertex_buffer[1]);
        }
        else
        {
            vertex_buffer = Application::GenBuffer<T>(1);
            vertex_buffer->create(vertices);
            vertex_buffer->set_format({{glm_to_shader_type<T>(), "position"}});

            render_state->create();
            render_state->add_vertex_buffer(vertex_buffer);
        }

        if (indices)
        {
            index_buffer = Application::GenBuffer<unsigned int>(1);
            index_buffer->create(indices);
            render_state->set_index_buffer(index_buffer);
        }
    }
};

} // namespace mare

#endif