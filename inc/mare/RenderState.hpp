#ifndef RENDERSTATE
#define RENDERSTATE

// MARE
#include "mare/Buffer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{
class RenderState
{
public:
    RenderState() : m_render_state_ID(0), m_vertex_buffer_count(0), m_attribute_index(0), m_vertex_render_count(0), m_index_render_count(0), m_is_indexed(false) {}
    virtual ~RenderState() {}
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void create() = 0;
    virtual void add_vertex_buffer(Buffer<float> *vbo) = 0;
    virtual void add_vertex_buffer(Buffer<glm::vec2> *vbo) = 0;
    virtual void add_vertex_buffer(Buffer<glm::vec3> *vbo) = 0;
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

protected:
    unsigned int m_render_state_ID;
    unsigned int m_vertex_buffer_count;
    unsigned int m_attribute_index;
    unsigned int m_vertex_render_count;
    unsigned int m_index_render_count;
    bool m_is_indexed;
};
} // namespace mare

#endif