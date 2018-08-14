#pragma once

#include <map>
#include <string>
#include "melo/evaluator/values.h"

namespace melo::evaluator {

using Scope = std::map<std::string, const Value*>;

class LinkedScope {
public:
  LinkedScope(Scope scope, const LinkedScope* next = nullptr)
      : scope_(scope), next_(next) {}

  inline Scope& scope() { return scope_; }

  inline void Set(const std::string& name, const Value* value) {
    scope_[name] = value;
  }
  const Value* Get(const std::string& name) const;

private:
  Scope scope_;
  const LinkedScope* next_;
};

} // namespace melo::evaluator
