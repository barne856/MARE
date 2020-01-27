#ifndef TEXTURE
#define TEXTURE

namespace mare
{
class Texture
{
public:
    Texture(const char *image_filepath) {}
    virtual ~Texture() {}
    virtual void bind(unsigned int binding_index) = 0;

protected:
    unsigned int m_texture_ID = 0;

};
} // namespace mare

#endif