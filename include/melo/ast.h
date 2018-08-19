#pragma once

#include <cctype>
#include <memory>
#include <string>
#include <utility>
#include <string>
#include <atic/list.h>
#include "melo/zone.h"

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

struct AstNode : public ZoneObject {
	using ZoneObject::operator new;
	const NodeType type;

#define V(NAME)                                                                \
	inline const NAME* As##NAME() const  {                                       \
		return type == k##NAME ? reinterpret_cast<const NAME*>(this) : nullptr;    \
	}                                                                            \
	inline bool Is##NAME() const { return As##NAME() != nullptr; }

	MELO_AST_NODE_TYPES(V)
#undef V

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
	const std::string name;

	Identifier(const std::string& name) : Expression(kIdentifier), name(name) {}
};

struct ListLiteral : public Expression {
	const atic::List<Expression*> elements;

	ListLiteral(const atic::List<Expression*>& elements)
			: Expression(kListLiteral), elements(elements) {}
};

struct NumericLiteral : public Expression {
	const std::string value;

	NumericLiteral(const std::string& value)
			: Expression(kNumericLiteral), value(value) {}
};

struct PhraseLiteral : public Expression {
	const NumericLiteral* length;
	const atic::List<Identifier*> notes;

	PhraseLiteral(
			const NumericLiteral* length, const atic::List<Identifier*>& notes)
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
	const atic::List<Expression*> args;

	FunctionCall(const Identifier* id, const atic::List<Expression*>& args)
			: Expression(kFunctionCall)
			, id(id)
			, args(args) {}
};

struct Block : public Statement {
	const atic::List<Statement*> statements;

	Block(const atic::List<Statement*>& statements)
			: Statement(kBlock), statements(statements) {}
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
	const atic::List<Identifier*> params;
	const Block* body;

	FunctionDeclaration(
			const Identifier* id, const atic::List<Identifier*>& params, const Block* body)
			: Statement(kFunctionDeclaration)
			, id(id)
			, params(params)
			, body(body) {}
};

struct Return : public Statement {
	const Expression* expr;

	Return(Expression* expr) : Statement(kReturn), expr(expr) {}
};


class NodeFactory {
public:
	NodeFactory(Zone* zone) : zone_(zone) {}

#define NODE_FACTORIES(NAME)                                                   \
 	template<typename... Args>                                                   \
	NAME* New##NAME(const Args&... args) {                                       \
		return new (zone_) NAME(args...);                                          \
	}

	MELO_AST_NODE_TYPES(NODE_FACTORIES)
#undef NODE_FACTORIES

private:
	Zone* zone_;
};

std::string NodeTypeToString(NodeType);

} // namespace melo::ast
