//
// Created by olcay on 2.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_DUMMYDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_DUMMYDISAMBIGUATION_H


#include "MorphologicalDisambiguator.h"

class DummyDisambiguation : public MorphologicalDisambiguator {
public:
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_DUMMYDISAMBIGUATION_H
