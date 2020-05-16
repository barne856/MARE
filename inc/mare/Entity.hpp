#ifndef ENTITY
#define ENTITY

// MARE
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

class Entity {
public:
  Entity() {}
  virtual ~Entity() {}

  // System Template Functions--------------------------------

  // generate single component and push it onto the stack
  template <typename T, typename... Args> void gen_system(Args... args) {
    // generate system reference of type T and cast to base type
    Referenced<System> sys =
        std::dynamic_pointer_cast<System>(gen_ref<T>(args...));
    // if T is a system push the system onto the stack
    if (sys)
      systems_.push_back(sys);
  }
  // generate multiple systems and push them onto the stack
  template <typename T, typename... Args>
  void gen_systems(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate system reference of type T and cast to base type
      Referenced<System> sys =
          std::dynamic_pointer_cast<System>(gen_ref<T>(args...));
      // if T is a system push the system onto the stack
      if (sys)
        systems_.push_back(sys);
    }
  }
  // get first system in the stack of a given type
  template <typename T> T *get_system() {
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        return system.get();
      }
    }
    return nullptr;
  }
  // get all systems of an interface type
  template <typename T> std::vector<T *> get_systems() {
    std::vector<T *> systems{};
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        systems.push_back(system.get());
      }
    }
    return systems;
  }
  // push an existing system onto the stack
  void push_system(Referenced<System> system) {
      systems_.push_back(system);
  }
  // push a vector of existing systems onto the stack
  void push_systems(std::vector<Referenced<System>> systems) {
    for (size_t i = 0; i < systems.size(); i++) {
      systems_.push_back(systems[i]);
    }
  }
  // remove first system of a given type from the stack and return it
  template <typename T> Referenced<T> pull_system() {
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        Referenced<T> pulled_system = system;
        systems_.erase(sys_it);
        return pulled_system;
      }
    }
    return nullptr;
  }
  // remove all systems of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_systems() {
    std::vector<T *> systems{};
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        Referenced<T> pulled_system = system;
        systems_.erase(sys_it);
        systems.push_back(pulled_system);
      }
    }
    return systems;
  }
  //----------------------------

  std::vector<Referenced<System>>::const_iterator systems_begin() const {
    return systems_.begin();
  }
  std::vector<Referenced<System>>::const_iterator systems_end() const {
    return systems_.end();
  }
  std::vector<Referenced<System>>::const_reverse_iterator
  systems_rbegin() const {
    return systems_.rbegin();
  }
  std::vector<Referenced<System>>::const_reverse_iterator systems_rend() const {
    return systems_.rend();
  }
  std::vector<Referenced<System>>::iterator systems_begin() {
    return systems_.begin();
  }
  std::vector<Referenced<System>>::iterator systems_end() {
    return systems_.end();
  }
  std::vector<Referenced<System>>::reverse_iterator systems_rbegin() {
    return systems_.rbegin();
  }
  std::vector<Referenced<System>>::reverse_iterator systems_rend() {
    return systems_.rend();
  }

private:
  std::vector<Referenced<System>> systems_;
};
} // namespace mare

#endif