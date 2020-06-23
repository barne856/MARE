#ifndef SCENE
#define SCENE

// MARE
#include "Layer.hpp"
#include "Mare.hpp"

// Standard Library
#include <vector>

namespace mare {
/**
 * @brief An interface for a Scene object.
 * @details The user can inherit from this class to create their own Scenes to
 * render. All Scenes have an Entity stack inherited from Layer and a Layer
 * Stack. Scenes also inherit from Camera and so are also Entities themselves.
 * This allows Systems to be added to Scenes and Components to add functionality
 * to Scenes though inheritence.
 * @see Layer
 * @see Entity
 * @see System
 * @see Camera
 * @see ProjectionType
 */
class Scene : public Layer {
public:
  /**
   * @brief Construct a new Scene object
   *
   * @param type The ProjectionType of the Scene's Camera.
   */
  Scene(ProjectionType type) : Layer(type) {}
  /**
   * @brief Virtual destructor for the Scene object
   */
  virtual ~Scene() {}

  /**
   * @brief Generate a Layer and push it onto the Layer stack.
   * @details This function will generate a new reference to a Layer of type
   * <T> and push it onto the Scene's Layer stack. All Layers that are in the
   * Layer stack are active and receive callbacks from the Engine. A Referenced
   * Layer is returned that can be used and will be deleted when it goes out of
   * scope.
   *
   * @tparam <T> The type of Layer. <T> must be a Layer.
   * @tparam <Args> The type of the arguments for the Contructor of the Layer.
   * @param args The arguments for the Constructor of the Layer.
   * @return The Referenced Layer.
   * @see Layer
   */
  template <typename T, typename... Args>
  Referenced<T> gen_layer(Args... args) {
    // generate layer reference of type T and cast to base type
    Referenced<T> layr = gen_ref<T>(args...);
    layers_.push_back(layr);
    return layr;
  }
  /**
   * @brief Generate multiple Layers and push them onto the Layer stack.
   * @details This function will create multiple Layers of the same type and
   * push them all onto the Layer stack. A `std::vector` of Referenced Layers
   * is returned that can be used and will be deleted when it goes out of
   * scope.
   *
   * @tparam <T> The type of Layers to generate. <T> must be a Layer.
   * @tparam <Args> The type of the arguments for the Contructor of the Layer.
   * @param args The arguments for the Constructor of the Layer.
   * @param count The number of Layers to generate.
   * @see Layer
   */
  template <typename T, typename... Args>
  std::vector<Referenced<T>> gen_layers(uint32_t count, Args... args) {
    std::vector<Referenced<T>> layers{};
    for (uint32_t i = 0; i < count; i++) {
      // generate layer reference of type T and cast to base type
      Referenced<T> layr = gen_ref<T>(args...);
      layers_.push_back(layr);
      layers.push_back(layr);
    }
    return layers;
  }
  /**
   * @brief Get a pointer to the first Layer of type <T> in the Renderer's
   * Layer stack.
   *
   * @tparam <T> The type of Layer to get. <T> must be a Layer.
   * @return A pointer to the first Layer of type <T> in the Layer stack.
   * nullptr is returned if no Layer of type <T> exists on the Layer stack.
   * @see Layer
   */
  template <typename T> T *get_layer() {
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (auto layer = std::dynamic_pointer_cast<T>(*layr_it)) {
        return layer.get();
      }
    }
    return nullptr;
  }
  /**
   * @brief Get a `std::vector` of pointers to all of the Layers of type <T> in
   * the Renderer's Layer stack.
   *
   * @tparam <T> The type of Layer to get. <T> must be a Layer.
   * @return A `std::vector` of pointers to all of the Layers of type <T> in
   * the Layer stack. An empty vector is returned if no Layer of type <T>
   * exists on the Layer stack.
   * @see Layer
   */
  template <typename T> std::vector<T *> get_layers() {
    std::vector<T *> layers{};
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (auto layer = std::dynamic_pointer_cast<T>(*layr_it)) {
        layers.push_back(layer.get());
      }
    }
    return layers;
  }
  /**
   * @brief Push an existing Referenced Layer onto the Renderer's Layer stack.
   *
   * @param system A Referenced Layer.
   * @see Layer
   */
  template <typename T> void push_layer(Referenced<T> layer) {
    Referenced<Layer> layr = std::dynamic_pointer_cast<Layer>(layer);
    if (layr)
      layers_.push_back(layr);
  }
  /**
   * @brief Push a `std::vector` of existing Referenced Layers onto the
   * Renderer's Layer stack.
   *
   * @param systems A `std::vector` of existing Referenced Layers.
   * @see Layer
   */
  template <typename T> void push_layers(std::vector<Referenced<T>> layers) {
    for (size_t i = 0; i < layers.size(); i++) {
      Referenced<Layer> layr = std::dynamic_pointer_cast<Layer>(layers[i]);
      if (layr)
        layers_.push_back(layr);
    }
  }
  /**
   * @brief Remove the first Layer of type <T> from the Layer stack and return
   * the Referenced Layer.
   *
   * @tparam <T> The type of Layer to pull.
   * @param layer A pointer to the data of the Layer to pull.
   * @return The Referenced Layer that was pulled from the Layer stack.
   * @see Layer
   */
  template <typename T> Referenced<T> pull_layer(Layer *layer) {
    for (auto layr_it = layer_begin(); layr_it != layer_end(); layr_it++) {
      if (layer == (*layr_it).get()) {
        Referenced<T> pulled_layer =
            std::dynamic_pointer_cast<T>(*layr_it)(*layr_it) = nullptr;
        return pulled_layer;
      }
    }
    return nullptr;
  }

  /**
   * @brief Get a const iterator pointing to the begining of the Layer stack.
   *
   * @return A const iterator pointing to the begining of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::const_iterator layer_begin() const {
    return layers_.begin();
  }
  /**
   * @brief Get a const iterator pointing to the end of the Layer stack.
   *
   * @return A const iterator pointing to the end of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::const_iterator layer_end() const {
    return layers_.end();
  }
  /**
   * @brief Get a const reverse iterator pointing to the end of the Layer
   * stack.
   *
   * @return A const reverse iterator pointing to the end of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::const_reverse_iterator layer_rbegin() const {
    return layers_.rbegin();
  }
  /**
   * @brief Get a const reverse iterator pointing to the begining of the Layer
   * stack.
   *
   * @return A const reverse iterator pointing to the begining of the Layer
   * stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::const_reverse_iterator layer_rend() const {
    return layers_.rend();
  }
  /**
   * @brief Get a iterator pointing to the begining of the Layer stack.
   *
   * @return An iterator pointing to the begining of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::iterator layer_begin() {
    return layers_.begin();
  }
  /**
   * @brief Get a iterator pointing to the end of the Layer stack.
   *
   * @return An iterator pointing to the end of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::iterator layer_end() { return layers_.end(); }
  /**
   * @brief Get a reverse iterator pointing to the end of the Layer
   * stack.
   *
   * @return A reverse iterator pointing to the end of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::reverse_iterator layer_rbegin() {
    return layers_.rbegin();
  }
  /**
   * @brief Get a reverse iterator pointing to the begining of the Layer
   * stack.
   *
   * @return A reverse iterator pointing to the begining of the Layer stack.
   * @see Layer
   */
  std::vector<Referenced<Layer>>::reverse_iterator layer_rend() {
    return layers_.rend();
  }
  void remove_null_layers() {
    if (layer_pulled) {
      layer_pulled = false;
      layers_.erase(std::remove_if(layer_begin(), layer_end(),
                                   [](Referenced<Layer> layer) {
                                     return layer.get() == nullptr;
                                   }),
                    layer_end());
    }
  }

private:
  std::vector<Referenced<Layer>> layers_{}; /**< The Layer stack.*/
  bool layer_pulled = false;
};
} // namespace mare

#endif