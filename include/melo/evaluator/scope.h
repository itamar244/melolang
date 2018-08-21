#pragma once

#include <map>
#include <string>
#include "melo/evaluator/values.h"
#include "melo/zone.h"

namespace melo::evaluator {

class Scope {
public:
  using Data = std::map<std::string, const Value*>;

  Scope(const Scope* next = nullptr) : next_(next) {}

  inline Zone* zone() { return &zone_; }

  const Value* Get(const std::string& name) const;
  void Set(const std::string& name, const Value* value);

private:
  Zone zone_;
  Data data_;
  const Scope* next_;
};

} // namespace melo::evaluator
