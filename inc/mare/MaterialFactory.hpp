#ifndef MATERIALFACTORY
#define MATERIALFACTORY

// MARE
#include "mare/Material.hpp"
#include "mare/Materials/BasicMaterial.hpp"
// External Libraries
#include "glm.hpp"

namespace mare
{
    class MaterialFactory
    {
        public:
        virtual ~MaterialFactory(){}
        virtual BasicMaterial* GenBasicMaterial() = 0;
        virtual Material* GenMaterial(const char *directory) = 0;
    };
}

#endif