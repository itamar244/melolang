#pragma once

#include <atic/ptr.h>
#include "melo/ast.h"
#include <atic/list.h>
#include "melo/evaluator/scope.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class SectionWalker {
public:
	SectionWalker(Scope& scope, const ListLiteralValue* section);
	~SectionWalker();

	inline std::size_t pos() const {
		return pos_;
	}

	bool HasNextPhrase() const;
	void Next();
	const PhraseValue* GetCurPhrase();

private:
	Scope& scope_;
	const std::size_t section_size_;
	atic::List<ast::Expression*>::const_iterator section_iterator_;
	SectionWalker* nested_walker_ = nullptr;
	std::size_t pos_ = 0;
};

}  // namespace melo::evaluator
