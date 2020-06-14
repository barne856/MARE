#ifndef ENTITY
#define ENTITY

// MARE
#include "Components/Transform.hpp"
#include "Mare.hpp"
#include "Systems.hpp"

namespace mare {

/**
 * @brief An Entity is an element of a Scene or Layer.
 * @details An Entity is the base class for all user created Entities. All
 * Entities contain a *system stack* and inherit from zero or more Component(s).
 * @see System
 * @see Transform
 */
class Entity : virtual public Transform {
public:
  /**
   * @brief Construct a new Entity object
   */
  Entity() {}
  /**
   * @brief Construct a new Entity object with an initial transform
   *
   * @param transform The initial transform to set.
   */
  Entity(glm::mat4 transform) : Transform(transform) {}
  /**
   * @brief Destroy the Entity object
   */
  virtual ~Entity() {}

  /**
   * @brief Generate a System and push it onto the System stack.
   * @details This function will generate a new reference to a System of type
   * <T> and push it onto the Entity's System stack. All Systems that are
   * attached to Entities in a Layer are active and receive callbacks from the
   * Engine. A Referenced System is returned that can be used and will be
   * deleted when it goes out of scope.
   *
   * @tparam <T> The type of System. <T> must be a System.
   * @tparam <Args> The type of the arguments for the Contructor of the System.
   * @param args The arguments for the Constructor of the System.
   * @return The Referenced System.
   * @see System
   */
  template <typename T, typename... Args>
  Referenced<T> gen_system(Args... args) {
    static_assert(std::is_base_of<System, T>::value);
    Referenced<T> sys = gen_ref<T>(args...);
    systems_.push_back(sys);
    return sys;
  }
  /**
   * @brief Generate multiple Systems and push them onto the System stack.
   * @details This function will create multiple Systems of the same type and
   * push them all onto the System stack. A `std::vector` of Referenced Systems
   * is returned that can be used and will be deleted when it goes out of
   * scope. This function does not have a lot of use but is included for
   * consistency.
   *
   * @tparam <T> The type of Systems to generate. <T> must be a System.
   * @tparam <Args> The type of the arguments for the Contructor of the System.
   * @param args The arguments for the Constructor of the System.
   * @param count The number of Systems to generate.
   * @see System
   */
  template <typename T, typename... Args>
  std::vector<Referenced<T>> gen_systems(uint32_t count, Args... args) {
    static_assert(std::is_base_of<System, T>::value);
    std::vector<Referenced<T>> systems{};
    for (uint32_t i = 0; i < count; i++) {
      Referenced<T> sys = gen_ref<T>(args...);
      systems_.push_back(sys);
      systems.push_back(sys);
    }
    return systems;
  }
  /**
   * @brief Get a pointer to the first System of type <T> in the Entity's
   * System stack.
   *
   * @tparam <T> The type of System to get. <T> must be a System.
   * @return A pointer to the first System of type <T> in the System stack.
   * nullptr is returned if no System of type <T> exists on the System stack.
   * @see System
   */
  template <typename T> T *get_system() {
    static_assert(std::is_base_of<System, T>::value);
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        return system.get();
      }
    }
    return nullptr;
  }
  /**
   * @brief Get a `std::vector` of pointers to all of the Systems of type <T> in
   * the Entitiy's System stack.
   *
   * @tparam <T> The type of System to get. <T> must be a System.
   * @return A `std::vector` of pointers to all of the Systems of type <T> in
   * the System stack. An empty vector is returned if no System of type <T>
   * exists on the System stack.
   * @see System
   */
  template <typename T> std::vector<T *> get_systems() {
    static_assert(std::is_base_of<System, T>::value);
    std::vector<T *> systems{};
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        systems.push_back(system.get());
      }
    }
    return systems;
  }
  /**
   * @brief Push an existing Referenced System onto the Entity's System stack.
   *
   * @param system A Referenced System.
   * @see System
   */
  void push_system(Referenced<System> system) { systems_.push_back(system); }
  /**
   * @brief Push a `std::vector` of existing Referenced Systems onto the
   * Entity's System stack.
   *
   * @param systems A `std::vector` of existing Referenced Systems.
   * @see System
   */
  void push_systems(std::vector<Referenced<System>> systems) {
    for (size_t i = 0; i < systems.size(); i++) {
      systems_.push_back(systems[i]);
    }
  }
  /**
   * @brief Remove the first System of type <T> from the System stack and return
   * the Referenced System.
   *
   * @tparam <T> The type of System to pull.
   * @return The Referenced System that was pulled from the System stack.
   * @see System
   */
  template <typename T> Referenced<T> pull_system() {
    static_assert(std::is_base_of<System, T>::value);
    for (auto sys_it = systems_begin(); sys_it != systems_end(); sys_it++) {
      if (auto system = std::dynamic_pointer_cast<T>(*sys_it)) {
        Referenced<T> pulled_system = system;
        systems_.erase(sys_it);
        return pulled_system;
      }
    }
    return nullptr;
  }
  /**
   * @brief Remove all of the Systems of type <T> from the System stack and
   * return a `std::vector` of the Referenced Systems.
   *
   * @tparam <T> The type of Systems to pull.
   * @return A `std::vector` of the Referenced Systems that were pulled from the
   * System stack.
   * @see System
   */
  template <typename T> std::vector<Referenced<T>> pull_systems() {
    static_assert(std::is_base_of<System, T>::value);
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

  /**
   * @brief Get a const iterator pointing to the begining of the System stack.
   *
   * @return A const iterator pointing to the begining of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::const_iterator systems_begin() const {
    return systems_.begin();
  }
  /**
   * @brief Get a const iterator pointing to the end of the System stack.
   *
   * @return A const iterator pointing to the end of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::const_iterator systems_end() const {
    return systems_.end();
  }
  /**
   * @brief Get a const reverse iterator pointing to the end of the System
   * stack.
   *
   * @return A const reverse iterator pointing to the end of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::const_reverse_iterator
  systems_rbegin() const {
    return systems_.rbegin();
  }
  /**
   * @brief Get a const reverse iterator pointing to the begining of the System
   * stack.
   *
   * @return A const reverse iterator pointing to the begining of the System
   * stack.
   * @see System
   */
  std::vector<Referenced<System>>::const_reverse_iterator systems_rend() const {
    return systems_.rend();
  }
  /**
   * @brief Get a iterator pointing to the begining of the System stack.
   *
   * @return An iterator pointing to the begining of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::iterator systems_begin() {
    return systems_.begin();
  }
  /**
   * @brief Get a iterator pointing to the end of the System stack.
   *
   * @return An iterator pointing to the end of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::iterator systems_end() {
    return systems_.end();
  }
  /**
   * @brief Get a reverse iterator pointing to the end of the System
   * stack.
   *
   * @return A reverse iterator pointing to the end of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::reverse_iterator systems_rbegin() {
    return systems_.rbegin();
  }
  /**
   * @brief Get a reverse iterator pointing to the begining of the System
   * stack.
   *
   * @return A reverse iterator pointing to the begining of the System stack.
   * @see System
   */
  std::vector<Referenced<System>>::reverse_iterator systems_rend() {
    return systems_.rend();
  }

private:
  std::vector<Referenced<System>> systems_; /**< The System stack.*/
};
} // namespace mare

#endif