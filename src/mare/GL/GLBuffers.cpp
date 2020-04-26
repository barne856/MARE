// MARE
#include "mare/GL/GLBuffers.hpp"

// External Libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Library
#include <iostream>

namespace mare
{
GLTexture2DBuffer::GLTexture2DBuffer(const char *filepath)
    : Texture2DBuffer(filepath)
{
  unsigned char *texture_data_ = stbi_load(filepath, &width_, &height_, &channels_, 0);
  texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(texture_data_, width_ * height_ * channels_, BufferType::STATIC);
  stbi_image_free(texture_data_);
  texture_data_ = nullptr;

  glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
  switch (channels_)
  {
  case 1:
    glTextureStorage2D(texture_ID_, 1, GL_R8, width_, height_);
    break;
  case 2:
    glTextureStorage2D(texture_ID_, 1, GL_RG8, width_, height_);
    break;
  case 3:
    glTextureStorage2D(texture_ID_, 1, GL_RGB8, width_, height_);
    break;
  case 4:
    glTextureStorage2D(texture_ID_, 1, GL_RGBA8, width_, height_);
    break;
  }
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    switch (channels_)
  {
  case 1:
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_R, GL_UNSIGNED_BYTE, nullptr);
    break;
  case 2:
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_RG, GL_UNSIGNED_BYTE, nullptr);
    break;
  case 3:
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    break;
  case 4:
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    break;
  }
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
GLTexture2DBuffer::GLTexture2DBuffer(TextureType type, int width, int height)
    : Texture2DBuffer(type, width, height)
{
  switch (type_)
  {
  case TextureType::RGBA8:
  {
    channels_ = 4;
    texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(nullptr, width_ * height_, BufferType::STATIC);
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
    glTextureStorage2D(texture_ID_, 1, GL_RGBA8, width_, height_);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    break;
  }
  case TextureType::RGBA32F:
  {
    channels_ = 4;
    texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * 4, BufferType::STATIC);
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
    glTextureStorage2D(texture_ID_, 1, GL_RGBA32F, width_, height_);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_RGBA, GL_FLOAT, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    break;
  }
  case TextureType::DEPTH:
  {
    channels_ = 1;
    texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * 4, BufferType::STATIC);
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
    glTextureStorage2D(texture_ID_, 1, GL_DEPTH_COMPONENT32F, width_, height_);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    break;
  }
  default:
  {
    channels_ = 4;
    texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * 4 * 4, BufferType::STATIC);
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
    glTextureStorage2D(texture_ID_, 1, GL_RGBA32F, width_, height_);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, GL_RGBA, GL_FLOAT, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    break;
  }
  }
}
GLTexture2DBuffer::~GLTexture2DBuffer()
{
  glDeleteTextures(1, &texture_ID_);
}

GLFramebuffer::GLFramebuffer(int width, int height)
    : Framebuffer(width, height)
{
  glCreateFramebuffers(1, &framebuffer_ID_);
  depth_texture_ = std::make_unique<GLTexture2DBuffer>(TextureType::DEPTH, width, height);
  color_texture_ = std::make_unique<GLTexture2DBuffer>(TextureType::RGBA32F, width, height);
  glNamedFramebufferTexture(framebuffer_ID_, GL_DEPTH_ATTACHMENT, depth_texture_->name(), 0);
  glNamedFramebufferTexture(framebuffer_ID_, GL_COLOR_ATTACHMENT0, color_texture_->name(), 0);
  GLenum draw_buffer[1] = {GL_COLOR_ATTACHMENT0};
  glNamedFramebufferDrawBuffers(framebuffer_ID_, 1, draw_buffer);
  if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
  {
    std::cerr << "Framebuffer is incomplete" << std::endl;
    if (GL_FRAMEBUFFER_UNDEFINED == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Framebuffer is undefined" << std::endl;
    }
    if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Framebuffer has incomplete attchment" << std::endl;
    }
    if (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Framebuffer has missing attachment" << std::endl;
    }
    if (GL_FRAMEBUFFER_UNSUPPORTED == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Framebuffer is unsupported" << std::endl;
    }
    if (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Framebuffer is incomplete (multisample)" << std::endl;
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
GLFramebuffer::~GLFramebuffer()
{
  glDeleteFramebuffers(1, &framebuffer_ID_);
}
} // namespace mare