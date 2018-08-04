#pragma once

#include <memory>
#include "../ast.h"
#include "../phrase.h"

namespace melo::evaluator {

class SectionWalker {
public:
	SectionWalker(ast::Section* section) : section_(section) {};

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
	const ast::Section* section_;

	std::unique_ptr<Phrase> phrase_cache_ = nullptr;
	std::size_t pos_ = 0;
};

}  // namespace melo::evaluator
