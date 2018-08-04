#pragma once

#include <memory>
#include <string>
#include "../ast.h"
#include "state.h"
#include "tokenizer.h"

namespace melo::parser {

class Parser {
public:
	Parser(const State::Ptr& state);
	ast::BlockPtr Parse();

private:
	State::Ptr state_;
	Tokenizer t_;

	ast::BlockPtr ParseBlock(bool top_level);

	// expressions
	ast::ExpressionPtr ParseExpression();
	ast::IdentifierPtr ParseIdentifier();
	ast::NumericLiteralPtr ParseNumber();
	ast::SectionPtr ParseSection();
	ast::PhraseLiteralPtr ParsePhraseLiteral();

	// statements
	ast::StatementPtr ParseStatement(bool top_level);
	ast::ExportPtr ParseExport();
	ast::FunctionDeclarationPtr ParseFunctionDeclaration();
	ast::ReturnPtr ParseReturn();
};

} // namespace melo::parser
