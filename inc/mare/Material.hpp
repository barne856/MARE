#ifndef MATERIAL
#define MATERIAL

// MARE
#include "mare/MareUtility.hpp"
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

    void upload_float(const char *name, float value) { shader_->upload_float(name, value); }
    void upload_vec3(const char *name, glm::vec3 value) { shader_->upload_vec3(name, value); }
    void upload_vec4(const char *name, glm::vec4 value) { shader_->upload_vec4(name, value); }
    void upload_mat3(const char *name, glm::mat3 value) { shader_->upload_mat3(name, value); }
    void upload_mat4(const char *name, glm::mat4 value) { shader_->upload_mat4(name, value); }
    void upload_mat4_array(const char *name, float *data, int count) { shader_->upload_mat4_array(name, data, count); }
    void bind_buffer_block(const char *name, size_t size) { shader_->bind_buffer_block(name, size); }
    void upload_buffer_block(const char *name, void *data) { shader_->upload_buffer_block(name, data); }

protected:
    Scoped<Shader> shader_;
};
} // namespace mare

#endif