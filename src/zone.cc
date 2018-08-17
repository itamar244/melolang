#include "melo/zone.h"
#include <cstdlib>

namespace melo {

Zone::~Zone() {
  for (auto ptr : ptrs_) {
    std::free(ptr);
  }
}

void* Zone::New(std::size_t size) {
  void* ptr = std::malloc(size);
  ptrs_.push_back(ptr);
  return ptr;
}

} // namespace melo
