#pragma once

#include <cstdlib>
#include <atic/structs.h>

namespace melo {

class Zone {
public:
  ~Zone();
  void* New(std::size_t size);

  template<typename T>
  inline T* NewArray(std::size_t size) {
    return reinterpret_cast<T*>(New(size * sizeof(T)));
  }

private:
  atic::Stack<void*> ptrs_;
};

} // namespace melo
