#pragma once

#include <cstring>
#include <map>
#include <memory>
#include <string>
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class Scope {
private:
  struct CmpStr {
    bool operator()(const char* lhs, const char* rhs) const {
      return std::strcmp(lhs, rhs) < 0;
    }
  };

public:
  using Data = std::map<const char*, const Value*, CmpStr>;

  Scope(const Scope* next = nullptr) : next_(next) {}

  ~Scope();

  const Value* Get(const char* name) const;
  void Set(const char* name, const Value* value);

private:
  Data data_;
  const Scope* next_;
};

} // namespace melo::evaluator
