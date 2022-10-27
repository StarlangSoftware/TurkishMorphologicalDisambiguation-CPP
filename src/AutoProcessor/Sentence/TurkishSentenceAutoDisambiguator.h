//
// Created by olcay on 2.05.2019.
//

#ifndef ANNOTATEDSENTENCE_TURKISHSENTENCEAUTODISAMBIGUATOR_H
#define ANNOTATEDSENTENCE_TURKISHSENTENCEAUTODISAMBIGUATOR_H


#include "SentenceAutoDisambiguator.h"
#include "../../LongestRootFirstDisambiguation.h"

class TurkishSentenceAutoDisambiguator : public SentenceAutoDisambiguator{
public:
    TurkishSentenceAutoDisambiguator(const FsmMorphologicalAnalyzer& morphologicalAnalyzer);
protected:
    void autoDisambiguateMultipleRootWords(AnnotatedSentence* sentence) override;
private:
    LongestRootFirstDisambiguation longestRootFirstDisambiguation;
    void setParseAutomatically(const FsmParse& disambiguatedParse, AnnotatedWord* word);
};


#endif //ANNOTATEDSENTENCE_TURKISHSENTENCEAUTODISAMBIGUATOR_H
