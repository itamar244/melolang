#include "module.h"
#include "values.h"
#include "utils.h"

namespace melo::evaluator {

Module::Module(ast::BlockPtr&& program)
		: program_(std::move(program))
		, main_walker_(GetMain(program_)->AsSection()) {
	for (auto& statement : program_->statements) {
		if (const auto& export_decl = statement->AsExport()) {
			exports_.insert({
				export_decl->id->name,
				ExpressionToValue(export_decl->value),
			});
		}
	}
}

}  // namespace melo::evaluator
