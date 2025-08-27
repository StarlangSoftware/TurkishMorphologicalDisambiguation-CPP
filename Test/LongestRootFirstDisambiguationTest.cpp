//
// Created by Olcay Taner YILDIZ on 6.01.2021.
//

#include <FsmMorphologicalAnalyzer.h>
#include "catch.hpp"
#include "DisambiguatedWord.h"
#include "DisambiguationCorpus.h"
#include "../src/LongestRootFirstDisambiguation.h"

TEST_CASE("LongestRootFirstDisambiguationTest-testDisambiguation") {
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    DisambiguationCorpus corpus = DisambiguationCorpus("datasets/atis.txt");
    LongestRootFirstDisambiguation algorithm = LongestRootFirstDisambiguation();
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
    cout << (correctRoot + 0.0) / corpus.numberOfWords();
    cout << (correctParse + 0.0) / corpus.numberOfWords();
}