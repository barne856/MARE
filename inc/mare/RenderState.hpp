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
    RenderState() : m_render_state_ID(0), m_vertex_buffer_count(0), m_attribute_index(0), m_vertex_render_count(0), m_index_render_count(0), m_is_indexed(false), vertex_buffers({}), index_buffer(nullptr) {}
    virtual ~RenderState() {}
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void create() = 0;
    virtual void add_vertex_buffer(Buffer<float> *vbo) = 0;
    virtual void set_index_buffer(Buffer<unsigned int> *ibo) = 0;
    inline unsigned int render_count() const
    {
        if (m_is_indexed)
        {
            return m_index_render_count;
        }
        else
        {
            return m_vertex_render_count;
        }
    }
    inline bool is_indexed() const { return m_is_indexed; }
    inline void set_draw_method(DrawMethod method) { draw_method = method; }
    inline DrawMethod get_draw_method() { return draw_method; }

protected:
    unsigned int m_render_state_ID;
    unsigned int m_vertex_buffer_count;
    unsigned int m_attribute_index;
    unsigned int m_vertex_render_count;
    unsigned int m_index_render_count;
    bool m_is_indexed;
    DrawMethod draw_method;
    std::vector<Buffer<float> *> vertex_buffers;
    Buffer<unsigned int> *index_buffer;
};
} // namespace mare

#endif