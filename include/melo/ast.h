#pragma once

#include <cctype>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace melo::ast {

#define MELO_WRAPPERS_NODE_TYPES(V)                                            \
	V(Block)

#define MELO_STATEMENT_NODE_TYPES(V)                                           \
	V(FunctionDeclaration)                                                       \
	V(Export)                                                                    \
	V(Return)

#define MELO_EXPRESSION_NODE_TYPES(V)                                          \
	V(Section)                                                                   \
	V(PhraseLiteral)                                                             \
	V(Identifier)                                                                \
	V(FunctionCall)                                                              \
	V(NoteLiteral)                                                               \
	V(NumericLiteral)

#define MELO_AST_NODE_TYPES(V)                                                 \
	MELO_WRAPPERS_NODE_TYPES(V)                                                  \
	MELO_STATEMENT_NODE_TYPES(V)                                                 \
	MELO_EXPRESSION_NODE_TYPES(V)

enum NodeType : uint8_t {
#define DECLARE_TYPE_ENUM(TYPE) k##TYPE,
	MELO_AST_NODE_TYPES(DECLARE_TYPE_ENUM)
#undef DECLARE_TYPE_ENUM
};

#define FR_DECL(NAME)                                                          \
	struct NAME;                                                                 \
	using NAME##Ptr = std::unique_ptr<NAME>;

	MELO_AST_NODE_TYPES(FR_DECL)
	FR_DECL(AstNode)
	FR_DECL(Expression)
	FR_DECL(Statement)
#undef FR_DECL

struct AstNode {
	const NodeType type;

#define V(NAME)                                                                \
	inline NAME* As##NAME() {                                                    \
		return type == k##NAME ? reinterpret_cast<NAME*>(this) : nullptr;          \
	}                                                                            \
	inline bool Is##NAME() { return As##NAME() != nullptr; }

	MELO_AST_NODE_TYPES(V)
#undef V

protected:
	AstNode(NodeType type) : type(type) {}
};

struct Statement : public AstNode {
	Statement(NodeType type) : AstNode(type) {}
};

struct Expression : public AstNode {
	Expression(NodeType type) : AstNode(type) {}
};

struct Identifier : public Expression {
	const std::string name;

	Identifier(const std::string& name) : Expression(kIdentifier), name(name) {}
};

struct PhraseLiteral : public Expression {
	const NumericLiteralPtr length;
	const std::vector<IdentifierPtr> notes;

	PhraseLiteral(NumericLiteralPtr length, std::vector<IdentifierPtr> notes)
			: Expression(kPhraseLiteral)
			, length(std::move(length))
			, notes(std::move(notes)) {}
};

struct Section : public Expression {
	const std::vector<PhraseLiteralPtr> phrases;

	Section(std::vector<PhraseLiteralPtr> phrases)
	: Expression(kSection), phrases(std::move(phrases)) {}
};

struct NoteLiteral : public Expression {
	const std::string name;
	const uint8_t octave;

	NoteLiteral(const std::string& name, uint8_t octave)
			: Expression(kNoteLiteral), name(name), octave(octave) {}
};

struct NumericLiteral : public Expression {
	const std::string value;

	NumericLiteral(const std::string& value)
			: Expression(kNumericLiteral), value(value) {}
};

struct FunctionCall : public Expression {
	const IdentifierPtr id;
	const std::vector<ExpressionPtr> args;

	FunctionCall(IdentifierPtr id, std::vector<ExpressionPtr> args)
			: Expression(kFunctionCall)
			, id(std::move(id))
			, args(std::move(args)) {}
};

struct Block : public Statement {
	const std::vector<StatementPtr> statements;

	Block(std::vector<StatementPtr> statements)
			: Statement(kBlock), statements(std::move(statements)) {}
};

struct Export : public Statement {
	const IdentifierPtr id;
	// FIX: make const
	ExpressionPtr value;

	Export(IdentifierPtr id, ExpressionPtr value)
			: Statement(kExport)
			, id(std::move(id))
			, value(std::move(value)) {}
};

struct FunctionDeclaration : public Statement {
	const IdentifierPtr id;
	const BlockPtr body;

	FunctionDeclaration(IdentifierPtr id, BlockPtr body)
			: Statement(kFunctionDeclaration)
			, id(std::move(id))
			, body(std::move(body)) {}
};

struct Return : public Statement {
	const ExpressionPtr expr;

	Return(ExpressionPtr expr) : Statement(kReturn), expr(std::move(expr)) {}
};

} // namespace melo::ast
