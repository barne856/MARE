#ifndef ENTITY
#define ENTITY

// MARE
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"
#include "mare/Transform.hpp"

namespace mare {

class Entity : public Transform {
public:
  Entity() {}
  virtual ~Entity() {}

  // Component Template Functions--------------------------------

  // generate single component and push it onto the stack
  template <typename T, typename... Args> void gen_component(Args... args) {
    // generate component reference of type T and cast to base type
    Referenced<Component> comp =
        std::dynamic_pointer_cast<Component>(gen_ref<T>(args...));
    // if T is a component push the component onto the stack
    if (comp)
      components_.push_back(comp);
  }
  // generate multiple components and push them onto the stack
  template <typename T, typename... Args>
  void gen_components(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate component reference of type T and cast to base type
      Referenced<Component> comp =
          std::dynamic_pointer_cast<Component>(gen_ref<T>(args...));
      // if T is a component push the component onto the stack
      if (comp)
        components_.push_back(comp);
    }
  }
  // get first component in the stack of a given type
  template <typename T> T *get_component() {
    for (auto comp_it = component_begin(); comp_it != component_end();
         comp_it++) {
      if (auto component = std::dynamic_pointer_cast<T>(*comp_it)) {
        return component.get();
      }
    }
    return nullptr;
  }
  // get all components of an interface type
  template <typename T> std::vector<T *> get_components() {
    std::vector<T *> components{};
    for (auto comp_it = component_begin(); comp_it != component_end();
         comp_it++) {
      if (auto component = std::dynamic_pointer_cast<T>(*comp_it)) {
        components.push_back(component.get());
      }
    }
    return components;
  }
  // push an existing reference onto the stack
  template <typename T> void push_component(Referenced<T> component) {
    Referenced<Component> comp =
        std::dynamic_pointer_cast<Component>(component);
    if (comp)
      components_.push_back(comp);
  }
  // push a vector of existing references onto the stack
  template <typename T>
  void push_components(std::vector<Referenced<T>> components) {
    for (size_t i = 0; i < components.size(); i++) {
      Referenced<Component> comp =
          std::dynamic_pointer_cast<Component>(components[i]);
      if (comp)
        components_.push_back(comp);
    }
  }
  // remove first component of a given type from the stack and return it
  template <typename T> Referenced<T> pull_component() {
    for (auto comp_it = component_begin(); comp_it != component_end();
         comp_it++) {
      if (auto component = std::dynamic_pointer_cast<T>(*comp_it)) {
        Referenced<T> pulled_component = std::move(component);
        components_.erase(comp_it);
        return pulled_component;
      }
    }
    return nullptr;
  }
  // remove all components of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_components() {
    std::vector<T *> components{};
    for (auto comp_it = component_begin(); comp_it != component_end();
         comp_it++) {
      if (auto component = std::dynamic_pointer_cast<T>(*comp_it)) {
        Referenced<T> pulled_component = std::move(component);
        components_.erase(comp_it);
        components.push_back(pulled_component);
      }
    }
    return components;
  }

  // Asset Template Functions--------------------------------

  // generate single asset and push it onto the stack
  template <typename T, typename... Args> void gen_asset(Args... args) {
    // generate asset reference of type T and cast to base type
    Referenced<Asset> asst =
        std::dynamic_pointer_cast<Asset>(gen_ref<T>(args...));
    // if T is a asset push the asset onto the stack
    if (asst)
      assets_.push_back(asst);
  }
  // generate multiple assets and push them onto the stack
  template <typename T, typename... Args>
  void gen_assets(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate asset reference of type T and cast to base type
      Referenced<Component> asst =
          std::dynamic_pointer_cast<Component>(gen_ref<T>(args...));
      // if T is a asset push the asset onto the stack
      if (asst)
        assets_.push_back(asst);
    }
  }
  // get first asset in the stack of a given type
  template <typename T> T *get_asset() {
    for (auto asst_it = asset_begin(); asst_it != asset_end(); asst_it++) {
      if (auto asset = std::dynamic_pointer_cast<T>(*asst_it)) {
        return asset.get();
      }
    }
    return nullptr;
  }
  // get all assets of an interface or concrete type
  template <typename T> std::vector<T *> get_assets() {
    std::vector<T *> assets{};
    for (auto asst_it = asset_begin(); asst_it != asset_end(); asst_it++) {
      if (auto asset = std::dynamic_pointer_cast<T>(*asst_it)) {
        assets.push_back(asset.get());
      }
    }
    return assets;
  }
  // push an existing reference onto the stack
  template <typename T> void push_asset(Referenced<T> asset) {
    Referenced<Component> asst = std::dynamic_pointer_cast<Component>(asset);
    if (asst)
      assets_.push_back(asst);
  }
  // push a vector of existing references onto the stack
  template <typename T> void push_assets(std::vector<Referenced<T>> assets) {
    for (size_t i = 0; i < assets.size(); i++) {
      Referenced<Component> asst =
          std::dynamic_pointer_cast<Component>(assets[i]);
      if (asst)
        assets_.push_back(asst);
    }
  }
  // remove first asset of a given type from the stack and return it
  template <typename T> Referenced<T> pull_asset() {
    for (auto asst_it = asset_begin(); asst_it != asset_end(); asst_it++) {
      if (auto asset = std::dynamic_pointer_cast<T>(*asst_it)) {
        Referenced<T> pulled_asset = std::move(asset);
        assets_.erase(asst_it);
        return pulled_asset;
      }
    }
    return nullptr;
  }
  // remove all assets of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_assets() {
    std::vector<T *> assets{};
    for (auto asst_it = asset_begin(); asst_it != asset_end(); asst_it++) {
      if (auto asset = std::dynamic_pointer_cast<T>(*asst_it)) {
        Referenced<T> pulled_asset = std::move(asset);
        assets_.erase(asst_it);
        assets.push_back(pulled_asset);
      }
    }
    return assets;
  }
  //----------------------------

  std::vector<Referenced<Component>>::const_iterator component_begin() const {
    return components_.begin();
  }
  std::vector<Referenced<Component>>::const_iterator component_end() const {
    return components_.end();
  }
  std::vector<Referenced<Component>>::const_reverse_iterator
  component_rbegin() const {
    return components_.rbegin();
  }
  std::vector<Referenced<Component>>::const_reverse_iterator
  component_rend() const {
    return components_.rend();
  }
  std::vector<Referenced<Component>>::iterator component_begin() {
    return components_.begin();
  }
  std::vector<Referenced<Component>>::iterator component_end() {
    return components_.end();
  }
  std::vector<Referenced<Component>>::reverse_iterator component_rbegin() {
    return components_.rbegin();
  }
  std::vector<Referenced<Component>>::reverse_iterator component_rend() {
    return components_.rend();
  }

  std::vector<Referenced<Asset>>::const_iterator asset_begin() const {
    return assets_.begin();
  }
  std::vector<Referenced<Asset>>::const_iterator asset_end() const {
    return assets_.end();
  }
  std::vector<Referenced<Asset>>::const_reverse_iterator asset_rbegin() const {
    return assets_.rbegin();
  }
  std::vector<Referenced<Asset>>::const_reverse_iterator asset_rend() const {
    return assets_.rend();
  }
  std::vector<Referenced<Asset>>::iterator asset_begin() {
    return assets_.begin();
  }
  std::vector<Referenced<Asset>>::iterator asset_end() { return assets_.end(); }
  std::vector<Referenced<Asset>>::reverse_iterator asset_rbegin() {
    return assets_.rbegin();
  }
  std::vector<Referenced<Asset>>::reverse_iterator asset_rend() {
    return assets_.rend();
  }

private:
  std::vector<Referenced<Component>> components_;
  std::vector<Referenced<Asset>> assets_;
};
} // namespace mare

#endif