#ifndef RENDERSTATE
#define RENDERSTATE

// MARE
#include "mare/Buffer.hpp"

// External Libraries
#include "glm.hpp"

// Standard Library
#include <vector>

namespace mare
{
enum class DrawMethod
{
    NONE,
    POINTS,
    LINES,
    LINE_STRIP,
    LINE_LOOP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

class RenderState
{
public:
    RenderState() : m_render_state_ID(0), m_vertex_buffer_count(0), m_attribute_index(0), m_vertex_render_count(0), m_index_render_count(0), vertex_buffer(nullptr), index_buffer(nullptr) {}
    virtual ~RenderState()
    {
        if (index_buffer)
        {
            delete index_buffer;
        }
        if (vertex_buffer)
        {
            delete vertex_buffer;
        }
    }
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void set_vertex_buffer(Buffer<float> *vbo) = 0;
    virtual void set_index_buffer(Buffer<unsigned int> *ibo) = 0;
    inline unsigned int render_count() const
    {
        if (index_buffer)
        {
            return m_index_render_count;
        }
        else
        {
            return m_vertex_render_count;
        }
    }
    inline bool is_indexed() const { return index_buffer; }
    inline void set_draw_method(DrawMethod method) { draw_method = method; }
    inline DrawMethod get_draw_method() { return draw_method; }
    // swap the rendered buffer to the next buffer if vertex data is multibuffered
    void swap_buffer()
    {
        if (vertex_buffer->is_multibuffered())
        {
            vertex_buffer->swap_buffer();
        }
    }
    unsigned int get_render_index()
    {
        return vertex_buffer->get_buffer_index() * m_vertex_render_count;
    }
    void wait_buffer()
    {
        vertex_buffer->wait_buffer();
    }
    void lock_buffer()
    {
        vertex_buffer->lock_buffer();
    }

protected:
    unsigned int m_render_state_ID;
    unsigned int m_vertex_buffer_count;
    unsigned int m_attribute_index;
    unsigned int m_vertex_render_count;
    unsigned int m_index_render_count;
    DrawMethod draw_method;
    Buffer<float> *vertex_buffer;       // positions, normals, and texture coords
    Buffer<unsigned int> *index_buffer; // used as rendering order if not set to nullptr
};
} // namespace mare

#endif