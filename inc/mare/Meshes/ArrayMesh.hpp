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
    ArrayMesh(DrawMethod method, std::vector<T> &vertices, std::vector<T> &normals, std::vector<unsigned int> &indices)
    {
        render_state->set_draw_method(method);

        vertex_buffers = Application::GenBuffer<T>(2);
        vertex_buffers[0].create(vertices, vertices.size()*sizeof(T));
        vertex_buffers[0].set_format({{glm_to_shader_type<T>(), "position"}});
        vertex_buffers[1].create(normals, vertices.size()*sizeof(T));
        vertex_buffers[1].set_format({{glm_to_shader_type<T>(), "normals"}});

        render_state->create();
        render_state->add_vertex_buffer(&vertex_buffers[0]);
        render_state->add_vertex_buffer(&vertex_buffers[1]);

        index_buffer = Application::GenBuffer<unsigned int>(1);
        index_buffer->create(indices);
        render_state->set_index_buffer(index_buffer);
    }
    ArrayMesh(DrawMethod method, std::vector<T> &vertices, std::vector<T> &normals)
    {
        draw_method = method;

        vertex_buffers = Application::GenBuffer<T>(2);
        vertex_buffers[0].create(vertices, vertices.size()*sizeof(T));
        vertex_buffers[0].set_format({{glm_to_shader_type<T>(), "position"}});
        vertex_buffers[1].create(normals, vertices.size()*sizeof(T));
        vertex_buffers[1].set_format({{glm_to_shader_type<T>(), "normals"}});

        render_state->create();
        render_state->add_vertex_buffer(&vertex_buffers[0]);
        render_state->add_vertex_buffer(&vertex_buffers[1]);
    }
    ArrayMesh(DrawMethod method, std::vector<T> &vertices)
    {
        draw_method = method;

        vertex_buffers = Application::GenBuffer<T>(1);
        vertex_buffers->create(vertices);
        vertex_buffers->set_format({{glm_to_shader_type<T>(), "position"}});

        render_state->create();
        render_state->add_vertex_buffer(vertex_buffers);
    }
};

} // namespace mare

#endif