#include "melo/evaluator/section_walker.h"
#include <memory>
#include <atic/ptr.h>
#include "melo/evaluator/evaluate_nodes.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

SectionWalker::~SectionWalker() {
	atic::MaybeDeletePtr(phrase_cache_);
}

const PhraseValue* SectionWalker::GetCurPhrase() {
	if (phrase_cache_ == nullptr) {
		phrase_cache_ = EvaluateExpr(
				scope_, *section_iterator_)->ExpectPhraseValue();
	}

	return phrase_cache_;
}

} // namespace melo::evaluator
