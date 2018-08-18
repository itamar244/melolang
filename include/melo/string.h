#pragma once

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <string>
#include "zone.h"

namespace melo {

class String {
public:
  String(Zone*, const std::string&);

  const char* data() const { return data_; }
  std::size_t size() const { return size_; }

  inline const char& at(std::size_t i) const {
    assert(i < size_);
    return data_[i];
  }

  operator const char*() const {
    return data_;
  }

  inline const char& operator[](std::size_t i) const {
    return data_[i];
  }

  inline bool operator==(const char* rhs) {
    return std::strcmp(data_, rhs) == 0;
  }

  inline bool operator==(const String& rhs) {
    return *this == rhs.data_;
  }

	String operator+(const String& rhs) const;
	String operator+(const char* rhs) const;

private:
  const bool used_zone_;
  std::size_t size_;
  const char* data_;

} // namespace melo
