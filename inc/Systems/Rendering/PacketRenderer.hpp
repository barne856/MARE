#ifndef PACKETRENDERER
#define PACKETRENDERER

// MARE
#include "Components/RenderPack.hpp"
#include "Mare.hpp"
#include "Renderer.hpp"
#include "Systems.hpp"

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
      mesh->render(camera, material.get(), rp);
    }
  }
};
} // namespace mare
#endif