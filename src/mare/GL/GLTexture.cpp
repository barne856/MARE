#include "mare/GL/GLTexture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace mare
{
GLTexture::GLTexture(const char *image_filepath)
    : Texture(image_filepath), m_image_data(nullptr)
{
    m_image_data = stbi_load(image_filepath, &width, &height, &channels, 4);
    if (m_image_data == NULL)
    {
        std::cerr << "Invalid image, cannot create texture." << std::endl;
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_ID);
    glTextureStorage2D(m_texture_ID, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(m_texture_ID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, m_image_data);
    stbi_image_free(m_image_data);
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &m_texture_ID);
}

void GLTexture::bind(unsigned int binding_index)
{
    glBindTextureUnit(binding_index, m_texture_ID);
}
} // namespace mare