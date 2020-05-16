#ifndef RENDERPACK
#define RENDERPACK

// MARE
#include "mare/Mare.hpp"
#include "mare/Meshes.hpp"
#include "mare/Shader.hpp"
#include "mare/Components/Transform.hpp"

// Standard Library
#include <utility>
#include <vector>

namespace mare {
class RenderPack : virtual public Transform {
public:
  // Packet Template Functions--------------------------------
  // get first packet in the stack of a given type
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
  // get all packets of an interface type
  template <typename T, typename U> std::vector<std::pair<T *, U*>> get_packets() {
    std::vector<std::pair<T *, U*>> packets{};
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          packets.push_back(std::pair<T *, U *>{mesh.get(), material.get()});
        }
      }
    }
    return packets;
  }
  // push an existing packet onto the stack
  void push_packet(std::pair<Referenced<Mesh>, Referenced<Material>> packet) {
      packets_.push_back(packet);
  }
  // push a vector of existing packets onto the stack
  template <typename T> void push_packet(std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>> packets) {
    for (size_t i = 0; i < packets.size(); i++) {
        packets_.push_back(packets[i]);
    }
  }
  // remove first packet of a given type from the stack and return it
  template <typename T, typename U>
  std::pair<Referenced<T>, Referenced<U>> pull_packet() {
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          std::pair<Referenced<T>, Referenced<U>> pulled_packet =
              std::move(std::pair<T, U>{mesh, material});
          packets_.erase(pack_it);
          return pulled_packet;
        }
      }
    }
    return std::pair<Referenced<T>, Referenced<U>>{};
  }
  // remove all packets of a given type from the stack and return them as a
  // vector of pairs of references
  template <typename T, typename U>
  std::vector<std::pair<Referenced<T>, Referenced<U>>> pull_packets() {
    std::vector<std::pair<Referenced<T>, Referenced<U>>> packets{};
    for (auto pack_it = packets_begin(); pack_it != packets_end(); pack_it++) {
      if (auto mesh = std::dynamic_pointer_cast<T>((*pack_it).first)) {
        if (auto material = std::dynamic_pointer_cast<U>((*pack_it).second)) {
          std::pair<Referenced<T>, Referenced<U>> pulled_packet =
              std::move(std::pair<T, U>{mesh, material});
          packets_.erase(pack_it);
          packets.push_back(pulled_packet);
        }
      }
    }
    return packets;
  }
  //------------------------------------

  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::const_iterator
  packets_begin() const {
    return packets_.begin();
  }
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::const_iterator
  packets_end() const {
    return packets_.end();
  }
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::const_reverse_iterator
  packets_rbegin() const {
    return packets_.rbegin();
  }
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::const_reverse_iterator
  packets_rend() const {
    return packets_.rend();
  }
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::iterator
  packets_begin() {
    return packets_.begin();
  }
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>>::iterator
  packets_end() {
    return packets_.end();
  }
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::reverse_iterator
  packets_rbegin() {
    return packets_.rbegin();
  }
  std::vector<
      std::pair<Referenced<Mesh>, Referenced<Material>>>::reverse_iterator
  packets_rend() {
    return packets_.rend();
  }

private:
  std::vector<std::pair<Referenced<Mesh>, Referenced<Material>>> packets_;
};

} // namespace mare

#endif
