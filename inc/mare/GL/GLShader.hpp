#ifndef GLSHADER
#define GLSHADER

// OpenGL
#include <GL/glew.h>

// Standard Library
#include <string>
#include <vector>
#include <unordered_map>

// MARE
#include "mare/Buffers.hpp"
#include "mare/Shader.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{
class GLShader : public Shader
{
public:
    GLShader(const char *directory);
    ~GLShader();
    inline void use() const { glUseProgram(shader_ID_); }
    const static std::string type_to_name(GLenum type);
    const static std::unordered_map<std::string, GLenum> shader_extension;
    void upload_float(const char *name, float value) override;
    void upload_vec3(const char *name, glm::vec3 value) override;
    void upload_vec4(const char *name, glm::vec4 value) override;
    void upload_mat3(const char *name, glm::mat3 value) override;
    void upload_mat4(const char *name, glm::mat4 value) override;
    virtual void upload_uniform(IBuffer* uniform) override;
    virtual void upload_storage(IBuffer* storage) override;
    virtual void upload_texture2D(const char* name, Texture2DBuffer *texture2D) override;
    virtual void upload_camera(Camera *camera) override;

private:
    std::vector<GLuint> shaders_;

    void read_shader(std::string shader_path, std::string &source);
    GLuint compile_shader(std::string &shader_source, GLenum SHADER_TYPE);
    GLuint create_program();
    void init_shader(const char *directory);

    std::unordered_map<std::string, int> uniform_cache{};
    std::unordered_map<std::string, int> uniform_binding_cache{};
    std::unordered_map<std::string, int> storage_binding_cache{};
    std::unordered_map<std::string, int> texture_binding_cache{};
    struct uniform_block_data
    {
        unsigned int binding_index;
        size_t size;
        unsigned int ubo;
        unsigned int index;
    };
    unsigned int uniform_block_binding_count_;
    unsigned int storage_block_binding_count_;
    unsigned int texture_binding_count_;
};
} // namespace mare

#endif