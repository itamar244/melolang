#include "melo/evaluator/section_walker.h"
#include <atic/ptr.h>
#include "evaluator/evaluate_nodes.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

SectionWalker::SectionWalker(Scope& scope, const ListLiteralValue* section)
		: scope_(scope)
		, section_(section)
		, section_size_(section->size())
		, section_iterator_(section->begin()) {}

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

// FIXME: will have issues with empty nested_walker_
Phrase SectionWalker::GetCurPhrase() {
	if (nested_walker_) {
		return nested_walker_->GetCurPhrase();
	}

	auto val = EvaluateExpr(scope_, *section_iterator_);
	if (auto list = val->AsListLiteralValue()) {
		nested_walker_ = new SectionWalker(scope_, list);
		return nested_walker_->GetCurPhrase();
	}

	return PhraseLiteralToValue(val->ExpectPhraseValue()->phrase);
}

} // namespace melo::evaluator
