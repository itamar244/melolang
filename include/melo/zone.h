#pragma once

#include <cstdlib>
#include <atic/structs.h>

namespace melo {

class Zone {
public:
  ~Zone();
  void* New(std::size_t size);

private:
  atic::Stack<void*> ptrs_;
};

} // namespace melo
