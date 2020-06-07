#ifndef PACKETRENDERER
#define PACKETRENDERER

// MARE
#include "mare/Components/RenderPack.hpp"
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

/**
 * @brief A RenderSystem that operates on RenderPack Components.
 * @details All Entities that have a PacketRenderer on their System stack will
 * render the Entity's Render Packets. Entitiy must inherit from the RenderPack
 * Component.
 * @see RenderPack
 */
class PacketRenderer : public RenderSystem<RenderPack> {
public:
  void render(float dt, Camera *camera, RenderPack *rp) override {
    for (auto pack_it = rp->packets_begin(); pack_it != rp->packets_end();
         pack_it++) {
      auto mesh = (*pack_it).first;
      auto material = (*pack_it).second;
      mesh->render(camera, material.get(), rp->get_transformation_matrix());
    }
  }
};
} // namespace mare
#endif