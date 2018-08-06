#include "melo/evaluator/section_walker.h"
#include <memory>
#include "evaluator/utils.h"

namespace melo::evaluator {

bool SectionWalker::HasNextPhrase() const {
	auto& phrases = section_->list->elements;

	return pos_ < phrases.size();
}

Phrase SectionWalker::GetCurPhrase() {
	if (phrase_cache_ == nullptr) {
		auto& phrases = section_->list->elements;

		phrase_cache_ = std::make_unique<Phrase>(
				PhraseNodeToPhrase(phrases.at(pos_)->AsPhraseLiteral()));
	}

	return *phrase_cache_;
}

} // namespace melo::evaluator
