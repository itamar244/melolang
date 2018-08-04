#include "parser/parser.h"
#include <exception>
#include <utility>
#include "melo/ast.h"

namespace melo::parser {

using namespace ast;

Parser::Parser(const State::Ptr& state)
		: state_(state)
		, t_(Tokenizer(state)) {}

BlockPtr Parser::Parse() {
	t_.Next();
	return ParseBlock(true);
}

BlockPtr Parser::ParseBlock(bool top_level) {
	std::vector<StatementPtr> statements;

	while (!t_.Match(top_level ? tt::eof : tt::braceR)) {
		auto& statement = statements.emplace_back(ParseStatement(top_level));
		if (!statement->IsFunctionDeclaration()) {
			t_.Expect(tt::semi);
			t_.Next();
		}
	}
	// skipping the closing '}'
	if (!top_level) t_.Next();

	return std::make_unique<Block>(std::move(statements));
}

// ------------------------------- expressions ---------------------------------
ExpressionPtr Parser::ParseExpression() {
	switch (state_->type) {
		case tt::bracketL:
			return ParseSection();
		case tt::num:
			return ParseNumber();
		default:
			throw std::logic_error(
					"unsupported token for expression: " +
					TokenTypeToString(state_->type));
	}
}

SectionPtr Parser::ParseSection() {
	std::vector<PhraseLiteralPtr> sections;

	t_.Next();
	while (!t_.Eat(tt::bracketR)) {
		sections.push_back(ParsePhraseLiteral());
		if (!t_.Eat(tt::comma) && !t_.Match(tt::bracketR)) {
			throw std::logic_error("unfinished section");
		}
	}

	return std::make_unique<Section>(std::move(sections));
}

PhraseLiteralPtr Parser::ParsePhraseLiteral() {
	t_.Expect(tt::parenL);
	t_.Next();
	t_.Expect(tt::num);
	auto length = ParseNumber();
	std::vector<IdentifierPtr> notes;

	while (!t_.Eat(tt::parenR)) {
		notes.push_back(ParseIdentifier());
	}

	return std::make_unique<PhraseLiteral>(std::move(length), std::move(notes));
}

IdentifierPtr Parser::ParseIdentifier() {
	auto id = std::make_unique<Identifier>(state_->value);
	t_.Next();
	return std::move(id);
}

NumericLiteralPtr Parser::ParseNumber() {
	auto num = std::make_unique<NumericLiteral>(state_->value);
	t_.Next();
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
	t_.Next();
	t_.Expect(tt::name);

	return std::make_unique<Export>(ParseIdentifier(), ParseExpression());
}

FunctionDeclarationPtr Parser::ParseFunctionDeclaration() {
	t_.Next();
	t_.Expect(tt::name);
	auto id = ParseIdentifier();
	t_.Expect(tt::braceL);
	t_.Next();

	return std::make_unique<FunctionDeclaration>(
			std::move(id),
			ParseBlock(false));
}

ReturnPtr Parser::ParseReturn() {
	t_.Next();
	return std::make_unique<Return>(ParseExpression());
}

} // namespace melo::parser
