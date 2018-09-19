#pragma once

#include <memory>
#include <string>
#include "melo/ast.h"
#include "melo/zone.h"
#include "parser/state.h"
#include "parser/tokenizer.h"

namespace melo::parser {

class Parser : private Tokenizer, private ast::NodeFactory {
public:
	Parser(State::Ptr state, Zone* zone)
			: Tokenizer(state), NodeFactory(zone) {}

	ast::Block* Parse();

private:
	ast::Block* ParseBlock(bool top_level);

	// expressions
	ast::Expression* ParseExpression();
	ast::ListLiteral* ParseListLiteral();
	ast::PhraseLiteral* ParsePhraseLiteral();
	ast::Identifier* ParseIdentifier();
	ast::Expression* ParseMaybeFunctionCall();
	ast::NumericLiteral* ParseNumber();
	ast::Spread* ParseSpread();

	// statements
	ast::Statement* ParseStatement(bool top_level);
	ast::Export* ParseExport();
	ast::VariableDeclaration* ParseVariableDeclaration();
	ast::FunctionDeclaration* ParseFunctionDeclaration();
	ast::Return* ParseReturn();
};

} // namespace melo::parser
