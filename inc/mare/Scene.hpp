#ifndef SCENE
#define SCENE

// MARE
#include "mare/Layer.hpp"
#include "mare/Overlay.hpp"
#include "mare/Object.hpp"

// Standard Library
#include <vector>

namespace mare
{
class Scene : public Layer
{
public:
    Scene() : m_layer_stack(nullptr)
    {
        m_layer_stack = new std::vector<Layer *>();
        m_layer_stack->push_back(this);
    }
    virtual ~Scene()
    {
        for (size_t i = m_layer_stack->size(); i--;)
        {
            if (i != 0)
            {
                delete m_layer_stack->at(i);
                m_layer_stack->at(i) = nullptr;
            }
        }
        delete m_layer_stack;
        m_layer_stack = nullptr;
    }
    widget_value get_widget_value(size_t overlay_index, size_t widget_index)
    {
        return static_cast<Overlay *>(get_layer(overlay_index))->get_widget(widget_index)->get_value();
    }
    Layer *get_layer(size_t index)
    {
        return (*m_layer_stack)[index + 1];
    }
    std::vector<Layer *> *get_layer_stack()
    {
        return m_layer_stack;
    }
    void push_layer(Layer *layer)
    {
        m_layer_stack->push_back(layer);
    }

private:
    std::vector<Layer *> *m_layer_stack;
};
} // namespace mare

#endif