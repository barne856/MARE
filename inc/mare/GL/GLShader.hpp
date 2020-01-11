#ifndef SHADER
#define SHADER
#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "glm.hpp"

namespace mare
{
class GLShader
{
public:
    GLShader();
    GLShader(const char *directory);
    ~GLShader();
    void create(const char *directory);
    inline void use() const { glUseProgram(m_program_ID); }
    const static std::string type_to_name(GLenum type);
    const static std::unordered_map<std::string, GLenum> shader_extension;
    inline const GLuint ID() const { return m_program_ID; }

private:
    GLuint m_program_ID;
    std::vector<GLuint> m_shaders;

    void read_shader(std::string shader_path, std::string &source);
    GLuint compile_shader(std::string &shader_source, GLenum SHADER_TYPE);
    GLuint create_program();
    void init_shader(const char *directory);
};
} // namespace mare

#endif