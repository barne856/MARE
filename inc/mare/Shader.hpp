#ifndef SHADER
#define SHADER

#include "glm.hpp"

namespace mare
{
class Shader
{
public:
    Shader() {}
    virtual ~Shader() {}
    virtual void create(const char *directory) = 0;
    virtual void use() const = 0;
    virtual void upload_float(const char *name, float value) = 0;
    virtual void upload_vec3(const char *name, glm::vec3 value) = 0;
    virtual void upload_vec4(const char *name, glm::vec4 value) = 0;
    virtual void upload_mat4(const char *name, glm::mat4 value) = 0;
    virtual void upload_mat4_array(const char *name, float *data, int count) = 0;
    virtual void bind_buffer_block(const char *name, size_t size) = 0;
    virtual void upload_buffer_block(const char *name, void *data) = 0;
};
} // namespace mare

#endif