#include "parser/parser.h"
#include <exception>
#include <utility>
#include <atic/list.h>
#include "melo/ast.h"

namespace melo::parser {

using namespace ast;

Block* Parser::Parse() {
	Next();
	return ParseBlock(true);
}

Block* Parser::ParseBlock(bool top_level) {
	atic::List<Statement*> statements;

	while (!Match(top_level ? tt::eof : tt::braceR)) {
		auto statement = ParseStatement(top_level);
		statements.push_back(statement);
		// if (!statement->IsFunctionDeclaration()) {
		// 	ExpectAndNext(tt::semi);
		// }
	}
	// skipping the closing '}'
	if (!top_level) Next();

	return NewBlock(statements);
}

// ------------------------------- expressions ---------------------------------
Expression* Parser::ParseExpression() {
	switch (state_->type) {
		case tt::bracketL:
			return ParseListLiteral();
		case tt::lt:
			return ParsePhraseLiteral();
		case tt::num:
			return ParseNumber();
		case tt::name:
			return ParseMaybeFunctionCall();
		case tt::spread:
			return ParseSpread();
		default:
			throw std::logic_error(
					"unsupported token for expression: " +
					TokenTypeToString(state_->type));
	}
}

ListLiteral* Parser::ParseListLiteral() {
	atic::List<Expression*> elements;

	Next();
	while (!Eat(tt::bracketR)) {
		elements.push_back(ParseExpression());
		if (Match(tt::eof)) {
			throw std::logic_error("unfinished list");
		}
	}

	return NewListLiteral(elements);
}

PhraseLiteral* Parser::ParsePhraseLiteral() {
	Next();
	Expect(tt::num);
	auto length = ParseNumber();
	atic::List<Identifier*> notes;

	while (!Eat(tt::gt)) {
		notes.push_back(ParseIdentifier());
	}

	return NewPhraseLiteral(length, notes);
}


Expression* Parser::ParseMaybeFunctionCall() {
	auto id = ParseIdentifier();
	if (!Eat(tt::parenL)) {
		return id;
	}
	atic::List<Expression*> args;
	// FIXME: when arguments are supported add params parsing
	ExpectAndNext(tt::parenR);
	return NewFunctionCall(id, args);
}

Identifier* Parser::ParseIdentifier() {
	auto id = NewIdentifier(state_->value);
	Next();
	return id;
}

NumericLiteral* Parser::ParseNumber() {
	auto num = NewNumericLiteral(state_->value);
	Next();
	return num;
}

Spread* Parser::ParseSpread() {
	Next();
	return NewSpread(ParseExpression());
}

// ------------------------------- statements ----------------------------------
Statement* Parser::ParseStatement(bool top_level) {
	switch (state_->type) {
		case tt::_export:
			if (!top_level) {
				throw std::logic_error("export must be at top level");
			}
			return ParseExport();
		case tt::_let:
			return ParseVariableDeclaration();
		case tt::_func:
			if (!top_level) {
				throw std::logic_error("function must be at top level");
			}
			return ParseFunctionDeclaration();
		case tt::_return:
			if (top_level) {
				throw std::logic_error("return must be inside a function");
			}
			return ParseReturn();
		default:
			throw std::logic_error(
					"unexpected token for statemenet: " +
					TokenTypeToString(state_->type));
	}
}

Export* Parser::ParseExport() {
	Next();
	Expect(tt::name);

	return NewExport(ParseIdentifier(), ParseExpression());
}

ast::VariableDeclaration* Parser::ParseVariableDeclaration() {
	Next();
	Expect(tt::name);
	return NewVariableDeclaration(ParseIdentifier(), ParseExpression());
}

FunctionDeclaration* Parser::ParseFunctionDeclaration() {
	Next();
	Expect(tt::name);
	auto id = ParseIdentifier();

	ExpectAndNext(tt::parenL);
	atic::List<Identifier*> params;

	while (!Eat(tt::parenR)) {
		Expect(tt::name);
		params.push_back(ParseIdentifier());
		if (!Eat(tt::comma) && !Match(tt::parenR)) {
			throw std::logic_error("unfinished arguments");
		}
	}

	ExpectAndNext(tt::braceL);

	return NewFunctionDeclaration(id, params, ParseBlock(false));
}

Return* Parser::ParseReturn() {
	Next();
	return NewReturn(ParseExpression());
}

} // namespace melo::parser
