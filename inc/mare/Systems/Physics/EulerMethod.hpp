#ifndef EULERMETHOD
#define EULERMETHOD

// MARE
#include "mare/Components/Rigidbody.hpp"
#include "mare/Systems.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {

/**
 * @brief A PhysicsSystem that operates on a Rigidbody Component.
 * @details The EulerMethod System uses the Euler Method to compute the solution
 * to the second-order differential equation describing the equation of motion
 * of a Rigidbody.
 *
 *  \f[
 *    \vv{\bm{F}} = m\vv{\bm{a}} = m\frac{\mathrm{d}\vv{\bm{v}}}{\mathrm{d}t} = m\frac{\mathrm{d}^2\vv{\bm{x}}}{\mathrm{d}t^2}
 *  \f]
 *
 */
class EulerMethod : public PhysicsSystem<Rigidbody> {
public:
  void update(float dt, Rigidbody *rb) override {
    // euler method to calculate velocity and position
    rb->linear_velocity += rb->force * dt;
    rb->translate(rb->linear_velocity * dt);
  }
};
} // namespace mare

#endif