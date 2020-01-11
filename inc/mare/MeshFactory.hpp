#ifndef MESHFACTORY
#define MESHFACTORY

// MARE
#include "mare/Mesh.hpp"
// External Libraries
#include "glm.hpp"

namespace mare
{
    class MeshFactory
    {
        public:
        virtual ~MeshFactory(){}
        virtual Mesh* GenTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) = 0;
    };
}

#endif