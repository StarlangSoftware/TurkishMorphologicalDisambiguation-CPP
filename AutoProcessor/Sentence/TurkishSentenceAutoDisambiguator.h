//
// Created by olcay on 2.05.2019.
//

#ifndef ANNOTATEDSENTENCE_TURKISHSENTENCEAUTODISAMBIGUATOR_H
#define ANNOTATEDSENTENCE_TURKISHSENTENCEAUTODISAMBIGUATOR_H


#include "SentenceAutoDisambiguator.h"
#include "../../RootWordStatisticsDisambiguation.h"

class TurkishSentenceAutoDisambiguator : public SentenceAutoDisambiguator{
public:
    TurkishSentenceAutoDisambiguator(FsmMorphologicalAnalyzer& morphologicalAnalyzer, RootWordStatistics& rootWordStatistics);
protected:
    void autoDisambiguateMultipleRootWords(AnnotatedSentence* sentence) override;
private:
    RootWordStatisticsDisambiguation rootWordStatisticsDisambiguation;
    void setParseAutomatically(FsmParse disambiguatedParse, AnnotatedWord* word);
};


#endif //ANNOTATEDSENTENCE_TURKISHSENTENCEAUTODISAMBIGUATOR_H
