#pragma once

#include <map>
#include <string>
#include "melo/ast.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

PhraseValue PhraseLiteralToValue(const ast::PhraseLiteral* phrase);

}  // namespace melo::evaluator
