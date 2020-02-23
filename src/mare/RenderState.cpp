// MARE
#include "mare/RenderState.hpp"

namespace mare
{
RenderState::RenderState() : render_state_ID_(0), vertex_buffer_count_(0), attribute_index_(0), vertex_render_count_(0), index_render_count_(0), vertex_buffer_(nullptr), index_buffer_(nullptr) {}
RenderState::~RenderState() {}

size_t RenderState::render_count() const
{
    if (index_buffer_)
    {
        return index_render_count_;
    }
    else
    {
        return vertex_render_count_;
    }
}
bool RenderState::is_indexed() const { return static_cast<bool>(index_buffer_); }
void RenderState::set_draw_method(DrawMethod method) { draw_method_ = method; }
DrawMethod RenderState::get_draw_method() { return draw_method_; }
// swap the rendered buffer to the next buffer if vertex data is multibuffered
void RenderState::swap_buffer()
{
    if (vertex_buffer_->is_multibuffered())
    {
        vertex_buffer_->swap_buffer();
    }
}
unsigned int RenderState::get_render_index()
{
    return vertex_buffer_->get_buffer_index() * static_cast<unsigned int>(vertex_render_count_);
}
void RenderState::wait_buffer()
{
    vertex_buffer_->wait_buffer();
}
void RenderState::lock_buffer()
{
    vertex_buffer_->lock_buffer();
}
} // namespace mare