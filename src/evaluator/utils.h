#pragma once

#include <map>
#include <string>
#include "melo/ast.h"
#include "melo/phrase.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

using Scope = std::map<std::string, ast::Expression>;

Phrase PhraseNodeToPhrase(const ast::PhraseLiteral* phrase);

}  // namespace melo::evaluator
