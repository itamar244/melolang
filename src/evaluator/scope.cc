#include "melo/evaluator/scope.h"

namespace melo::evaluator {

const Value* LinkedScope::Get(const std::string& name) const {
  const LinkedScope* cur = this;
  const Value* value = nullptr;

  do {
    auto entry = scope_.find(name);
    if (entry != scope_.end()) {
      value = entry->second;
    } else {
      cur = cur->next_;
    }
  } while (cur != nullptr && value != nullptr);

  return value;
}

} // namespace melo::evaluator
