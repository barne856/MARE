#ifndef RENDERSYSTEMFORWARDER
#define RENDERSYSTEMFORWARDER

#include "Entity.hpp"
#include "Systems.hpp"

namespace mare {
class RenderSystemForwarder : public IRenderSystem {
public:
  RenderSystemForwarder(Entity *forward_entity, Entity *parent = nullptr)
      : forward_entity_(forward_entity), parent_(parent) {}
  void render(float dt, Camera *camera, Entity *entity) override {
    auto render_systems = forward_entity_->get_systems<IRenderSystem>();
    for (auto &system : render_systems) {
      if (parent_) {
        forward_entity_->set_transformation_matrix(
            parent_->get_transformation_matrix());
      }
      system->render(dt, camera, forward_entity_);
    }
  }

private:
  Entity *forward_entity_;
  Entity *parent_;
};
} // namespace mare

#endif