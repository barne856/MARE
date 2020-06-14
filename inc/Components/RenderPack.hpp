#ifndef RENDERPACK
#define RENDERPACK

// MARE
#include "Entity.hpp"
#include "Mare.hpp"
#include "Meshes.hpp"
#include "Shader.hpp"

// Standard Library
#include <utility>
#include <vector>

namespace mare {
/**
 * @brief A Component that renders *Render Packets*.
 * @details A RenderPack contains pairs of Referenced Mesh-Material pairs called
 * *Render Packets* which are rendered each frame. Any Entity that inherits from
 * RenderPack can push Render Packets to the Component which will render them.
 */
class RenderPack : virtual public Entity {
public:
  /**
   * @brief Get a pointer to the first Packet of type <T,U> in the Entity's
   * Packet stack.
   *
   * @tparam <T> The type of Mesh to get. <T> must be a Mesh.
   * @tparam <U> The type of Material to get. <U> must be a Material.
   * @return A pointer to the first Packet of type <T,U> in the Packet stack.
   * An empty pair is returned if no Packet of type <T,U> exists on the Packet
   * stack.
   * @see Mesh
   * @see Material
   */
  template <typename T, typename U> std::pair<T *, U *> get_packet() {
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          return std::pair<T *, U *>{mesh.get(), material.get()};
        }
      }
    }
    return std::pair<T *, U *>{};
  }
  /**
   * @brief Get a std::vector of all the Packets of type <T,U> on the Packet
   * stack.
   *
   * @tparam <T> The type of Mesh to search for.
   * @tparam <U> The type of Material to search for.
   * @return A `std::vector` of the mesh-material pairs of type <T,U> found in
   * the Packet stack.
   */
  template <typename T, typename U>
  std::vector<std::pair<T *, U *>> get_packets() {
    std::vector<std::pair<T *, U *>> packets{};
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          packets.push_back(std::pair<T *, U *>{mesh.get(), material.get()});
        }
      }
    }
    return packets;
  }
  /**
   * @brief Push an exisitng Packet on the stack.
   *
   * @param packet The Packet to push.
   */
  void push_packet(std::pair<Referenced<Mesh>, Referenced<Material>> packet) {
    packets_.push_back(packet);
  }
  /**
   * @brief Push a `std::vector` of existing Packets onto the Packet stack.
   *
   * @param packets the Packets to push.
   */
  void push_packets(
      std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>> packets) {
    for (size_t i = 0; i < packets.size(); i++) {
      packets_.push_back(packets[i]);
    }
  }
  /**
   * @brief Remove the first Packet of type <T,U> from the Packet stack and
   * return the Packet.
   *
   * @tparam <T> The Mesh type to search for.
   * @tparam <U> The Material type to search for.
   * @return The pulled Packet.
   */
  template <typename T, typename U>
  std::pair<Referenced<T>, Referenced<U>> pull_packet() {
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          std::pair<Referenced<T>, Referenced<U>> pulled_packet =
              std::pair<Referenced<T>, Referenced<U>>{mesh, material};
          packets_.erase(pack_it);
          return pulled_packet;
        }
      }
    }
    return std::pair<Referenced<T>, Referenced<U>>{};
  }
  /**
   * @brief Remove all of the Packets of type <T,U> from the Packet stack and
   * return a `std::vector` of the Packets.
   *
   * @tparam <T> The type of Mesh to search for.
   * @tparam <U> The type of Material to search for.
   * @return A `std::vector` of the pulled Packets.
   */
  template <typename T, typename U>
  std::vector<std::pair<Referenced<T>, Referenced<U>>> pull_packets() {
    std::vector<int> rm{};
    int i = 0;
    std::vector<std::pair<Referenced<T>, Referenced<U>>> packets{};
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          std::pair<Referenced<T>, Referenced<U>> pulled_packet =
              std::pair<Referenced<T>, Referenced<U>>{mesh, material};
          packets.push_back(pulled_packet);
          rm.push_back(i);
          i++;
        }
      }
    }
    // remove all pulled packets
    size_t rm_index = 0;
    packets_.erase(std::remove_if(std::begin(packets_), std::end(packets_),
                                  [&](auto &elem) {
                                    if (rm.size() != rm_index &&
                                        &elem - &packets_[0] == rm[rm_index]) {
                                      rm_index++;
                                      return true;
                                    }
                                    return false;
                                  }),
                   std::end(packets_));
    return packets;
  }

  /**
   * @brief Get a const iterator pointing to the begining of the Packet stack.
   *
   * @return A const iterator pointing to the begining of the Packet stack.
   */
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::const_iterator
  packets_begin() const {
    return packets_.begin();
  }
  /**
   * @brief Get a const iterator pointing to the end of the Packet stack.
   *
   * @return A const iterator pointing to the end of the Packet stack.
   */
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::const_iterator
  packets_end() const {
    return packets_.end();
  }
  /**
   * @brief Get a const reverse iterator pointing to the end of the Packet
   * stack.
   *
   * @return A const reverse iterator pointing to the end of the Packet stack.
   */
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::const_reverse_iterator
  packets_rbegin() const {
    return packets_.rbegin();
  }
  /**
   * @brief Get a const reverse iterator pointing to the begining of the Packet
   * stack.
   *
   * @return A const reverse iterator pointing to the begining of the Packet
   * stack.
   */
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::const_reverse_iterator
  packets_rend() const {
    return packets_.rend();
  }
  /**
   * @brief Get a iterator pointing to the begining of the Packet stack.
   *
   * @return An iterator pointing to the begining of the Packet stack.
   */
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::iterator
  packets_begin() {
    return packets_.begin();
  }
  /**
   * @brief Get a iterator pointing to the end of the Packet stack.
   *
   * @return An iterator pointing to the end of the Packet stack.
   */
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::iterator
  packets_end() {
    return packets_.end();
  }
  /**
   * @brief Get a reverse iterator pointing to the end of the Packet
   * stack.
   *
   * @return A reverse iterator pointing to the end of the Packet stack.
   */
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::reverse_iterator
  packets_rbegin() {
    return packets_.rbegin();
  }
  /**
   * @brief Get a reverse iterator pointing to the begining of the Packet
   * stack.
   *
   * @return A reverse iterator pointing to the begining of the Packet stack.
   */
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::reverse_iterator
  packets_rend() {
    return packets_.rend();
  }

private:
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>
      packets_; /**< The container of the render packets.*/
};

} // namespace mare

#endif
