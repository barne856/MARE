#ifndef GLMATERIALFACTORY
#define GLMATERIALFACTORY

// MARE
#include "mare/GL/Materials/GLBasicMaterial.hpp"
#include "mare/MaterialFactory.hpp"
// External Libraries
#include "glm.hpp"

namespace mare
{
    class GLMaterialFactory : public MaterialFactory
    {
        public:
        virtual ~GLMaterialFactory(){}
        BasicMaterial* GenBasicMaterial() override;
    };
}

#endif