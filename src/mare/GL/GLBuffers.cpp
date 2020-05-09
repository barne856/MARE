// MARE
#include "mare/GL/GLBuffers.hpp"

// External Libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Library
#include <iostream>

namespace mare
{

  GLenum gl_sized_tex_format(TextureType type)
  {
    switch (type)
    {
    case TextureType::R8:
      return GL_R8;
    case TextureType::RG8:
      return GL_RG8;
    case TextureType::RGB8:
      return GL_RGB8;
    case TextureType::RGBA8:
      return GL_RGBA8;
    case TextureType::R32F:
      return GL_R32F;
    case TextureType::RG32F:
      return GL_RG32F;
    case TextureType::RGB32F:
      return GL_RGB32F;
    case TextureType::RGBA32F:
      return GL_RGBA32F;
    case TextureType::DEPTH:
      return GL_DEPTH_COMPONENT32F;
    default:
      return NULL;
    }
  }

  GLenum gl_tex_format(TextureType type)
  {
    switch (type)
    {
    case TextureType::R8:
      return GL_RED;
    case TextureType::RG8:
      return GL_RG;
    case TextureType::RGB8:
      return GL_RGB;
    case TextureType::RGBA8:
      return GL_RGBA;
    case TextureType::R32F:
      return GL_RED;
    case TextureType::RG32F:
      return GL_RG;
    case TextureType::RGB32F:
      return GL_RGB;
    case TextureType::RGBA32F:
      return GL_RGBA;
    case TextureType::DEPTH:
      return GL_DEPTH_COMPONENT;
    default:
      return NULL;
    }
  }

  int gl_tex_bytes(TextureType type)
  {
    switch (type)
    {
    case TextureType::R8:
      return 1;
    case TextureType::RG8:
      return 1;
    case TextureType::RGB8:
      return 1;
    case TextureType::RGBA8:
      return 1;
    case TextureType::R32F:
      return 4;
    case TextureType::RG32F:
      return 4;
    case TextureType::RGB32F:
      return 4;
    case TextureType::RGBA32F:
      return 4;
    case TextureType::DEPTH:
      return 1;
    default:
      return NULL;
    }
  }

  int gl_tex_type(TextureType type)
  {
    switch (type)
    {
    case TextureType::R8:
      return GL_UNSIGNED_BYTE;
    case TextureType::RG8:
      return GL_UNSIGNED_BYTE;
    case TextureType::RGB8:
      return GL_UNSIGNED_BYTE;
    case TextureType::RGBA8:
      return GL_UNSIGNED_BYTE;
    case TextureType::R32F:
      return GL_FLOAT;
    case TextureType::RG32F:
      return GL_FLOAT;
    case TextureType::RGB32F:
      return GL_FLOAT;
    case TextureType::RGBA32F:
      return GL_FLOAT;
    case TextureType::DEPTH:
      return GL_FLOAT;
    default:
      return NULL;
    }
  }

  GLTexture2D::GLTexture2D(const char *filepath)
      : Texture2D(filepath)
  {
    unsigned char *texture_data_ = stbi_load(filepath, &width_, &height_, &channels_, 0);
    texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(texture_data_, width_ * height_ * channels_, BufferType::STATIC);
    stbi_image_free(texture_data_);
    texture_data_ = nullptr;

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
    switch (channels_)
    {
    case 1:
      type_ = TextureType::R8;
      break;
    case 2:
      type_ = TextureType::RG8;
      break;
    case 3:
      type_ = TextureType::RGB8;
      break;
    case 4:
      type_ = TextureType::RGBA8;
      break;
    }
    glTextureStorage2D(texture_ID_, 1, gl_sized_tex_format(type_), width_, height_);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, gl_tex_format(type_), gl_tex_type(type_), nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }
  GLTexture2D::GLTexture2D(TextureType type, int width, int height)
      : Texture2D(type, width, height)
  {
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_ID_);
    glTextureStorage2D(texture_ID_, 1, gl_sized_tex_format(type_), width_, height_);
    switch (type_)
    {
    case TextureType::R8:
      channels_ = 1;
      texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::RG8:
      channels_ = 2;
      texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::RGB8:
      channels_ = 3;
      texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::RGBA8:
      channels_ = 4;
      texture_buffer_ = std::make_unique<GLBuffer<uint8_t>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::R32F:
      channels_ = 1;
      texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::RG32F:
      channels_ = 2;
      texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::RGB32F:
      channels_ = 3;
      texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::RGBA32F:
      channels_ = 4;
      texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * channels_ * gl_tex_bytes(type_), BufferType::READ_WRITE);
      break;
    case TextureType::DEPTH:
      channels_ = 1;
      texture_buffer_ = std::make_unique<GLBuffer<float>>(nullptr, width_ * height_ * channels_ * 4, BufferType::READ_WRITE);

      glBindTexture(GL_TEXTURE_2D, texture_ID_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glBindTexture(GL_TEXTURE_2D, 0);
      break;
    default:
      break;
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, texture_buffer_->name());
    glTextureSubImage2D(texture_ID_, 0, 0, 0, width_, height_, gl_tex_format(type_), gl_tex_type(type_), nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }
  GLTexture2D::~GLTexture2D()
  {
    glDeleteTextures(1, &texture_ID_);
  }

  GLFramebuffer::GLFramebuffer(int width, int height)
      : Framebuffer(width, height)
  {
    glCreateFramebuffers(1, &framebuffer_ID_);
    depth_texture_ = std::make_unique<GLTexture2D>(TextureType::DEPTH, width, height);
    color_texture_ = std::make_unique<GLTexture2D>(TextureType::RGBA32F, width, height);
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