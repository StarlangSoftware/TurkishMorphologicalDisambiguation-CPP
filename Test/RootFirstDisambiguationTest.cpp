//
// Created by Olcay Taner YILDIZ on 6.01.2021.
//

#include <FsmMorphologicalAnalyzer.h>
#include "catch.hpp"
#include "DisambiguationCorpus.h"
#include "../src/RootFirstDisambiguation.h"
#include "DisambiguatedWord.h"

TEST_CASE("RootFirstDisambiguationTest-testDisambiguation") {
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    DisambiguationCorpus corpus = DisambiguationCorpus("penntreebank.txt");
    RootFirstDisambiguation algorithm = RootFirstDisambiguation();
    algorithm.train(corpus);
    int correctParse = 0;
    int correctRoot = 0;
    for (int i = 0; i < corpus.sentenceCount(); i++){
        FsmParseList* sentenceAnalyses = fsm.robustMorphologicalAnalysis(*(corpus.getSentence(i)));
        vector<FsmParse> fsmParses =  algorithm.disambiguate(sentenceAnalyses, corpus.getSentence(i)->wordCount());
        for (int j = 0; j < corpus.getSentence(i)->wordCount(); j++){
            auto* word = (DisambiguatedWord*) corpus.getSentence(i)->getWord(j);
            if (Word::toLowerCase(fsmParses[j].transitionlist()) == Word::toLowerCase(word->getParse().to_string())){
                correctParse++;
                correctRoot++;
            } else {
                if (fsmParses[j].getWord()->getName() == word->getParse().getWord()->getName()){
                    correctRoot++;
                }
            }
        }
    }
    REQUIRE_THAT(0.9520, Catch::Matchers::WithinAbs((correctRoot + 0.0) / corpus.numberOfWords(), 0.002));
    REQUIRE_THAT(0.8632, Catch::Matchers::WithinAbs((correctParse + 0.0) / corpus.numberOfWords(), 0.002));
}