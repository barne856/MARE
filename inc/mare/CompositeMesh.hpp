#ifndef COMPOSITEMESH
#define COMPOSITEMESH

// Standard Library
#include <vector>
// MARE
#include "mare/Mesh.hpp"

namespace mare
{
class CompositeMesh : public Mesh
{
public:
    virtual ~CompositeMesh() {}
    virtual void render() = 0;

private:
    std::vector<Mesh*> m_meshes {};
};
} // namespace mare

#endif