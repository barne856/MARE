#ifndef WIDGET
#define WIDGET

// MARE
#include "mare/Object.hpp"
#include "mare/Layer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{

class Widget : public Layer, public Object
{
public:
    using Object::render;
    Widget() {}
    virtual ~Widget() {}

};
} // namespace mare

#endif