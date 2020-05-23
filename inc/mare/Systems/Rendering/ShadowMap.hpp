#ifndef SHADOWMAP
#define SHADOWMAP

// MARE
#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Components/Shadow.hpp"
#include "mare/Entities/Camera.hpp"
#include "mare/Entities/Spotlight.hpp"
#include "mare/Mare.hpp"
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Systems.hpp"

// External Libraries
#include "glm.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare {
/**
 * @brief A RenderSystem that operates on a Scene.
 * @details A ShadowMap System will create a shadow-map for the Entites in a
 * Scene that inherit from the Shadow Component. All Entities in the Scene that
 * inherit from the Shadow Component will cast shadows in the Scene and all
 * Entities in the Scene that have a ShadowRenderer System attached to them will
 * receive shadows from other Entities in the Scene.
 * @see Shadow
 * @see ShadowRenderer
 */
class ShadowMap : public RenderSystem<Scene> {
public:
  ShadowMap(int oversample = 1) : oversample_(oversample) {
    depth_buffer = Renderer::gen_framebuffer(
        oversample_ * Renderer::get_info().window_width,
        oversample_ * Renderer::get_info().window_height);
    material = gen_scoped<BasicMaterial>();
  }
  ~ShadowMap() {}
  void set_light(Referenced<Spotlight> light) { spotlight = light; }
  void render(float dt, Camera *camera, Scene *scene) override {

    // Render all meshes from all entities in the scene from the perspective of
    // the light into the depth buffer
    Renderer::set_framebuffer(depth_buffer.get());

    // Renderer properties
    Renderer::enable_depth_testing(true);
    Renderer::enable_face_culling(false);
    // Clear depth buffer
    Renderer::clear_depth_buffer();

    glViewport(0, 0, oversample_ * Renderer::get_info().window_width,
               oversample_ * Renderer::get_info().window_height);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(4.0f, 4.0f);
    // Get entities with a shadow component
    auto shadable_entities = scene->get_entities<Shadow>();
    // render all meshes from the perspective of the light with a basic material
    // to record depth into the depth buffer
    for (auto ent : shadable_entities) {
      for (auto pack_it = ent->packets_begin(); pack_it != ent->packets_end();
           pack_it++) {
        auto mesh = (*pack_it).first;
        mesh->render(std::dynamic_pointer_cast<Camera>(spotlight).get(),
                     material.get(), ent->get_model());
      }
      // set shadow properties
      ent->light_view = spotlight;
      ent->depth_buffer = depth_buffer;
    }
    // return to default framebuffer
    glDisable(GL_POLYGON_OFFSET_FILL);
    Renderer::set_framebuffer(nullptr);
    glViewport(0, 0, Renderer::get_info().window_width,
               Renderer::get_info().window_height);
    // Render entities normally with a shader that draws shadows...
  }
  Referenced<Framebuffer> depth_buffer;
  Referenced<Spotlight> spotlight;
  Scoped<BasicMaterial> material;

private:
  int oversample_;
};
} // namespace mare

#endif