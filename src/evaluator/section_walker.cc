#include "melo/evaluator/section_walker.h"
#include <memory>
#include "melo/evaluator/evaluate_nodes.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

bool SectionWalker::HasNextPhrase() const {
	auto& phrases = section_->list->elements;

	return pos_ < phrases.size();
}

PhraseValue SectionWalker::GetCurPhrase() {
	if (phrase_cache_ == nullptr) {
		phrase_cache_ = EvaluateExpr(
				scope_, section_->list->elements.at(pos_))->AsPhraseValue();
	}

	return *phrase_cache_;
}

} // namespace melo::evaluator
