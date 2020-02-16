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
    Mesh *mesh;
    Material *material;
};
class Object : public Mesh
{
public:
    Object();
    virtual ~Object();
    virtual void render(Layer *layer, Material *material) {} // not used
    virtual void render(Layer *layer, Material *material, glm::mat4 parent_model) {} // not used
    virtual void render(Layer *layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) {} // not used
    void push_primative(Mesh *mesh, Material *material);
    void render(Layer *layer);
    std::vector<Primative> primatives;
};
} // namespace mare

#endif