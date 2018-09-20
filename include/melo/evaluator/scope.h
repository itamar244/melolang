#pragma once

#include <map>
#include <memory>
#include <string>
#include "melo/evaluator/values.h"
#include "melo/zone.h"

namespace melo::evaluator {

class Scope {
public:
  using Value = std::shared_ptr<const melo::evaluator::Value>;
  using Data = std::map<std::string, Value>;

  Scope(const Scope* next = nullptr) : next_(next) {}

  Value Get(const std::string& name);
  void Set(const std::string& name, Value value);

#define VALUES_CREATOR(NAME)                                                   \
  template<typename... Args>                                                   \
  static inline std::shared_ptr<NAME> New##NAME(const Args&... args) {         \
    return std::make_shared<NAME>(args...);                                    \
  }

  MELO_EVALUATOR_VALUE_TYPES(VALUES_CREATOR)
#undef VALUES_CREATOR

private:
  Data data_;
  const Scope* next_;
};

} // namespace melo::evaluator
