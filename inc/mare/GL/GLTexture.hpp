#ifndef GLTEXTURE
#define GLTEXTURE

// OpenGL
#include <GL/glew.h>

// MARE
#include "mare/Texture.hpp"

namespace mare
{
class GLTexture2D : public Texture2D
{
public:
    GLTexture2D(const char *image_filepath);
    ~GLTexture2D();
    void bind(unsigned int binding_index) override;

private:
    unsigned char *m_image_data;
    int width, height, channels;
};
} // namespace mare

#endif