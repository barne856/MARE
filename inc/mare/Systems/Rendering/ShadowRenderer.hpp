#ifndef SHADOWRENDERER
#define SHADOWRENDERER

// MARE
#include "mare/Components/Shadow.hpp"
#include "mare/Components/Transform.hpp"
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

/**
 * @brief A RenderSystem that operates on Shadow Components.
 * @details All Entities that have a ShadowRenderer on their System stack will
 * render with shadows cast on them by other Entities in the Scene. The Scene
 * must have a ShadowMap System and the Entitiy must inherit from the Shadow
 * Component.
 * @see Shadow
 * @see ShadowMap
 */
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
                                  sc->light_view->get_projection() *
                                  sc->light_view->get_transformation_matrix();
        material->upload_mat4("shadow_matrix", shadow_matrix);
        material->upload_texture2D("depth_texture",
                                   sc->depth_buffer->depth_texture());

        mesh->render(camera, material.get(), sc->get_transformation_matrix());
      }
    }
  }
};
} // namespace mare
#endif