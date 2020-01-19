#ifndef SIMPLEMESH
#define SIMPLEMESH

// MARE
#include "mare/Mesh.hpp"
#include "mare/Buffer.hpp"
#include "mare/RenderState.hpp"

namespace mare
{
template <typename T>
class SimpleMesh : public Mesh
{
public:
    SimpleMesh();
    ~SimpleMesh();

    void render(Material *material) override;
    void render(Material *material, glm::mat4 parent_model) override;
    void render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;

    inline void bind() const { render_state->bind(); }
    inline RenderState<T> *get_state() const { return render_state; }
    inline DrawMethod get_draw_method() const { return draw_method; }

protected:
    RenderState<T> *render_state = nullptr;
    Buffer<T> *vertex_buffer = nullptr;
    Buffer<unsigned int> *index_buffer = nullptr;
    DrawMethod draw_method;
};
} // namespace mare

#endif