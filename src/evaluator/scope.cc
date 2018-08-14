#include "melo/evaluator/scope.h"

namespace melo::evaluator {

Scope::~Scope() {
  for (const auto& pair : data_) {
    delete pair.second;
  }
}

const Value* Scope::Get(const std::string& name) const {
  const Scope* cur = this;
  const Value* value = nullptr;

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

void Scope::Set(const std::string& name, const Value* value) {
  auto it = data_.find(name);
  if (it != data_.end()) {
    delete it->second;
    it->second = value;
  } else {
    data_.insert({ name, value });
  }
}

} // namespace melo::evaluator
