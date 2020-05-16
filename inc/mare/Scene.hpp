#ifndef SCENE
#define SCENE

// MARE
#include "mare/Layer.hpp"
#include "mare/Mare.hpp"

// Standard Library
#include <vector>

namespace mare {
class Scene : public Layer {
public:
  Scene(ProjectionType type) : Layer(type) {}
  virtual ~Scene() {}

  // Layer Template Functions--------------------------------

  // generate single layer and push it onto the stack
  template <typename T, typename... Args> void gen_layer(Args... args) {
    // generate layer reference of type T and cast to base type
    Referenced<Layer> layr =
        std::dynamic_pointer_cast<Layer>(gen_ref<T>(args...));
    // if T is a layer push the asset onto the stack
    if (layr)
      layers_.push_back(layr);
  }
  // generate multiple layers and push them onto the stack
  template <typename T, typename... Args>
  void gen_layers(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate layer reference of type T and cast to base type
      Referenced<Layer> layr =
          std::dynamic_pointer_cast<Layer>(gen_ref<T>(args...));
      // if T is a layer push the asset onto the stack
      if (layr)
        layers_.push_back(layr);
    }
  }
  // get first layer in the stack of a given type
  template <typename T> T *get_layer() {
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (auto layer = std::dynamic_pointer_cast<T>(*layr_it)) {
        return layer.get();
      }
    }
    return nullptr;
  }
  // get all layers of an interface or concrete type
  template <typename T> std::vector<T *> get_layers() {
    std::vector<T *> layers{};
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (auto layer = std::dynamic_pointer_cast<T>(*layr_it)) {
        layers.push_back(layer.get());
      }
    }
    return layers;
  }
  // push an existing layer onto the stack
  template <typename T> void push_layer(Referenced<T> layer) {
    Referenced<Layer> layr = std::dynamic_pointer_cast<Layer>(layer);
    if (layr)
      layers_.push_back(layr);
  }
  // push a vector of existing layers onto the stack
  template <typename T> void push_layers(std::vector<Referenced<T>> layers) {
    for (size_t i = 0; i < layers.size(); i++) {
      Referenced<Layer> layr = std::dynamic_pointer_cast<Layer>(layers[i]);
      if (layr)
        layers_.push_back(layr);
    }
  }
  // remove first layer of a given type from the stack and return it
  template <typename T> Referenced<T> pull_layer() {
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (auto layer = std::dynamic_pointer_cast<T>(*layr_it)) {
        Referenced<T> pulled_layer = std::move(layer);
        layers_.erase(layr_it);
        return pulled_layer;
      }
    }
    return nullptr;
  }
  // remove all layers of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_layers() {
    std::vector<T *> layers{};
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (auto layer = std::dynamic_pointer_cast<T>(*layr_it)) {
        Referenced<T> pulled_layer = std::move(layer);
        layers_.erase(layr_it);
        layers.push_back(pulled_layer);
      }
    }
    return layers;
  }
  //----------------------------

  std::vector<Referenced<Layer>>::const_iterator layer_begin() const {
    return layers_.begin();
  }
  std::vector<Referenced<Layer>>::const_iterator layer_end() const {
    return layers_.end();
  }
  std::vector<Referenced<Layer>>::const_reverse_iterator layer_rbegin() const {
    return layers_.rbegin();
  }
  std::vector<Referenced<Layer>>::const_reverse_iterator layer_rend() const {
    return layers_.rend();
  }
  std::vector<Referenced<Layer>>::iterator layer_begin() {
    return layers_.begin();
  }
  std::vector<Referenced<Layer>>::iterator layer_end() { return layers_.end(); }
  std::vector<Referenced<Layer>>::reverse_iterator layer_rbegin() {
    return layers_.rbegin();
  }
  std::vector<Referenced<Layer>>::reverse_iterator layer_rend() {
    return layers_.rend();
  }

private:
  std::vector<Referenced<Layer>> layers_{};
};
} // namespace mare

#endif