#ifndef MESH
#define MESH

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Material.hpp"
#include "mare/Buffer.hpp"
#include "mare/Layer.hpp"
#include "mare/Transform.hpp"

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare
{
class Mesh : public Transform
{
public:
    Mesh() {}
    virtual ~Mesh() {}

    virtual void render(const Layer *layer, Material *material) = 0;
    virtual void render(const Layer *layer, Material *material, const glm::mat4 &parent_model) = 0;
    virtual void render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) = 0;
};

} // namespace mare

#endif