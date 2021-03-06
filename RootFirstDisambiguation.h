//
// Created by olcay on 4.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H


#include "NaiveDisambiguation.h"

class RootFirstDisambiguation : public NaiveDisambiguation {
protected:
    NGram<string>* wordBiGramModel;
    NGram<string>* igBiGramModel;
    double getWordProbability(string word, vector<FsmParse> correctFsmParses, int index);
    double getIgProbability(string word, vector<FsmParse> correctFsmParses, int index);
    Word* getBestRootWord(FsmParseList fsmParseList);
    FsmParse getParseWithBestIgProbability(FsmParseList parseList, vector<FsmParse> correctFsmParses, int index);
public:
    ~RootFirstDisambiguation();
    void train(DisambiguationCorpus& corpus);
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
    void saveModel();
    void loadModel();
};


#endif //MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H
