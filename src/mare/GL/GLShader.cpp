#include "mare/GL/GLShader.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <gtc/type_ptr.hpp>

namespace mare
{

GLShader::GLShader(const char *directory)
    : m_program_ID{0}, m_shaders{}
{
    init_shader(directory);
}

GLShader::~GLShader()
{
    for (auto elem : uniform_block_binding_cache)
    {
        glDeleteBuffers(1, &elem.second.ubo);
    }
    glDeleteProgram(m_program_ID); // Silently ignored if m_programID is 0
    for (auto s : m_shaders)
    {
        glDeleteShader(s);
    }
}

const std::string GLShader::type_to_name(GLenum type)
{
    switch (type)
    {
    case GL_VERTEX_SHADER:
        return "VERTEX";
    case GL_TESS_CONTROL_SHADER:
        return "TESSELATION CONTROL";
    case GL_TESS_EVALUATION_SHADER:
        return "TESSELATION EVALUATION";
    case GL_GEOMETRY_SHADER:
        return "GEOMETRY";
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT";
    case GL_COMPUTE_SHADER:
        return "COMPUTE";
    }
    return "INVALID_TYPE";
}

const std::unordered_map<std::string, GLenum> GLShader::shader_extension{
    {".vert", GL_VERTEX_SHADER},
    {".tesc", GL_TESS_CONTROL_SHADER},
    {".tese", GL_TESS_EVALUATION_SHADER},
    {".geom", GL_GEOMETRY_SHADER},
    {".frag", GL_FRAGMENT_SHADER},
    {".comp", GL_COMPUTE_SHADER}};

void GLShader::read_shader(std::string shader_path, std::string &source)
{
    std::ifstream s_file{shader_path};
    if (s_file.is_open())
    {
        std::string line;
        while (std::getline(s_file, line))
        {
            source += (line + '\n');
        }
        s_file.close();
    }
    else
    {
        std::cerr << "Unable to open shader source file: " << shader_path << std::endl;
    }
}

GLuint GLShader::compile_shader(std::string &shader_source, GLenum SHADER_TYPE)
{
    const GLchar *src{shader_source.c_str()};
    GLuint shader = glCreateShader(SHADER_TYPE);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> error_log(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &error_log[0]);

        std::cerr << type_to_name(SHADER_TYPE) << " SHADER FAILED TO COMPILE:" << std::endl;
        for (auto &c : error_log)
            std::cerr << c;

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint GLShader::create_program()
{
    GLuint program = glCreateProgram();
    for (auto s : m_shaders)
    {
        glAttachShader(program, s);
    }
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        for (auto s : m_shaders)
        {
            glDeleteShader(s);
        }
        m_shaders.clear();

        std::cerr << "SHADER PROGRAM FAILED TO LINK:\n";
        for (auto &c : infoLog)
            std::cerr << c;

        return 0;
    }

    // Always detach shaders after a successful link.
    for (auto s : m_shaders)
    {
        glDetachShader(program, s);
        glDeleteShader(s);
    }
    m_shaders.clear();
    return program;
}

void GLShader::init_shader(const char *directory)
{
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        std::string source{};
        read_shader(entry.path().string(), source);
        m_shaders.push_back(compile_shader(source, shader_extension.at(entry.path().extension().string())));
    }
    m_program_ID = create_program();
}

void GLShader::create(const char *directory)
{
    glDeleteProgram(m_program_ID);
    for (auto s : m_shaders)
    {
        glDeleteShader(s);
    }
    glUseProgram(0);
    m_program_ID = 0;
    init_shader(directory);
}

void GLShader::upload_float(const char *name, float value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_program_ID, name));
    }
    glUniform1f(uniform_cache[name], value);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::upload_vec3(const char *name, glm::vec3 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_program_ID, name));
    }
    glUniform3f(uniform_cache[name], value.x, value.y, value.z);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::upload_vec4(const char *name, glm::vec4 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_program_ID, name));
    }
    glUniform4f(uniform_cache[name], value.x, value.y, value.z, value.w);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::upload_mat3(const char *name, glm::mat3 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_program_ID, name));
    }
    glUniformMatrix3fv(uniform_cache[name], 1, GL_FALSE, glm::value_ptr(value));
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::upload_mat4(const char *name, glm::mat4 value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_program_ID, name));
    }
    glUniformMatrix4fv(uniform_cache[name], 1, GL_FALSE, glm::value_ptr(value));
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::upload_mat4_array(const char *name, float *data, int count)
{
    if (!uniform_cache.count(name))
    {
        // cache the location;
        uniform_cache.insert_or_assign(name, glGetUniformLocation(m_program_ID, name));
    }
    glUniformMatrix4fv(uniform_cache[name], count, GL_FALSE, data);
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::bind_buffer_block(const char *name, size_t size)
{
    if (!uniform_block_binding_cache.count(name))
    {
        // create buffer and cache the location;
        GLuint ubo;
        glCreateBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferStorage(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
        uniform_block_data data = {glGetUniformBlockIndex(m_program_ID, name), size, ubo, GLuint(uniform_block_binding_cache.size())};
        uniform_block_binding_cache.insert_or_assign(name, data);
        glUniformBlockBinding(m_program_ID, uniform_block_binding_cache[name].binding_index, uniform_block_binding_cache[name].index);
    }
}

void GLShader::upload_buffer_block(const char *name, void *data)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform_block_binding_cache[name].index, uniform_block_binding_cache[name].ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, uniform_block_binding_cache[name].size, data);
}

} // namespace mare
