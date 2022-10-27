//
// Created by Olcay Taner YILDIZ on 24.02.2020.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H

#include "MorphologicalDisambiguator.h"

class LongestRootFirstDisambiguation: public MorphologicalDisambiguator {
private:
    map<string, string> rootList;
    void readFromFile(const string& fileName);
public:
    LongestRootFirstDisambiguation();
    explicit LongestRootFirstDisambiguation(const string& fileName);
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) const override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_LONGESTROOTFIRSTDISAMBIGUATION_H
