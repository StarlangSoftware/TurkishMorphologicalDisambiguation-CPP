//
// Created by Olcay Taner YILDIZ on 24.02.2020.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H

#include "MorphologicalDisambiguator.h"

class LongestRootFirstDisambiguation: public MorphologicalDisambiguator {
public:
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H
