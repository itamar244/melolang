#pragma once

#include <atic/ptr.h>
#include "melo/ast.h"
#include <atic/list.h>
#include "melo/evaluator/scope.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class SectionWalker {
public:
	SectionWalker(Scope& scope, const ListLiteralValue* section)
			: scope_(scope)
			, section_size_(section->list->elements.size())
			, section_iterator_(section->list->elements.begin()) {}

	~SectionWalker();

	inline std::size_t pos() const {
		return pos_;
	}

	inline void Next() {
		atic::MaybeDeletePtr(phrase_cache_);
		++pos_;
		++section_iterator_;
	}

	inline bool HasNextPhrase() const {
		return pos_ < section_size_;
	}

	const PhraseValue* GetCurPhrase();

private:
	Scope& scope_;
	const std::size_t section_size_;
	atic::List<ast::Expression*>::const_iterator section_iterator_;
	const PhraseValue* phrase_cache_ = nullptr;
	std::size_t pos_ = 0;
};

}  // namespace melo::evaluator
