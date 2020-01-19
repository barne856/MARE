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
template <typename T>
class RenderState
{
public:
    RenderState() : m_render_state_ID(0), m_vertex_buffer_count(0), m_attribute_index(0), m_vertex_render_count(0), m_index_render_count(0), m_is_indexed(false), vertex_buffers({}), index_buffer(nullptr) {}
    virtual ~RenderState()
    {
        for (auto &buffer : vertex_buffers)
        {
            if (buffer)
            {
                delete[] buffer;
            }
        }
        if(index_buffer)
        {
            delete[] index_buffer;
        }
    }
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void create() = 0;
    virtual void add_vertex_buffer(Buffer<T> *vbo) = 0;
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
    std::vector<Buffer<T> *> vertex_buffers;
    Buffer<unsigned int>* index_buffer;
};
} // namespace mare

#endif