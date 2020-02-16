// MARE
#include "mare/RenderState.hpp"

namespace mare
{
RenderState::RenderState() : m_render_state_ID(0), m_vertex_buffer_count(0), m_attribute_index(0), m_vertex_render_count(0), m_index_render_count(0), vertex_buffer(nullptr), index_buffer(nullptr) {}
RenderState::~RenderState()
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

size_t RenderState::render_count() const
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
bool RenderState::is_indexed() const { return index_buffer; }
void RenderState::set_draw_method(DrawMethod method) { draw_method = method; }
DrawMethod RenderState::get_draw_method() { return draw_method; }
// swap the rendered buffer to the next buffer if vertex data is multibuffered
void RenderState::swap_buffer()
{
    if (vertex_buffer->is_multibuffered())
    {
        vertex_buffer->swap_buffer();
    }
}
unsigned int RenderState::get_render_index()
{
    return vertex_buffer->get_buffer_index() * static_cast<unsigned int>(m_vertex_render_count);
}
void RenderState::wait_buffer()
{
    vertex_buffer->wait_buffer();
}
void RenderState::lock_buffer()
{
    vertex_buffer->lock_buffer();
}
} // namespace mare