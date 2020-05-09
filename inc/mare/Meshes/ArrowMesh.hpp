#ifndef ARROWMESH
#define ARROWMESH

#include "mare/Meshes.hpp"
#include "mare/Meshes/ConeMesh.hpp"
#include "mare/Meshes/CylinderMesh.hpp"

#include "glm.hpp"

namespace mare
{
class ArrowMesh : public CompositeMesh
{
public:
    ArrowMesh(float body_length, float body_width, float head_length, float head_width, unsigned int sides)
    {
        head = gen_scoped<ConeMesh>(0.5f, sides);
        body = gen_scoped<CylinderMesh>(0.0f, 2.0f*3.141592653f, sides);
        body->set_scale({body_width, body_width, body_length});
        head->set_scale({head_width, head_width, head_length});
        head->set_position({0.0f, 0.0f, body_length});
        push_mesh(std::move(body));
        push_mesh(std::move(head));
    }

private:
    Scoped<ConeMesh> head;
    Scoped<CylinderMesh> body;
};
} // namespace mare

#endif