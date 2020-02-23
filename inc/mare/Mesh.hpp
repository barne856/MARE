#ifndef MESH
#define MESH

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Material.hpp"
#include "mare/Buffer.hpp"

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare
{
class Layer; // forward declaration
class Mesh : public Asset
{
public:
    Mesh() : translation_(glm::vec3(0.0f)), rotation_(glm::mat4(1.0f)), scale_(glm::vec3(1.0f)), transform_(glm::mat4(1.0f)), normal_(glm::mat3(1.0f)) {}
    virtual ~Mesh() {}

    virtual void render(const Layer *layer, Material *material) = 0;
    virtual void render(const Layer *layer, Material *material, const glm::mat4 &parent_model) = 0;
    virtual void render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) = 0;
    void translate(glm::vec3 translation);
    void set_position(glm::vec3 position);
    void rotate(glm::vec3 axis, float angle);
    void set_scale(glm::vec3 scale);
    inline glm::vec3 get_position() const { return translation_; }
    inline glm::vec3 get_scale() const { return scale_; }
    inline glm::mat4 get_model() const { return transform_; }
    inline glm::mat3 get_normal() const { return normal_; }

protected:
    void update_model_matrix()
    {
        transform_ = glm::translate(glm::mat4(1.0f), translation_) * rotation_ * glm::scale(glm::mat4(1.0f), scale_);
        normal_ = glm::transpose(glm::inverse(glm::mat3(rotation_ * glm::scale(glm::mat4(1.0f), scale_))));
    }

    glm::vec3 translation_;
    glm::mat4 rotation_;
    glm::vec3 scale_;
    glm::mat4 transform_;
    glm::mat3 normal_;
};

} // namespace mare

#endif