#ifndef GLMATERIAL
#define GLMATERIAL

// Standard Library
#include <unordered_map>

// MARE
#include "mare/Material.hpp"
#include "mare/GL/GLShader.hpp"

#include "glm.hpp"

namespace mare
{
class GLMaterial : public virtual Material
{
public:
    virtual ~GLMaterial();
    virtual void render() = 0;
    void bind() final override;
    void upload_uniform_float(const char *name, float value);
    void upload_uniform_vec3(const char *name, glm::vec3 value);
    void upload_uniform_vec4(const char *name, glm::vec4 value);
    void upload_uniform_mat4(const char *name, glm::mat4 value);
    void upload_uniform_mat4_array(const char *name, float *data, int count);
    void bind_uniform_block(const char *name, size_t size);
    void upload_uniform_block(const char *name, void *data);

protected:
    GLShader m_shader;

private:
    std::unordered_map<std::string, int> uniform_cache;
    struct uniform_block_data
    {
        unsigned int binding_index;
        size_t size;
        unsigned int ubo;
        unsigned int index;
    };
    std::unordered_map<std::string, uniform_block_data> uniform_block_binding_cache;
};
} // namespace mare

#endif