#include "melo/zone.h"
#include <cstdlib>

namespace melo {

Zone::~Zone() {
  while (!ptrs_.empty()) {
    std::free(ptrs_.pop());
  }
}

void* Zone::New(std::size_t size) {
  void* ptr = std::malloc(size);
  ptrs_.push_front(ptr);
  return ptr;
}

} // namespace melo
