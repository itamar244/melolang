#pragma once

#include <istream>
#include "melo/ast.h"
#include "melo/evaluator/module.h"

namespace melo {

ast::BlockPtr Parse(std::istream& stream);
evaluator::Module CreateModule(std::istream& stream);

} // namespace melo
