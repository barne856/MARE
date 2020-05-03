#ifndef BASICTEXTUREMATERIAL
#define BASICTEXTUREMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Shader.hpp"
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
        upload_texture2D("tex", texture_.get());
    }
    void set_texture(Referenced<Texture2DBuffer> texture)
    {
        texture_ = texture;
    }
private:
    Referenced<Texture2DBuffer> texture_;
};
} // namespace mare

#endif