#include "GL/GLShader.hpp"
#include "Entities/Camera.hpp"
#include "GL/GLBuffers.hpp"
#include <filesystem>
#include <fstream>
#include <gtc/type_ptr.hpp>
#include <iostream>

namespace mare {

GLShader::GLShader(const char *directory) : shaders_{} {
  shader_ID_ = 0;
  init_shader(directory);
}

GLShader::~GLShader() {
  glDeleteProgram(shader_ID_); // Silently ignored if m_programID is 0
  for (auto s : shaders_) {
    glDeleteShader(s);
  }
}

const std::string GLShader::type_to_name(GLenum type) {
  switch (type) {
  case GL_VERTEX_SHADER:
    return "VERTEX";
  case GL_TESS_CONTROL_SHADER:
    return "TESSELATION CONTROL";
  case GL_TESS_EVALUATION_SHADER:
    return "TESSELATION EVALUATION";
  case GL_GEOMETRY_SHADER:
    return "GEOMETRY";
  case GL_FRAGMENT_SHADER:
    return "FRAGMENT";
  case GL_COMPUTE_SHADER:
    return "COMPUTE";
  }
  return "INVALID_TYPE";
}

const std::unordered_map<std::string, GLenum> GLShader::shader_extension{
    {".vert", GL_VERTEX_SHADER},          {".tesc", GL_TESS_CONTROL_SHADER},
    {".tese", GL_TESS_EVALUATION_SHADER}, {".geom", GL_GEOMETRY_SHADER},
    {".frag", GL_FRAGMENT_SHADER},        {".comp", GL_COMPUTE_SHADER}};

void GLShader::read_shader(std::string shader_path, std::string &source) {
  std::ifstream s_file{shader_path};
  if (s_file.is_open()) {
    std::string line;
    while (std::getline(s_file, line)) {
      source += (line + '\n');
    }
    s_file.close();
  } else {
    std::cerr << "Unable to open shader source file: " << shader_path
              << std::endl;
  }
}

GLuint GLShader::compile_shader(std::string &shader_source,
                                GLenum SHADER_TYPE) {
  const GLchar *src{shader_source.c_str()};
  GLuint shader = glCreateShader(SHADER_TYPE);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> error_log(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &error_log[0]);

    std::cerr << type_to_name(SHADER_TYPE)
              << " SHADER FAILED TO COMPILE:" << std::endl;
    for (auto &c : error_log)
      std::cerr << c;

    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

GLuint GLShader::create_program() {
  GLuint program = glCreateProgram();
  for (auto s : shaders_) {
    glAttachShader(program, s);
  }
  glLinkProgram(program);

  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(program);
    for (auto s : shaders_) {
      glDeleteShader(s);
    }
    shaders_.clear();

    std::cerr << "SHADER PROGRAM FAILED TO LINK:\n";
    for (auto &c : infoLog)
      std::cerr << c;

    return 0;
  }

  // Always detach shaders after a successful link.
  for (auto s : shaders_) {
    glDetachShader(program, s);
    glDeleteShader(s);
  }
  shaders_.clear();
  return program;
}

void GLShader::init_shader(const char *directory) {
  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    std::string source{};
    read_shader(entry.path().string(), source);
    shaders_.push_back(compile_shader(
        source, shader_extension.at(entry.path().extension().string())));
  }
  shader_ID_ = create_program();
}

void GLShader::upload_float(const char *name, float value,
                            bool suppress_warnings) {
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
  }
  glUniform1f(resource_cache_[name], value);
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform '" << name
              << "' exists in the shader" << std::endl;
  }
}

void GLShader::upload_vec3(const char *name, glm::vec3 value,
                           bool suppress_warnings) {
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
  }
  glUniform3f(resource_cache_[name], value.x, value.y, value.z);
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform '" << name
              << "' exists in the shader" << std::endl;
  }
}

void GLShader::upload_vec4(const char *name, glm::vec4 value,
                           bool suppress_warnings) {
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
  }
  glUniform4f(resource_cache_[name], value.x, value.y, value.z, value.w);
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform '" << name
              << "' exists in the shader" << std::endl;
  }
}

void GLShader::upload_mat3(const char *name, glm::mat3 value,
                           bool suppress_warnings) {
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
  }
  glUniformMatrix3fv(resource_cache_[name], 1, GL_FALSE, glm::value_ptr(value));
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform '" << name
              << "' exists in the shader" << std::endl;
  }
}

