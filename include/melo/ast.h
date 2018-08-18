#pragma once

#include <cctype>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "melo/zone.h"
#include "melo/string.h"

namespace melo::ast {

#define MELO_WRAPPERS_NODE_TYPES(V)                                            \
	V(Block)

#define MELO_STATEMENT_NODE_TYPES(V)                                           \
	V(FunctionDeclaration)                                                       \
	V(Export)                                                                    \
	V(Return)

#define MELO_EXPRESSION_NODE_TYPES(V)                                          \
	V(ListLiteral)                                                               \
	V(PhraseLiteral)                                                             \
	V(Identifier)                                                                \
	V(FunctionCall)                                                              \
	V(NumericLiteral)                                                            \
	V(Spread)

#define MELO_AST_NODE_TYPES(V)                                                 \
	MELO_WRAPPERS_NODE_TYPES(V)                                                  \
	MELO_STATEMENT_NODE_TYPES(V)                                                 \
	MELO_EXPRESSION_NODE_TYPES(V)

enum NodeType : uint8_t {
#define DECLARE_TYPE_ENUM(TYPE) k##TYPE,
	MELO_AST_NODE_TYPES(DECLARE_TYPE_ENUM)
#undef DECLARE_TYPE_ENUM
};

#define FR_DECL(NAME) struct NAME;

	MELO_AST_NODE_TYPES(FR_DECL)
#undef FR_DECL

struct AstNode {
	const NodeType type;

#define V(NAME)                                                                \
	inline const NAME* As##NAME() const  {                                       \
		return type == k##NAME ? reinterpret_cast<const NAME*>(this) : nullptr;    \
	}                                                                            \
	inline bool Is##NAME() const { return As##NAME() != nullptr; }

	MELO_AST_NODE_TYPES(V)
#undef V

	void* operator new(std::size_t size, Zone* zone) { return zone->New(size); }

protected:
	AstNode(NodeType type) : type(type) {}

private:
	// for accidently preventing call `operator new` without Zone*
	void* operator new(std::size_t size);
};

struct Statement : public AstNode {
	Statement(NodeType type) : AstNode(type) {}
};

struct Expression : public AstNode {
	Expression(NodeType type) : AstNode(type) {}
};

struct Identifier : public Expression {
	const String name;

	Identifier(const String& name) : Expression(kIdentifier), name(name) {}
};

struct ListLiteral : public Expression {
	const std::vector<Expression*> elements;

	ListLiteral(std::vector<Expression*> elements)
			: Expression(kListLiteral), elements(elements) {}
};

struct NumericLiteral : public Expression {
	const String value;

	NumericLiteral(const String& value)
			: Expression(kNumericLiteral), value(value) {}
};

struct PhraseLiteral : public Expression {
	const NumericLiteral* length;
	const std::vector<Identifier*> notes;

	PhraseLiteral(
			const NumericLiteral* length, std::vector<Identifier*>& notes)
			: Expression(kPhraseLiteral)
			, length(length)
			, notes(notes) {}
};

struct Spread : public Expression {
	const Expression* value;

	Spread(Expression* value) : Expression(kSpread), value(value) {}
};

struct FunctionCall : public Expression {
	const Identifier* id;
	const std::vector<Expression*> args;

	FunctionCall(const Identifier* id, std::vector<Expression*>& args)
			: Expression(kFunctionCall)
			, id(id)
			, args(std::move(args)) {}
};

struct Block : public Statement {
	const std::vector<const Statement*> statements;

	Block(std::vector<const Statement*>& statements)
			: Statement(kBlock), statements(std::move(statements)) {}
};

struct Export : public Statement {
	const Identifier* id;
	const Expression* value;

	Export(const Identifier* id, const Expression* value)
			: Statement(kExport)
			, id(id)
			, value(value) {}
};

struct FunctionDeclaration : public Statement {
	const Identifier* id;
	const std::vector<Identifier*> params;
	const Block* body;

	FunctionDeclaration(
			const Identifier* id, std::vector<Identifier*>& params, const Block* body)
			: Statement(kFunctionDeclaration)
			, id(id)
			, params(std::move(params))
			, body(body) {}
};

struct Return : public Statement {
	const Expression* expr;

	Return(Expression* expr) : Statement(kReturn), expr(expr) {}
};


class NodeFactory {
public:
	NodeFactory(Zone* zone) : zone_(zone) {}

	inline String CreateString(const std::string& str) {
		return String(zone_, str);
	}

#define NODE_FACTORIES(NAME)                                                   \
 	template<typename... Args>                                                   \
	NAME* New##NAME(Args... args) {                                              \
		return new (zone_) NAME(args...);                                          \
	}

	MELO_AST_NODE_TYPES(NODE_FACTORIES)
#undef NODE_FACTORIES

private:
	Zone* zone_;
};

} // namespace melo::ast
