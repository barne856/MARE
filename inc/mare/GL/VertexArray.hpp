#ifndef VERTEXARRAY
#define VERTEXARRAY
#include "mare/GL/GLBuffer.hpp"

namespace mare
{
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void create();
    void add_vertex_buffer(GLBuffer<float> *vbo);
    void set_index_buffer(GLBuffer<unsigned int> *ibo);
    size_t count() const;
    bool is_indexed() const;

private:
    unsigned int m_vertex_array_ID;
    unsigned int m_buffer_count;
    unsigned int m_attribute_index;
    size_t m_render_count;
    size_t m_indexed_render_count;
    bool m_is_indexed;
};
} // namespace mare

#endif