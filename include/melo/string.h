#pragma once

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <string>
#include "zone.h"

namespace melo {

class String : public ZoneObject {
public:
  // String(const char*& buffer);
  // String(const char*&& str);
  String(const std::string&);
  String(const String&);
  String(String&&);
  ~String() override;

  inline const char* data() const { return data_; }
  inline std::size_t size() const { return size_; }

  inline const char& at(std::size_t i) const {
    assert(i < size_);
    return data_[i];
  }

  inline operator const char*() const {
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
  std::size_t size_;
  const char* data_;

  // friend class String operator+(const char* lhs, const String& rhs);
  //
  // String() : data_(nullptr), size_(0) {}
};

// String operator+(const char* lhs, const String& rhs);

} // namespace melo
