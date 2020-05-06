#ifndef SHADER
#define SHADER

// MARE
#include "mare/Mare.hpp"
#include "mare/Buffers.hpp"

// External Libraries
#include "glm.hpp"

// Standard Library
#include <unordered_map>

namespace mare
{
    // forward declarations
    class Camera;
    class Mesh;
    class Shader
    {
    public:
        Shader() {}
        virtual ~Shader() {}
        inline uint32_t name() { return shader_ID_; }
        virtual void use() const = 0;

        virtual void upload_float(const char *name, float value, bool suppress_warnings = false) = 0;
        virtual void upload_vec3(const char *name, glm::vec3 value, bool suppress_warnings = false) = 0;
        virtual void upload_vec4(const char *name, glm::vec4 value, bool suppress_warnings = false) = 0;
        virtual void upload_mat3(const char *name, glm::mat3 value, bool suppress_warnings = false) = 0;
        virtual void upload_mat4(const char *name, glm::mat4 value, bool suppress_warnings = false) = 0;
        virtual void upload_uniform(const char* name, IBuffer *uniform, bool suppress_warnings = false) = 0;
        virtual void upload_storage(const char* name, IBuffer *storage, bool suppress_warnings = false) = 0;
        virtual void upload_texture2D(const char *name, Texture2DBuffer *texture2D, bool suppress_warnings = false) = 0;
        virtual void upload_image2D(const char *name, Texture2DBuffer *texture2D, bool suppress_warnings = false) = 0;

    protected:
        uint32_t shader_ID_;
    };

    class ShaderProgram : public Asset
    {
    public:
        ShaderProgram(const char *directory);
        virtual ~ShaderProgram(){};
        inline void bind() const { shader_->use(); }
        inline unsigned int name() { return shader_->name(); }

        void upload_float(const char *name, float value, bool suppress_warnings = false) { shader_->upload_float(name, value, suppress_warnings); }
        void upload_vec3(const char *name, glm::vec3 value, bool suppress_warnings = false) { shader_->upload_vec3(name, value, suppress_warnings); }
        void upload_vec4(const char *name, glm::vec4 value, bool suppress_warnings = false) { shader_->upload_vec4(name, value, suppress_warnings); }
        void upload_mat3(const char *name, glm::mat3 value, bool suppress_warnings = false) { shader_->upload_mat3(name, value, suppress_warnings); }
        void upload_mat4(const char *name, glm::mat4 value, bool suppress_warnings = false) { shader_->upload_mat4(name, value, suppress_warnings); }
        void upload_uniform(const char* name, IBuffer *uniform, bool suppress_warnings = false) { shader_->upload_uniform(name, uniform, suppress_warnings); }
        void upload_storage(const char* name, IBuffer *storage, bool suppress_warnings = false) { shader_->upload_storage(name, storage, suppress_warnings); }
        void upload_texture2D(const char *name, Texture2DBuffer *texture2D, bool suppress_warnings = false) { shader_->upload_texture2D(name, texture2D, suppress_warnings); }
        void upload_image2D(const char *name, Texture2DBuffer *texture2D, bool suppress_warnings = false) { shader_->upload_image2D(name, texture2D, suppress_warnings); }

    protected:
        Referenced<Shader> shader_;

    private:
        // caches
        static std::unordered_map<std::string, Referenced<Shader>> shader_cache_;
    };

    class GraphicsProgram : public ShaderProgram
    {
    public:
        GraphicsProgram(const char *directory) : ShaderProgram(directory) {}
        virtual ~GraphicsProgram() {}
        virtual void render() {}
    };

    class ComputeProgram : public ShaderProgram
    {
    public:
        ComputeProgram(const char *directory) : ShaderProgram(directory) {}
        virtual ~ComputeProgram() {}
        void dispatch_compute(uint32_t x = 1, uint32_t y = 1, uint32_t z = 1);
        virtual void compute() {}
    };

    class Material : public GraphicsProgram
    {
    public:
        Material(const char *directory) : GraphicsProgram(directory) {}
        virtual ~Material() {}
        void upload_camera(Camera *camera, bool suppress_warnings = false);
        void upload_mesh_model_matrix(Mesh *mesh, bool suppress_warnings = false);
        void upload_mesh_model_matrix(Mesh *mesh, glm::mat4 parent_model, bool suppress_warnings = false);
        void upload_mesh_normal_matrix(Mesh *mesh, bool suppress_warnings = false);
        void upload_mesh_normal_matrix(Mesh *mesh, glm::mat4 parent_model, bool suppress_warnings = false);
        void upload_mesh_instance_matrices(Buffer<glm::mat4> *models, bool suppress_warnings = false);
        void upload_mesh(Mesh* mesh, bool suppress_warnings = false);
        void upload_mesh(Mesh* mesh, glm::mat4 parent_model, bool suppress_warnings = false);
        void upload_mesh(Mesh* mesh, glm::mat4 parent_model, Buffer<glm::mat4> *models, bool suppress_warnings = false);
    };
} // namespace mare

#endif