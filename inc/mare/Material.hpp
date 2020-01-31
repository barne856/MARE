#ifndef MATERIAL
#define MATERIAL

#include "mare/Shader.hpp"

namespace mare
{
class Material
{
public:
    Material(const char *directory);
    virtual ~Material() {}
    virtual void render() {}
    inline void bind() const { m_shader->use(); }

    void upload_float(const char *name, float value) { m_shader->upload_float(name, value); }
    void upload_vec3(const char *name, glm::vec3 value) { m_shader->upload_vec3(name, value); }
    void upload_vec4(const char *name, glm::vec4 value) { m_shader->upload_vec4(name, value); }
    void upload_mat3(const char *name, glm::mat3 value) { m_shader->upload_mat3(name, value); }
    void upload_mat4(const char *name, glm::mat4 value) { m_shader->upload_mat4(name, value); }
    void upload_mat4_array(const char *name, float *data, int count) { m_shader->upload_mat4_array(name, data, count); }
    void bind_buffer_block(const char *name, size_t size) { m_shader->bind_buffer_block(name, size); }
    void upload_buffer_block(const char *name, void *data) { m_shader->upload_buffer_block(name, data); }

protected:
    Shader *m_shader;
};
} // namespace mare

#endif