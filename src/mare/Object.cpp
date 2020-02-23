// MARE
#include "mare/Object.hpp"

namespace mare
{
Object::Object(){}
Object::~Object() {}
void Object::push_primative(Referenced<Mesh> mesh, Referenced<Material> material)
{
    primatives.push_back({mesh, material});
}
void Object::render(const Layer* layer)
{
    for (auto &primative : primatives)
    {
        primative.material->bind();
        primative.material->render();
        primative.mesh->render(layer, primative.material.get(), transform_);
    }
}
} // namespace mare