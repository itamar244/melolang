#pragma once

#include <memory>
#include <atic/ptr.h>
#include "melo/evaluator/scope.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class SectionWalker {
public:
	SectionWalker(LinkedScope& scope, const ListLiteralValue* section)
			: scope_(scope)
			, section_(section) {}

	~SectionWalker() {
		atic::MaybeDeletePtr(phrase_cache_);
	}

	inline std::size_t pos() const {
		return pos_;
	}
	inline void Next() {
		atic::MaybeDeletePtr(phrase_cache_);
		++pos_;
	}

	bool HasNextPhrase() const;
	PhraseValue GetCurPhrase();

private:
	LinkedScope& scope_;
	const ListLiteralValue* section_;
	const PhraseValue* phrase_cache_ = nullptr;
	std::size_t pos_ = 0;
};

}  // namespace melo::evaluator
