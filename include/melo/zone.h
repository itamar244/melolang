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

class ZoneObject {
public:
  inline void* operator new(std::size_t size, Zone* zone) {
    return zone->New(size);
  }
  inline void* operator new(std::size_t size) {
    return ::operator new(size);
  }
};

} // namespace melo
