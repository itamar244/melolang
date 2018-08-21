#include "melo/evaluator/section_walker.h"
#include <iostream>
#include <atic/ptr.h>
#include "melo/evaluator/evaluate_nodes.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

SectionWalker::SectionWalker(Scope& scope, const ListLiteralValue* section)
		: scope_(scope)
		, section_size_(section->list->elements.size())
		, section_iterator_(section->list->elements.begin()) {}

SectionWalker::~SectionWalker() {
	atic::MaybeDeletePtr(nested_walker_);
}


bool SectionWalker::HasNextPhrase() const {
	return nested_walker_
			? pos_ <= section_size_ - 1 ||
				(pos_ == section_size_ - 1 && nested_walker_->HasNextPhrase())
			: pos_ < section_size_;
}

void SectionWalker::Next() {
	if (nested_walker_) {
		nested_walker_->Next();
		if (nested_walker_->HasNextPhrase()) {
			return;
		}
		atic::DeletePtr(nested_walker_);
	}

	++section_iterator_;
	++pos_;
}

const PhraseValue* SectionWalker::GetCurPhrase() {
	if (nested_walker_) {
		return nested_walker_->GetCurPhrase();
	}

	if (auto spread = (*section_iterator_)->AsSpread()) {
		nested_walker_ = new SectionWalker(
				scope_,
				EvaluateExpr(scope_, spread->value)->ExpectListLiteralValue());
		// FIXME: will have issues with empty nested_walker_
		return nested_walker_->GetCurPhrase();
	}

	return EvaluateExpr(
			scope_, *section_iterator_)->ExpectPhraseValue();
}

} // namespace melo::evaluator
