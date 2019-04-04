//
// Created by olcay on 2.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_HMMDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_HMMDISAMBIGUATION_H


#include "NaiveDisambiguation.h"

class HmmDisambiguation : public NaiveDisambiguation {
protected:
    NGram<string> wordBiGramModel;
    NGram<string> igBiGramModel;
public:
    void train(DisambiguationCorpus corpus) override;
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_HMMDISAMBIGUATION_H
