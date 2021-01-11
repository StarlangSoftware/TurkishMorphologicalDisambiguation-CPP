//
// Created by Olcay Taner Yıldız on 2019-04-30.
//

#include "SentenceAutoDisambiguator.h"

/**
 * Constructor for the class.
 * @param morphologicalAnalyzer Morphological analyzer for parsing the words. Morphological analyzer will return all
 *                              possible parses of each word so that the automatic disambiguator can disambiguate the
 *                              words.
 * @param rootWordStatistics The object contains information about the selected correct root words in a corpus for a set
 *                           of possible lemma. Root word statistics can be used to distinguish between possible root words.
 */
SentenceAutoDisambiguator::SentenceAutoDisambiguator(FsmMorphologicalAnalyzer& morphologicalAnalyzer,
                                                     RootWordStatistics& rootWordStatistics) {
    this->morphologicalAnalyzer = morphologicalAnalyzer;
    this->rootWordStatistics = rootWordStatistics;
}

/**
 * The main method to automatically disambiguate a sentence. The algorithm
 * 1. Disambiguates the morphological analyses with a single analysis.
 * 2. Disambiguates the morphological analyses in which the possible analyses contain only one
 * distinct root word.
 * 3. Disambiguates the morphological analyses where there are multiple candidate root words and
 * possibly multiple candidate morphological analyses for each candidate root word.
 * @param sentence The sentence to be disambiguated automatically.
 */
void SentenceAutoDisambiguator::autoDisambiguate(AnnotatedSentence* sentence) {
    autoDisambiguateMultipleRootWords(sentence);
}
