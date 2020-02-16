// MARE
#include "mare/Object.hpp"

namespace mare
{
Object::Object(){}
Object::~Object()
{
    for (auto &primative : primatives)
    {
        if (primative.mesh)
        {
            delete primative.mesh;
        }
        if (primative.material)
        {
            delete primative.material;
        }
    }
};
void Object::push_primative(Mesh *mesh, Material *material)
{
    primatives.push_back({mesh, material});
}
void Object::render(Layer *layer)
{
    for (auto &primative : primatives)
    {
        primative.material->bind();
        primative.material->render();
        primative.mesh->render(layer, primative.material, transform);
    }
}
} // namespace mare