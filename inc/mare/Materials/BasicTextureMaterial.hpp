#ifndef BASICTEXTUREMATERIAL
#define BASICTEXTUREMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Material.hpp"
#include "mare/Mare.hpp"

namespace mare
{
class BasicTextureMaterial : public virtual Material
{
public:
    BasicTextureMaterial() : Material("./res/Shaders/BasicTexture") {}
    virtual ~BasicTextureMaterial() {}
    void render() override
    {
        upload_texture2D("tex", texture);
    }
    Texture2DBuffer* texture;
};
} // namespace mare

#endif