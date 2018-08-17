#pragma once

#include <cstdlib>
#include <list>

namespace melo {

class Zone {
public:
  ~Zone();
  void* New(std::size_t size);

private:
  std::list<void*> ptrs_;
};

} // namespace melo
