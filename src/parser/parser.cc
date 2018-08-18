#include "parser/parser.h"
#include <exception>
#include <utility>
#include "melo/ast.h"

namespace melo::parser {

using namespace ast;

Block* Parser::Parse() {
	Next();
	return ParseBlock(true);
}

Block* Parser::ParseBlock(bool top_level) {
	std::vector<const Statement*> statements;

	while (!Match(top_level ? tt::eof : tt::braceR)) {
		auto& statement = statements.emplace_back(ParseStatement(top_level));
		if (!statement->IsFunctionDeclaration()) {
			ExpectAndNext(tt::semi);
		}
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
		case tt::parenL:
			return ParsePhraseLiteral();
		case tt::num:
			return ParseNumber();
		case tt::name:
			return ParseMaybeFunctionCall();
		default:
			throw std::logic_error(
					"unsupported token for expression: " +
					TokenTypeToString(state_->type));
	}
}

ListLiteral* Parser::ParseListLiteral() {
	std::vector<Expression*> elements;

	Next();
	while (!Eat(tt::bracketR)) {
		elements.push_back(ParseExpression());
		if (!Eat(tt::comma) && !Match(tt::bracketR)) {
			throw std::logic_error("unfinished section");
		}
	}

	return NewListLiteral(elements);
}

PhraseLiteral* Parser::ParsePhraseLiteral() {
	ExpectAndNext(tt::parenL);
	Expect(tt::num);
	auto length = ParseNumber();
	std::vector<Identifier*> notes;

	while (!Eat(tt::parenR)) {
		notes.push_back(ParseIdentifier());
	}

	return NewPhraseLiteral(length, notes);
}


Expression* Parser::ParseMaybeFunctionCall() {
	auto id = ParseIdentifier();
	if (!Eat(tt::parenL)) {
		return id;
	}
	std::vector<Expression*> args;
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

// ------------------------------- statements ----------------------------------
Statement* Parser::ParseStatement(bool top_level) {
	switch (state_->type) {
		// case tt::_var:
		// 	return ParseVarDeclaration();
		// 	break;
		case tt::_export:
			if (!top_level) {
				throw std::logic_error("export must be at top level");
			}
			return ParseExport();
			break;
		case tt::_func:
			if (!top_level) {
				throw std::logic_error("function must be at top level");
			}
			return ParseFunctionDeclaration();
			break;
		case tt::_return:
			if (top_level) {
				throw std::logic_error("return must be inside a function");
			}
			return ParseReturn();
		default:
			throw std::logic_error(
					"unsupported token for statement: " +
					TokenTypeToString(state_->type));
	}
}

Export* Parser::ParseExport() {
	Next();
	Expect(tt::name);

	return NewExport(ParseIdentifier(), ParseExpression());
}

FunctionDeclaration* Parser::ParseFunctionDeclaration() {
	Next();
	Expect(tt::name);
	auto id = ParseIdentifier();

	ExpectAndNext(tt::parenL);
	std::vector<Identifier*> params;

	while (!Eat(tt::parenR)) {
		Expect(tt::name);
		params.push_back(ParseIdentifier());
		if (!Eat(tt::comma) && !Match(tt::parenR)) {
			throw std::logic_error("unfinished section");
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
