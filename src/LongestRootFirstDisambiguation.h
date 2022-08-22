//
// Created by Olcay Taner YILDIZ on 24.02.2020.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H

#include "MorphologicalDisambiguator.h"

class LongestRootFirstDisambiguation: public MorphologicalDisambiguator {
private:
    map<string, string> rootList;
    void readFromFile(string fileName);
public:
    LongestRootFirstDisambiguation();
    explicit LongestRootFirstDisambiguation(string fileName);
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H
