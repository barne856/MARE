#ifndef TEXTURE
#define TEXTURE

namespace mare
{
class Texture2D
{
public:
    Texture2D(const char *image_filepath) {}
    virtual ~Texture2D() {}
    virtual void bind(unsigned int binding_index) = 0;

protected:
    unsigned int m_texture_ID = 0;

};
} // namespace mare

#endif