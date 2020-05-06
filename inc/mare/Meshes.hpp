#ifndef MESHES
#define MESHES

// MARE
#include "mare/Mare.hpp"
#include "mare/Shader.hpp"
#include "mare/Buffers.hpp"
#include "mare/Entities/Camera.hpp"
#include "mare/Transform.hpp"

// Standard Library
#include <vector>
#include <unordered_map>

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare
{
enum class DrawMethod
{
    NONE,
    POINTS,
    LINES,
    LINE_STRIP,
    LINE_LOOP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

class Mesh : public Transform
{
public:
    Mesh() {}
    virtual ~Mesh() {}

    virtual void render(Camera *camera, Material *material) = 0;
    virtual void render(Camera *camera, Material *material, glm::mat4 &parent_model) = 0;
    virtual void render(Camera *camera, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
};

class SimpleMesh : public Mesh
{
public:
    SimpleMesh();
    ~SimpleMesh();

    void render(Camera *camera, Material *material) override;
    void render(Camera *camera, Material *material, glm::mat4 &parent_model) override;
    void render(Camera *camera, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;

    virtual void bind(Material *material);
    virtual void add_geometry_buffer(Scoped<Buffer<float>> geometry_buffer);
    virtual void set_index_buffer(Scoped<Buffer<uint32_t>> index_buffer);

    size_t render_count() const;
    void invalidate_render_state_cache();
    bool is_indexed() const;
    void set_draw_method(DrawMethod method);
    DrawMethod get_draw_method();
    // swap the rendered buffer to the next buffer if vertex data is multibuffered
    void swap_buffers();
    unsigned int get_render_index();
    void wait_buffers();
    void lock_buffers();
    std::unordered_map<uint32_t, uint32_t> render_states; // [key = shader_ID, value = render_state_ID]
    std::vector<Scoped<Buffer<float>>> geometry_buffers;
    Scoped<Buffer<uint32_t>> index_buffer;
    size_t geometry_buffer_count;
    size_t vertex_render_count;
    size_t index_render_count;

protected:
    DrawMethod draw_method_;
};

class CompositeMesh : public Mesh
{
public:
    CompositeMesh() {}
    virtual ~CompositeMesh() {}
    void render(Camera *camera, Material *material) override;
    void render(Camera *camera, Material *material, glm::mat4 &parent_model) override;
    void render(Camera *camera, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;
    void push_mesh(Scoped<Mesh> mesh); // push a mesh onto the mesh stack
    void pop_mesh();                   // remove the last mesh on the mesh stack
    void clear();                      // clear the entire mesh stack

protected:
    std::vector<Scoped<Mesh>> meshes_{};
};

class InstancedMesh : public Mesh
{
public:
    InstancedMesh(unsigned int max_instances);
    virtual ~InstancedMesh() {}
    void render(Camera *camera, Material *material) override;
    void render(Camera *camera, Material *material, glm::mat4 &parent_model) override;
    void render(Camera *camera, Material *material, glm::mat4 &parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;
    void set_mesh(Scoped<Mesh> mesh);
    void push_instance(glm::mat4 model);
    void pop_instance();
    void clear_instances();
    void flush_instances(std::vector<glm::mat4> *models, uint32_t offset);
    glm::mat4 &operator[](unsigned int i);
    glm::mat4 operator[](unsigned int i) const;
    Buffer<glm::mat4>* get_instance_models();
    Scoped<Buffer<glm::mat4>> swap_instance_models(Scoped<Buffer<glm::mat4>> models);

protected:
    unsigned int instance_count_;
    Scoped<Buffer<glm::mat4>> instance_transforms_;
    Scoped<Mesh> mesh_;
    unsigned int max_instances_;
};

} // namespace mare

#endif