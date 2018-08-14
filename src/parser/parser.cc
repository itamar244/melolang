#include "parser/parser.h"
#include <exception>
#include <utility>
#include "melo/ast.h"

namespace melo::parser {

using namespace ast;

BlockPtr Parser::Parse() {
	Next();
	return ParseBlock(true);
}

BlockPtr Parser::ParseBlock(bool top_level) {
	std::vector<StatementPtr> statements;

	while (!Match(top_level ? tt::eof : tt::braceR)) {
		auto& statement = statements.emplace_back(ParseStatement(top_level));
		if (!statement->IsFunctionDeclaration()) {
			ExpectAndNext(tt::semi);
		}
	}
	// skipping the closing '}'
	if (!top_level) Next();

	return std::make_unique<Block>(std::move(statements));
}

// ------------------------------- expressions ---------------------------------
ExpressionPtr Parser::ParseExpression() {
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

ListLiteralPtr Parser::ParseListLiteral() {
	std::vector<ExpressionPtr> sections;

	Next();
	while (!Eat(tt::bracketR)) {
		sections.push_back(ParseExpression());
		if (!Eat(tt::comma) && !Match(tt::bracketR)) {
			throw std::logic_error("unfinished section");
		}
	}

	return std::make_unique<ListLiteral>(std::move(sections));
}

PhraseLiteralPtr Parser::ParsePhraseLiteral() {
	ExpectAndNext(tt::parenL);
	Expect(tt::num);
	auto length = ParseNumber();
	std::vector<IdentifierPtr> notes;

	while (!Eat(tt::parenR)) {
		notes.push_back(ParseIdentifier());
	}

	return std::make_unique<PhraseLiteral>(std::move(length), std::move(notes));
}


ast::ExpressionPtr Parser::ParseMaybeFunctionCall() {
	auto id = ParseIdentifier();
	if (!Eat(tt::parenL)) {
		return id;
	}
	std::vector<ExpressionPtr> args;
	// FIXME: when arguments are supported add params parsing
	ExpectAndNext(tt::parenR);
	return std::make_unique<FunctionCall>(std::move(id), std::move(args));
}

IdentifierPtr Parser::ParseIdentifier() {
	auto id = std::make_unique<Identifier>(state_->value);
	Next();
	return std::move(id);
}

NumericLiteralPtr Parser::ParseNumber() {
	auto num = std::make_unique<NumericLiteral>(state_->value);
	Next();
	return std::move(num);
}

// ------------------------------- statements ----------------------------------
StatementPtr Parser::ParseStatement(bool top_level) {
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

ExportPtr Parser::ParseExport() {
	Next();
	Expect(tt::name);

	return std::make_unique<Export>(ParseIdentifier(), ParseExpression());
}

FunctionDeclarationPtr Parser::ParseFunctionDeclaration() {
	Next();
	Expect(tt::name);
	auto id = ParseIdentifier();

	ExpectAndNext(tt::parenL);
	std::vector<IdentifierPtr> params;

	while (!Eat(tt::parenR)) {
		Expect(tt::name);
		params.push_back(ParseIdentifier());
		if (!Eat(tt::comma) && !Match(tt::parenR)) {
			throw std::logic_error("unfinished section");
		}
	}

	ExpectAndNext(tt::braceL);

	return std::make_unique<FunctionDeclaration>(
			std::move(id),
			std::move(params),
			ParseBlock(false));
}

ReturnPtr Parser::ParseReturn() {
	Next();
	return std::make_unique<Return>(ParseExpression());
}

} // namespace melo::parser
