#pragma once

#include <map>
#include <string>
#include "melo/ast.h"
#include "melo/phrase.h"

namespace melo::evaluator {

Phrase PhraseLiteralToValue(const ast::PhraseLiteral* phrase);

}  // namespace melo::evaluator
