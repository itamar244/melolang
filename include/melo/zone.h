#pragma once

#include <cstdlib>
#include <atic/stack.h>

namespace melo {

class ZoneObject;

class Zone {
public:
  ~Zone();
  void* New(std::size_t size);

  inline void Add(ZoneObject* object) {
    ptrs_.push(object);
  }

private:
  atic::Stack<ZoneObject*> ptrs_;
};

class ZoneObject {
public:
  virtual ~ZoneObject() {}

  inline void* operator new(std::size_t size, Zone* zone) {
    return zone->New(size);
  }
  inline void* operator new(std::size_t size) {
    return ::operator new(size);
  }
};

} // namespace melo
