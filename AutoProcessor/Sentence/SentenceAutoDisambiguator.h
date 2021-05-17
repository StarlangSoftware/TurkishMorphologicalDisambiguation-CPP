//
// Created by Olcay Taner Yıldız on 2019-04-30.
//

#ifndef ANNOTATEDSENTENCE_SENTENCEAUTODISAMBIGUATOR_H
#define ANNOTATEDSENTENCE_SENTENCEAUTODISAMBIGUATOR_H
#include "AnnotatedSentence.h"
#include "../../AutoDisambiguator.h"

/**
* Abstract class to disambiguate a sentence automatically. By implementing 3 abstract methods,
* the class can disambiguate
* (1) Simplest situation: Morphological analyses with a single analysis. For this case, there is actually no disambiguation,
* system selects 'the' morphological analysis.
* (2) More complex situation: Morphological analyses in which the possible analyses contain only one distinct root word. For this
* case, the root word would be fixed, but the correct morphological analysis depends on the context.
* (3) Most complex situation: Morphological analyses where there are multiple candidate root words and possibly multiple candidate
* morphological analyses for each candidate root word.
* Each method tries to disambiguate and if successful, sets the correct morphological analysis and the correct
* metamorpheme of the word.
 * */
class SentenceAutoDisambiguator : public AutoDisambiguator {
protected:
    /**
     * The written method should disambiguate morphological analyses where there are multiple candidate root words
     * and possibly multiple candidate morphological analyses for each candidate root word. If it finds the correct
     * morphological analysis of a word(s), it should set morphological analysis and metamorpheme of that(those) word(s).
     * To disambiguate between the root words, one can use the root word statistics.
     * @param sentence The sentence to be disambiguated automatically.
     */
    virtual void autoDisambiguateMultipleRootWords(AnnotatedSentence* sentence) = 0;

public:
    SentenceAutoDisambiguator(FsmMorphologicalAnalyzer& morphologicalAnalyzer);
    void autoDisambiguate(AnnotatedSentence* sentence);
};


#endif //ANNOTATEDSENTENCE_SENTENCEAUTODISAMBIGUATOR_H
