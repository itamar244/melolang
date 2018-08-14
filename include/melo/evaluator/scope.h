#pragma once

#include <map>
#include <memory>
#include <string>
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class Scope {
public:
  using Data = std::map<std::string, const Value*>;

  Scope(const Scope* next = nullptr) : next_(next) {}

  ~Scope();

  const Value* Get(const std::string& name) const;
  void Set(const std::string& name, const Value* value);

private:
  Data data_;
  const Scope* next_;
};

} // namespace melo::evaluator
