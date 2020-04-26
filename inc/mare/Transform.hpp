#ifndef TRANSFORM
#define TRANSFORM

// MARE
#include "mare/Mare.hpp" // Asset

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare
{
class Transform : public Asset
{
public:
    Transform() : translation_(glm::vec3(0.0f)), rotation_(glm::mat4(1.0f)), scale_(glm::vec3(1.0f)), transform_(glm::mat4(1.0f)), normal_(glm::mat3(1.0f)), up_vector_(glm::vec3(0.0f, 1.0f, 0.0f)), forward_vector_(glm::vec3(0.0f, 0.0f, -1.0f)) {}
    virtual ~Transform() {}
    void translate(glm::vec3 translation);
    void set_position(glm::vec3 position);
    void rotate(glm::vec3 axis, float angle);
    void set_rotation(glm::vec3 axis, float angle);
    void set_scale(glm::vec3 scale);
    void set_up_vector(glm::vec3 up);
    void set_forward_vector(glm::vec3 forward);
    void face_towards(glm::vec3 direction);
    glm::mat4 get_view() const;
    glm::vec3 get_direction() const;
    glm::vec3 get_up_vector() const;
    inline glm::vec3 get_position() const { return translation_; }
    inline glm::vec3 get_scale() const { return scale_; }
    inline glm::mat4 get_model() const { return transform_; }
    inline glm::mat3 get_normal() const { return normal_; }

protected:
    void update_model_matrix();

    glm::vec3 translation_;
    glm::mat4 rotation_;
    glm::vec3 scale_;
    glm::mat4 transform_;
    glm::mat3 normal_;
    glm::vec3 up_vector_;
    glm::vec3 forward_vector_;
};

} // namespace mare

#endif