#include "mare/GL/GLMaterial.hpp"
#include <iostream>
#include <gtc/type_ptr.hpp>

namespace mare
{
GLMaterial::~GLMaterial()
{
    for (auto elem : uniform_block_binding_cache)
    {
        glDeleteBuffers(1, &elem.second.ubo);
    }
}

void GLMaterial::bind()
{
    m_shader.use();
}

void GLMaterial::upload_uniform_float(const char *name, float value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_shader.ID(), name));
    }
    glUniform1f(uniform_cache[name], value);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLMaterial::upload_uniform_vec3(const char *name, glm::vec3 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_shader.ID(), name));
    }
    glUniform3f(uniform_cache[name], value.x, value.y, value.z);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLMaterial::upload_uniform_vec4(const char *name, glm::vec4 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_shader.ID(), name));
    }
    glUniform4f(uniform_cache[name], value.x, value.y, value.z, value.w);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLMaterial::upload_uniform_mat4(const char *name, glm::mat4 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_shader.ID(), name));
    }
    glUniformMatrix4fv(uniform_cache[name], 1, GL_FALSE, glm::value_ptr(value));
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLMaterial::upload_uniform_mat4_array(const char *name, float *data, int count)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_shader.ID(), name));
    }
    glUniformMatrix4fv(uniform_cache[name], count, GL_FALSE, data);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLMaterial::bind_uniform_block(const char *name, size_t size)
{
    if (!uniform_block_binding_cache.count(name))
    {
        // create buffer and cache the location;
        GLuint ubo;
        glCreateBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferStorage(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
        uniform_block_data data = {glGetUniformBlockIndex(m_shader.ID(), name), size, ubo, GLuint(uniform_block_binding_cache.size())};
        uniform_block_binding_cache.insert_or_assign(name, data);
        glUniformBlockBinding(m_shader.ID(), uniform_block_binding_cache[name].binding_index, uniform_block_binding_cache[name].index);
    }
}

void GLMaterial::upload_uniform_block(const char *name, void *data)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform_block_binding_cache[name].index, uniform_block_binding_cache[name].ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, uniform_block_binding_cache[name].size, data);
}
} // namespace mare
