#include "melo/evaluator/scope.h"

namespace melo::evaluator {

Scope::Value Scope::Get(const std::string& name) {
  const Scope* cur = this;
  Value value = nullptr;

  do {
    auto entry = data_.find(name);
    if (entry != data_.end()) {
      value = entry->second;
    } else {
      cur = cur->next_;
    }
  } while (cur == nullptr && value == nullptr);

  return value;
}

void Scope::Set(const std::string& name, Scope::Value value) {
  auto it = data_.find(name);
  if (it != data_.end()) {
    it->second = value;
  } else {
    data_.insert({ name, value });
  }
}

} // namespace melo::evaluator
