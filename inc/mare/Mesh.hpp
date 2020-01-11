#ifndef MESH
#define MESH

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare
{

class Mesh
{
public:
    Mesh() : translation(glm::vec3(0.0f)), rotation(glm::mat4(1.0f)), scale(glm::vec3(0.0f)), transform(glm::mat4(1.0f)) {}
    virtual ~Mesh() {}
    virtual void render() = 0;
    void translate(glm::vec3 translation)
    {
        this->translation += translation;
        update_model_matrix();
    }
    void rotate(glm::vec3 axis, float angle)
    {
        rotation = glm::rotate(rotation, angle, axis);
        update_model_matrix();
    }
    void size(glm::vec3 scale)
    {
        this->scale = scale;
        update_model_matrix();
    }
    glm::mat4 get_model() const
    {
        return transform;
    }

private:

    void update_model_matrix()
    {
        transform = glm::translate(glm::mat4(1.0), translation) * rotation * glm::scale(glm::mat4(1.0), scale);
    }

    glm::vec3 translation;
    glm::mat4 rotation;
    glm::vec3 scale;
    glm::mat4 transform;
};

} // namespace mare

#endif