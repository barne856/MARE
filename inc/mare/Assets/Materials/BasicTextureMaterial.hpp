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
    BasicTextureMaterial() : Material("./inc/mare/Assets/Shaders/BasicTexture") {}
    virtual ~BasicTextureMaterial() {}
    void render() override
    {
        upload_texture2D("tex", texture_.get());
    }
    void set_texture(Referenced<Texture2D> texture)
    {
        texture_ = texture;
    }
private:
    Referenced<Texture2D> texture_;
};
} // namespace mare

#endif