#ifndef SHADER
#define SHADER

// MARE
#include "mare/Buffers.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{
class Camera; // forward declare camera
class Shader
{
public:
    Shader() {}
    virtual ~Shader() {}
    inline uint32_t name() { return shader_ID_; }
    virtual void use() const = 0;
    virtual void upload_float(const char *name, float value) = 0;
    virtual void upload_vec3(const char *name, glm::vec3 value) = 0;
    virtual void upload_vec4(const char *name, glm::vec4 value) = 0;
    virtual void upload_mat3(const char *name, glm::mat3 value) = 0;
    virtual void upload_mat4(const char *name, glm::mat4 value) = 0;
    virtual void upload_uniform(IBuffer *uniform) = 0;
    virtual void upload_storage(IBuffer *storage) = 0;
    virtual void upload_texture2D(const char* name, Texture2DBuffer *texture2D) = 0;
    virtual void upload_camera(Camera *camera) = 0;

protected:
    uint32_t shader_ID_;
};
} // namespace mare

#endif