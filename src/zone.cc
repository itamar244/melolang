#include "melo/zone.h"
#include <cstdlib>

namespace melo {

Zone::~Zone() {
  while (!ptrs_.empty()) {
    delete ptrs_.pop();
  }
}

void* Zone::New(std::size_t size) {
  void* ptr = ::operator new(size);
  ptrs_.push_front(reinterpret_cast<ZoneObject*>(ptr));
  return ptr;
}

} // namespace melo
