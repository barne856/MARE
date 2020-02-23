#ifndef OBJECT
#define OBJECT

// MARE
#include "mare/Mesh.hpp"
#include "mare/Material.hpp"
#include "mare/Layer.hpp"

// Standard Library
#include <vector>

namespace mare
{
struct Primative
{
    Referenced<Mesh> mesh;
    Referenced<Material> material;
};
class Object : public Mesh
{
public:
    Object();
    virtual ~Object();
    virtual void render(const Layer *layer, Material *material) override {}                                                                                              // not used
    virtual void render(const Layer *layer, Material *material, const glm::mat4 &parent_model) override {}                                                               // not used
    virtual void render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) override {} // not used
    void push_primative(Referenced<Mesh> mesh, Referenced<Material> material);
    void render(const Layer *layer);
    std::vector<Primative> primatives;
};
} // namespace mare

#endif