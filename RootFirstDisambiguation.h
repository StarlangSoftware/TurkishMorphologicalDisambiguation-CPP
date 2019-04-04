//
// Created by olcay on 4.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H


#include "NaiveDisambiguation.h"

class RootFirstDisambiguation : public NaiveDisambiguation {
protected:
    NGram<string> wordBiGramModel = NGram<string>(0);
    NGram<string> igBiGramModel = NGram<string>(0);
    double getWordProbability(string word, vector<FsmParse> correctFsmParses, int index);
    double getIgProbability(string word, vector<FsmParse> correctFsmParses, int index);
    Word* getBestRootWord(FsmParseList fsmParseList);
    FsmParse getParseWithBestIgProbability(FsmParseList parseList, vector<FsmParse> correctFsmParses, int index);
public:
    void train(DisambiguationCorpus corpus) override;
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H
