#pragma once

#include <istream>
#include "ast.h"
#include "evaluator/module.h"

namespace melo {

ast::BlockPtr Parse(std::istream& stream);
evaluator::Module CreateModule(std::istream& stream);

} // namespace melo
