#ifndef GLSHADER
#define GLSHADER

// OpenGL
#include <GL/glew.h>

// Standard Library
#include <string>
#include <vector>
#include <unordered_map>

// MARE
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
    void create(const char *directory);
    inline void use() const { glUseProgram(m_program_ID); }
    const static std::string type_to_name(GLenum type);
    const static std::unordered_map<std::string, GLenum> shader_extension;
    inline const GLuint ID() const { return m_program_ID; }
    void upload_float(const char *name, float value) override;
    void upload_vec3(const char *name, glm::vec3 value) override;
    void upload_vec4(const char *name, glm::vec4 value) override;
    void upload_mat3(const char *name, glm::mat3 value) override;
    void upload_mat4(const char *name, glm::mat4 value) override;
    void upload_mat4_array(const char *name, float *data, int count) override;
    void bind_buffer_block(const char *name, size_t size) override;
    void upload_buffer_block(const char *name, void *data) override;

private:
    GLuint m_program_ID;
    std::vector<GLuint> m_shaders;

    void read_shader(std::string shader_path, std::string &source);
    GLuint compile_shader(std::string &shader_source, GLenum SHADER_TYPE);
    GLuint create_program();
    void init_shader(const char *directory);

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