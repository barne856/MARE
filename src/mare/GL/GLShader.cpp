#include "mare/GL/GLShader.hpp"
#include "mare/Entities/Camera.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <gtc/type_ptr.hpp>

namespace mare
{

GLShader::GLShader(const char *directory)
    : shaders_{}, uniform_block_binding_count_(0), storage_block_binding_count_(0), texture_binding_count_(0)
{
    shader_ID_ = 0;
    init_shader(directory);
}

GLShader::~GLShader()
{
    glDeleteProgram(shader_ID_); // Silently ignored if m_programID is 0
    for (auto s : shaders_)
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
    for (auto s : shaders_)
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
        for (auto s : shaders_)
        {
            glDeleteShader(s);
        }
        shaders_.clear();

        std::cerr << "SHADER PROGRAM FAILED TO LINK:\n";
        for (auto &c : infoLog)
            std::cerr << c;

        return 0;
    }

    // Always detach shaders after a successful link.
    for (auto s : shaders_)
    {
        glDetachShader(program, s);
        glDeleteShader(s);
    }
    shaders_.clear();
    return program;
}

void GLShader::init_shader(const char *directory)
{
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        std::string source{};
        read_shader(entry.path().string(), source);
        shaders_.push_back(compile_shader(source, shader_extension.at(entry.path().extension().string())));
    }
    shader_ID_ = create_program();
}

void GLShader::upload_float(const char *name, float value)
{
    if (!uniform_cache.count(name))
    {
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformLocation(shader_ID_, name));
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
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformLocation(shader_ID_, name));
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
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformLocation(shader_ID_, name));
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
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformLocation(shader_ID_, name));
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
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformLocation(shader_ID_, name));
    }
    glUniformMatrix4fv(uniform_cache[name], 1, GL_FALSE, glm::value_ptr(value));
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform '" << name << "' exists in the shader" << std::endl;
    }
}

void GLShader::upload_uniform(IBuffer *uniform)
{
    const char* name = uniform->format().attributes()[0].name.c_str();
    if (!uniform_cache.count(name))
    {
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformBlockIndex(shader_ID_, name));
        uniform_binding_cache.insert_or_assign(name, ++uniform_block_binding_count_);
        glUniformBlockBinding(shader_ID_, uniform_cache[name], uniform_binding_cache[name]);
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform_binding_cache[name], uniform->name());
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No uniform block '" << name << "' exists in the shader" << std::endl;
    }
}
void GLShader::upload_storage(IBuffer *storage)
{
    const char* name = storage->format().attributes()[0].name.c_str();
    if (!uniform_cache.count(name))
    {
        // cache the location
        uniform_cache.insert_or_assign(name, glGetProgramResourceIndex(shader_ID_, GL_SHADER_STORAGE_BLOCK, name));
        storage_binding_cache.insert_or_assign(name, ++storage_block_binding_count_);
        glShaderStorageBlockBinding(shader_ID_, uniform_cache[name], storage_binding_cache[name]);
    }
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage_binding_cache[name], storage->name());
    if (uniform_cache[name] == GL_INVALID_INDEX)
    {
        std::cerr << "SHADER WARNING: No storage buffer block '" << name << "' exists in the shader" << std::endl;
    } 
}
void GLShader::upload_texture2D(const char* name, Texture2DBuffer *texture2D)
{
    if (!uniform_cache.count(name))
    {
        // cache the location
        uniform_cache.insert_or_assign(name, glGetUniformLocation(shader_ID_, name));
        texture_binding_cache.insert_or_assign(name, ++texture_binding_count_);
        glUniform1i(uniform_cache[name], texture_binding_cache[name]);
    }
    glBindTextureUnit(texture_binding_cache[name], texture2D->name());
    if (uniform_cache[name] == -1)
    {
        std::cerr << "SHADER WARNING: No storage buffer block '" << name << "' exists in the shader" << std::endl;
    }
}
void GLShader::upload_camera(Camera *camera)
{
    upload_mat4("projection", camera->projection());
    upload_mat4("view", camera->view());
}

} // namespace mare
