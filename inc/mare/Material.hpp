#ifndef MATERIAL
#define MATERIAL

// MARE
#include "mare/Mare.hpp"
#include "mare/Shader.hpp"

namespace mare
{
class Material : public Asset
{
public:
    Material(const char *directory);
    virtual ~Material() {}
    virtual void render() {}
    inline void bind() const { shader_->use(); }
    inline unsigned int name() { return shader_->name(); }

    void upload_float(const char *name, float value) { shader_->upload_float(name, value); }
    void upload_vec3(const char *name, glm::vec3 value) { shader_->upload_vec3(name, value); }
    void upload_vec4(const char *name, glm::vec4 value) { shader_->upload_vec4(name, value); }
    void upload_mat3(const char *name, glm::mat3 value) { shader_->upload_mat3(name, value); }
    void upload_mat4(const char *name, glm::mat4 value) { shader_->upload_mat4(name, value); }
    void upload_uniform(IBuffer *uniform) { shader_->upload_uniform(uniform); }
    void upload_storage(IBuffer *storage) { shader_->upload_storage(storage); }
    void upload_texture2D(const char* name, Texture2DBuffer *texture2D) { shader_->upload_texture2D(name, texture2D); }
    void upload_camera(Camera *camera) { shader_->upload_camera(camera); }

protected:
    Scoped<Shader> shader_;
};
} // namespace mare

#endif