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
    RenderState();
    virtual ~RenderState();

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void set_vertex_buffer(Scoped<Buffer<float>> vbo) = 0;
    virtual void set_index_buffer(Scoped<Buffer<unsigned int>> ibo) = 0;
    
    size_t render_count() const;
    bool is_indexed() const;
    void set_draw_method(DrawMethod method);
    DrawMethod get_draw_method();
    // swap the rendered buffer to the next buffer if vertex data is multibuffered
    void swap_buffer();
    unsigned int get_render_index();
    void wait_buffer();
    void lock_buffer();

protected:
    unsigned int render_state_ID_;
    size_t vertex_buffer_count_;
    unsigned int attribute_index_;
    size_t vertex_render_count_;
    size_t index_render_count_;
    DrawMethod draw_method_;
    Scoped<Buffer<float>> vertex_buffer_;       // positions, normals, and texture coords
    Scoped<Buffer<unsigned int>> index_buffer_; // used as rendering order if not set to nullptr
};
} // namespace mare

#endif