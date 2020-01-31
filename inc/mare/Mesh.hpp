#ifndef MESH
#define MESH

// MARE
#include "mare/Material.hpp"
#include "mare/Buffer.hpp"

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare
{
class Layer; // forward declaration
class Mesh
{
public:
    Mesh() : translation(glm::vec3(0.0f)), rotation(glm::mat4(1.0f)), scale(glm::vec3(1.0f)), transform(glm::mat4(1.0f)), normal(glm::mat3(1.0f)) {}
    virtual ~Mesh() {}

    virtual void render(Layer* layer, Material *material) = 0;
    virtual void render(Layer* layer, Material *material, glm::mat4 parent_model) = 0;
    virtual void render(Layer* layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
    void translate(glm::vec3 translation);
    void set_position(glm::vec3 position);
    void rotate(glm::vec3 axis, float angle);
    void set_scale(glm::vec3 scale);
    inline glm::vec3 get_position() const { return translation; }
    inline glm::vec3 get_scale() const { return scale; }
    inline glm::mat4 get_model() const { return transform; }
    inline glm::mat3 get_normal() const { return normal; }

protected:
    void update_model_matrix()
    {
        transform = glm::translate(glm::mat4(1.0f), translation) * rotation * glm::scale(glm::mat4(1.0f), scale);
        normal = glm::transpose(glm::inverse(glm::mat3(rotation * glm::scale(glm::mat4(1.0f), scale))));
    }

    glm::vec3 translation;
    glm::mat4 rotation;
    glm::vec3 scale;
    glm::mat4 transform;
    glm::mat3 normal;
};

} // namespace mare

#endif