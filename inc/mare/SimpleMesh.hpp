#ifndef SIMPLEMESH
#define SIMPLEMESH

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Mesh.hpp"
#include "mare/Buffer.hpp"
#include "mare/RenderState.hpp"

namespace mare
{
class SimpleMesh : public Mesh
{
public:
    SimpleMesh();
    ~SimpleMesh();

    void render(const Layer *layer, Material *material) override;
    void render(const Layer *layer, Material *material, const glm::mat4 &parent_model) override;
    void render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) override;

    inline void bind() const { render_state->bind(); }
    inline const Scoped<RenderState>& get_state() const { return render_state; }
    inline DrawMethod get_draw_method() const { return render_state->get_draw_method(); }

protected:
    Scoped<RenderState> render_state = nullptr;
};
} // namespace mare

#endif