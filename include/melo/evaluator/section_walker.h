#pragma once

#include <atic/ptr.h>
#include <atic/list.h>
#include "melo/ast.h"
#include "melo/evaluator/scope.h"
#include "melo/evaluator/values.h"
#include "melo/phrase.h"

namespace melo::evaluator {

class SectionWalker {
public:
	SectionWalker(Scope& scope, const ListLiteralValue* section);
	~SectionWalker();

	inline std::size_t pos() const {
		return pos_;
	}

	inline const SectionWalker* nested_walker() const {
		return nested_walker_;
	}

	inline void MoveToBegin() {
		pos_ = 0;
		section_iterator_ = section_->begin();
	}

	bool HasNextPhrase() const;
	inline bool HasNextItem() const {
		return pos_ < section_size_;
	}
	void Next();
	Phrase GetCurPhrase();

private:
	Scope& scope_;
	const std::size_t section_size_;
	const ListLiteralValue* section_;
	atic::List<ast::Expression*>::const_iterator section_iterator_;
	SectionWalker* nested_walker_ = nullptr;
	std::size_t pos_ = 0;
	bool to_flat_lists_;

	SectionWalker(
			Scope& scope, const ListLiteralValue* section, bool to_flat_lists);
};

}  // namespace melo::evaluator
