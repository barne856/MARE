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
class Object
{
public:
    Object(){};
    virtual ~Object()
    {
        for (auto &primative : primatives)
        {
            if(primative.mesh)
            {
                delete primative.mesh;
            }
            if(primative.material)
            {
                delete primative.material;
            }
        }
    };
    void push_primative(Mesh *mesh, Material *material)
    {
        primatives.push_back({mesh, material});
    }
    void render(Layer *layer)
    {
        for (auto &primative : primatives)
        {
            primative.material->bind();
            primative.material->render();
            primative.mesh->render(layer, primative.material);
        }
    }
    std::vector<Primative> primatives;
};
} // namespace mare

#endif