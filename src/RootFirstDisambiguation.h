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
    double getWordProbability(const string& word, const vector<FsmParse>& correctFsmParses, int index) const;
    double getIgProbability(const string& word, const vector<FsmParse>& correctFsmParses, int index) const;
    Word* getBestRootWord(const FsmParseList& fsmParseList) const;
    FsmParse getParseWithBestIgProbability(const FsmParseList& parseList, const vector<FsmParse>& correctFsmParses, int index) const;
public:
    ~RootFirstDisambiguation();
    void train(const DisambiguationCorpus& corpus);
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) const override;
    void saveModel();
    void loadModel();
};


#endif //MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTDISAMBIGUATION_H
