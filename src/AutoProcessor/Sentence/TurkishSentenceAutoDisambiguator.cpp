//
// Created by olcay on 2.05.2019.
//

#include "TurkishSentenceAutoDisambiguator.h"

/**
 * Constructor for the class.
 */
TurkishSentenceAutoDisambiguator::TurkishSentenceAutoDisambiguator(const FsmMorphologicalAnalyzer& morphologicalAnalyzer) : SentenceAutoDisambiguator(morphologicalAnalyzer) {
}

/**
 * If the words has only single root in its possible parses, the method disambiguates by looking special cases.
 * The cases are implemented in the caseDisambiguator method.
 * @param disambiguatedParse Parse used in setting the parse.
 * @param word Word to be disambiguated.
 */
void TurkishSentenceAutoDisambiguator::setParseAutomatically(const FsmParse& disambiguatedParse, AnnotatedWord* word) {
    word->setParse(disambiguatedParse.transitionlist());
    word->setMetamorphicParse(disambiguatedParse.getWithList());
}

/**
 * The method disambiguates words with multiple possible root words in its morphological parses. If the word
 * is already morphologically disambiguated, the method does not disambiguate that word. The method first check
 * for multiple root words by using rootWords method. If there are multiple root words, the method select the most
 * occurring root word (if its occurence wrt other root words occurence is above some threshold) for that word
 * using the bestRootWord method. If root word is selected, then the case for single root word is called.
 * @param sentence The sentence to be disambiguated automatically.
 */
void TurkishSentenceAutoDisambiguator::autoDisambiguateMultipleRootWords(AnnotatedSentence* sentence) {
    FsmParseList* fsmParses = morphologicalAnalyzer.robustMorphologicalAnalysis(*sentence);
    vector<FsmParse> correctParses = longestRootFirstDisambiguation.disambiguate(fsmParses, sentence->wordCount());
    for (int i = 0; i < sentence->wordCount(); i++){
        auto* word = (AnnotatedWord*) sentence->getWord(i);
        if (word->getParse() == nullptr){
            setParseAutomatically(correctParses.at(i), word);
        }
    }
}
