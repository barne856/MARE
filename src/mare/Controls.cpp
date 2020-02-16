#include "mare/Camera.hpp"
#include "mare/Controls.hpp"
#include "mare/Renderer.hpp"
#include <cmath>

namespace mare
{

const float PI = 3.141592653f;

void OrbitControls::interpret_input(Camera *camera, RendererInput &input)
{
    if (input.mouse_button == 1)
    {
        // orbit around center and adjust inclination
        float sensitivity = 300.0f;
        float dtheta = -float(input.mouse_vel.y) / sensitivity;
        float dphi = -float(input.mouse_vel.x) / sensitivity;
        glm::vec3 dir = camera->get_direction();
        glm::vec3 pos = camera->get_position();
        glm::vec3 up = camera->get_up();

        glm::vec3 center = pos + dir * camera->get_distance_to_center();

        float r_xy = glm::length(glm::vec2(pos.x, pos.y) - glm::vec2(center.x, center.y));
        if (r_xy == 0.0f)
        {
            float phi = PI / 2.0f;

            if (up.x > 0.0f)
            {
                phi = atan((up.y) / (up.x));
            }
            else if (up.x < 0.0f)
            {
                phi = atan((up.y) / (up.x)) + PI;
            }
            else if (up.y < 0.0f)
            {
                phi = -PI / 2.0f;
            }
            up = {cos(phi + dphi), sin(phi + dphi), 0.0f};
            if (dtheta > 0.0f)
            {
                up = {0.0f, 0.0f, 1.0f};
                float r = glm::length(pos - center);
                pos.x = r * sin(dtheta) * cos(phi + dphi + PI) + center.x;
                pos.y = r * sin(dtheta) * sin(phi + dphi + PI) + center.y;
                pos.z = r * cos(dtheta) + center.z;
            }
        }
        else
        {

            float phi = PI;

            if (pos.x - center.x > 0.0f)
            {
                phi = atan((pos.y - center.y) / (pos.x - center.x));
            }
            else if (pos.x - center.x < 0.0f)
            {
                phi = atan((pos.y - center.y) / (pos.x - center.x)) + PI;
            }
            else if (pos.y - center.y < 0.0f)
            {
                phi = -PI / 2.0f;
            }
            float r = glm::length(pos - center);
            float h = pos.z - center.z;
            float theta = PI / 2.0f - atan(h / r_xy);

            if (theta + dtheta > PI / 2.0f)
            {
                dtheta = PI / 2.0f - theta;
            }
            if (theta + dtheta < 0.0f)
            {
                dtheta = -theta;
                up = {dir.x, dir.y, 0.0f};
            }
            pos.x = r * sin(theta + dtheta) * cos(phi + dphi) + center.x;
            pos.y = r * sin(theta + dtheta) * sin(phi + dphi) + center.y;
            pos.z = r * cos(theta + dtheta) + center.z;
        }
        camera->set_view(pos, center, up);
    }
}

void FlyControls::interpret_input(Camera *camera, RendererInput &input)
{
    float sensitivity = 300.0f;
    float speed = 1.0f;
    float dtheta = float(input.mouse_vel.y) / sensitivity;
    float dphi = -float(input.mouse_vel.x) / sensitivity;
    glm::vec3 dir = camera->get_direction();
    glm::vec3 up = camera->get_up();

    float theta = acosf(dir.z);
    float phi = atan2f(dir.y, dir.x);

    theta = glm::clamp(theta + dtheta, 0.01f, PI - 0.01f);
    phi += dphi;

    dir = glm::vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta));
    glm::vec3 right = glm::normalize(glm::cross(dir, {0.0f, 0.0f, 1.0f}));
    up = glm::cross(right, dir);

    camera->set_direction(dir);
    camera->set_up(up);
    camera->recalculate_view();

    float x = 0.0f;
    float y = 0.0f;
    if (input.W_PRESSED)
    {
        y += 1.0f;
    }
    if (input.S_PRESSED)
    {
        y -= 1.0f;
    }
    if (input.D_PRESSED)
    {
        x += 1.0f;
    }
    if (input.A_PRESSED)
    {
        x -= 1.0f;
    }
    if (x || y)
    {
        x /= sqrtf(x * x + y * y);
        y /= sqrtf(x * x + y * y);
    }
    glm::vec3 velocity = (dir*y + right*x)*speed;
    camera->set_linear_velocity(velocity);

}

} // namespace mare