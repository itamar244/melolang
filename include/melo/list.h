#pragma once

#include <cstdlib>
#include <atic/structs.h>
#include "zone.h"

namespace melo {

template<typename T>
class List : public ZoneObject {
  struct Node : public ZoneObject {
    T value;
    Node* next;

    Node(T value, Node* next = nullptr) : value(value), next(next) {}
  };

public:
  using value_type = T;

  class const_iterator {
  public:
    const_iterator(Node* node) : cur_(node) {}

    inline const_iterator& operator++() {
      cur_ = cur_->next;
      return *this;
    }

    inline const T& operator*() const { return iterator::cur_->value; }
    inline bool operator==(const const_iterator& rhs) {
      return cur_ == rhs.cur_;
    }
    inline bool operator!=(const const_iterator& rhs) {
      return cur_ != rhs.cur_;
    }

  protected:
    Node* cur_;
  };

  class iterator : public const_iterator {
  public:
    inline T& operator*() { return const_iterator::cur_->value; }
  };

  List(Zone* zone = nullptr) : zone_(zone) {}

  ~List() {
    if (!zone_) {
      while (first_) {
        auto next = first_->next;
        delete first_;
        first_ = next;
      }
    }
  }

  inline std::size_t size() const { return size_; }
  inline iterator begin() { return {first_}; }
  inline iterator end() { return {nullptr}; }
  inline const_iterator begin() const { return {first_}; }
  inline const_iterator end() const { return {nullptr}; }

  void push_back(T value) {
    if (!first_) {
      first_ = last_ = CreateNode(value);
    } else {
      last_ = last_->next = CreateNode(value);
    }
    ++size_;
  }

private:
  Zone* zone_;
  std::size_t size_ = 0;
  Node* first_ = nullptr;
  Node* last_ = nullptr;

  template<typename... Args>
  inline Node* CreateNode(Args... args) {
    return zone_
        ? new (zone_) Node(args...)
        : new Node(args...);
  }
};

} // namespace melo
