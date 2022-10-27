//
// Created by olcay on 2.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_HMMDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_HMMDISAMBIGUATION_H


#include "NaiveDisambiguation.h"

class HmmDisambiguation : public NaiveDisambiguation {
protected:
    NGram<string>* wordBiGramModel;
    NGram<string>* igBiGramModel;
public:
    ~HmmDisambiguation();
    void train(DisambiguationCorpus& corpus);
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) const override;
    void saveModel();
    void loadModel();
};


#endif //MORPHOLOGICALDISAMBIGUATION_HMMDISAMBIGUATION_H
