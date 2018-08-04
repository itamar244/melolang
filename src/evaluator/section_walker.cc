#include <melo/evaluator/section_walker.h>
#include <memory>
#include "utils.h"

namespace melo::evaluator {

bool SectionWalker::HasNextPhrase() const {
	auto& phrases = section_->phrases;

	return pos_ < phrases.size();
}

Phrase SectionWalker::GetCurPhrase() {
	if (phrase_cache_ == nullptr) {
		auto& phrases = section_->phrases;

		phrase_cache_ =
			std::make_unique<Phrase>(PhraseNodeToPhrase(phrases.at(pos_)));
	}

	return *phrase_cache_;
}

} // namespace melo::evaluator
