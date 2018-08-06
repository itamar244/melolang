#pragma once

#include <memory>
#include "melo/phrase.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class SectionWalker {
public:
	SectionWalker(const ListLiteralValue* section) : section_(section) {};

	inline std::size_t pos() const {
		return pos_;
	}
	inline void Next() {
		phrase_cache_ = nullptr;
		++pos_;
	}

	bool HasNextPhrase() const;
	Phrase GetCurPhrase();

private:
	const ListLiteralValue* section_;

	std::unique_ptr<Phrase> phrase_cache_ = nullptr;
	std::size_t pos_ = 0;
};

}  // namespace melo::evaluator
