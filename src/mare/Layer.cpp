// MARE
#include "mare/Layer.hpp"

namespace mare
{
Layer::Layer() : m_camera(nullptr) {}
Layer::~Layer()
{
    if (m_camera)
    {
        delete m_camera;
        m_camera = nullptr;
    }
}
} // namespace mare