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

        void upload_float(const char *name, float value, bool suppress_warnings = false) override;
        void upload_vec3(const char *name, glm::vec3 value, bool suppress_warnings = false) override;
        void upload_vec4(const char *name, glm::vec4 value, bool suppress_warnings = false) override;
        void upload_mat3(const char *name, glm::mat3 value, bool suppress_warnings = false) override;
        void upload_mat4(const char *name, glm::mat4 value, bool suppress_warnings = false) override;
        virtual void upload_uniform(const char *name, IBuffer *uniform, bool suppress_warnings = false) override;
        virtual void upload_storage(const char *name, IBuffer *storage, bool suppress_warnings = false) override;
        virtual void upload_texture2D(const char *name, Texture2DBuffer *texture2D, bool suppress_warnings = false) override;
        virtual void upload_image2D(const char *name, Texture2DBuffer *texture2D, bool suppress_warnings = false) override;

    private:
        std::vector<GLuint> shaders_;

        void read_shader(std::string shader_path, std::string &source);
        GLuint compile_shader(std::string &shader_source, GLenum SHADER_TYPE);
        GLuint create_program();
        void init_shader(const char *directory);

        std::unordered_map<std::string, int> resource_cache_;
        std::unordered_map<std::string, GLuint> uniform_binding_cache_;
        std::unordered_map<std::string, GLuint> storage_binding_cache_;
        std::unordered_map<std::string, GLuint> texture_binding_cache_;
        std::unordered_map<std::string, GLuint> image_binding_cache_;
    };
} // namespace mare

#endif