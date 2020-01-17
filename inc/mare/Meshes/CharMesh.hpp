#ifndef CHARMESH
#define CHARMESH

#include "mare/Mesh.hpp"
#include "glm.hpp"

namespace mare
{

class CharMesh : public virtual Mesh
{
public:
    CharMesh(std::string str, float keying = 1.0f) {}
    ~CharMesh() {}
    virtual glm::vec2 get_center() const = 0;
    virtual void set_center(glm::vec3 center) = 0;
};
} // namespace mare
#endif