void GLShader::upload_mat4(const char *name, glm::mat4 value,
                           bool suppress_warnings) {
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
  }
  glUniformMatrix4fv(resource_cache_[name], 1, GL_FALSE, glm::value_ptr(value));
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform '" << name
              << "' exists in the shader" << std::endl;
  }
}

void GLShader::upload_uniform(const char *name, IBuffer *uniform,
                              bool suppress_warnings) {
  if (!uniform) {
    // no data to upload
    return;
  }
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformBlockIndex(shader_ID_, name));
    uniform_binding_cache_.insert_or_assign(
        name, static_cast<GLuint>(uniform_binding_cache_.size()));
    glUniformBlockBinding(shader_ID_, resource_cache_[name],
                          uniform_binding_cache_[name]);
  }
  glBindBufferBase(GL_UNIFORM_BUFFER, uniform_binding_cache_[name],
                   uniform->name());
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform block '" << name
              << "' exists in the shader" << std::endl;
  }
}
void GLShader::upload_storage(const char *name, IBuffer *storage,
                              bool suppress_warnings) {
  if (!storage) {
    // no data to upload
    return;
  }
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(
        name,
        glGetProgramResourceIndex(shader_ID_, GL_SHADER_STORAGE_BLOCK, name));
    storage_binding_cache_.insert_or_assign(
        name, static_cast<GLuint>(storage_binding_cache_.size()));
    glShaderStorageBlockBinding(shader_ID_, resource_cache_[name],
                                storage_binding_cache_[name]);
  }
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage_binding_cache_[name],
                   storage->name());
  if (resource_cache_[name] == GL_INVALID_INDEX && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No storage buffer block '" << name
              << "' exists in the shader" << std::endl;
  }
}
void GLShader::upload_texture2D(const char *name, Texture2D *texture2D,
                                bool suppress_warnings) {
  if (!texture2D) {
    // no data to upload
    return;
  }
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
    texture_binding_cache_.insert_or_assign(
        name, static_cast<GLuint>(texture_binding_cache_.size()));
    glUniform1i(resource_cache_[name], texture_binding_cache_[name]);
  }
  glBindTextureUnit(texture_binding_cache_[name], texture2D->name());
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform sampler2D '" << name
              << "' exists in the shader" << std::endl;
  }
}

void GLShader::upload_image2D(const char *name, Texture2D *texture2D,
                              bool suppress_warnings) {
  if (!texture2D) {
    // no data to upload
    return;
  }
  if (!resource_cache_.count(name)) {
    // cache the location
    resource_cache_.insert_or_assign(name,
                                     glGetUniformLocation(shader_ID_, name));
    image_binding_cache_.insert_or_assign(
        name, static_cast<GLuint>(image_binding_cache_.size()));
    glUniform1i(resource_cache_[name], image_binding_cache_[name]);
  }
  glBindImageTexture(image_binding_cache_[name], texture2D->name(), 0, GL_FALSE,
                     0, GL_READ_WRITE,
                     opengl::gl_sized_tex_format(texture2D->type()));
  if (resource_cache_[name] == -1 && suppress_warnings == false) {
    std::cerr << "SHADER WARNING: No uniform image2D '" << name
              << "' exists in the shader" << std::endl;
  }
}

// Memory Barriers
void GLShader::barrier(BarrierType type) {
  switch (type) {
  case BarrierType::ATTRIBUTE:
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    break;
  case BarrierType::INDEX:
    glMemoryBarrier(GL_ELEMENT_ARRAY_BARRIER_BIT);
    break;
  case BarrierType::UNIFORM:
    glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
    break;
  case BarrierType::TEXTURE_FETCH:
    glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
    break;
  case BarrierType::IMAGE:
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    break;
  case BarrierType::COMMAND:
    glMemoryBarrier(GL_COMMAND_BARRIER_BIT);
    break;
  case BarrierType::BUFFER_UPDATE:
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    break;
  case BarrierType::FRAMEBUFFER:
    glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
    break;
  case BarrierType::ATOMIC_COUNTER:
    glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
    break;
  case BarrierType::STORAGE:
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    break;
  case BarrierType::ALL:
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    break;
  }
}

} // namespace mare
