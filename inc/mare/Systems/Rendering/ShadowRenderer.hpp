#ifndef SHADOWRENDERER
#define SHADOWRENDERER

// MARE
#include "mare/Components/Shadow.hpp"
#include "mare/Components/Transform.hpp"
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

class ShadowRenderer : public RenderSystem<Shadow> {
public:
  void render(float dt, Camera *camera, Shadow *sc) override {
    if (sc->light_view && sc->depth_buffer) {
      for (auto pack_it = sc->packets_begin(); pack_it != sc->packets_end();
           pack_it++) {
        auto mesh = (*pack_it).first;
        auto material = (*pack_it).second;
        material->bind();
        glm::mat4 shadow_matrix = sc->scale_bias_matrix *
                                  sc->light_view->projection() *
                                  sc->light_view->view();
        material->upload_mat4("shadow_matrix", shadow_matrix);
        material->upload_texture2D("depth_texture",
                                   sc->depth_buffer->depth_texture());

        mesh->render(camera, material.get(), sc->get_model());
      }
    }
  }
};
} // namespace mare
#endif