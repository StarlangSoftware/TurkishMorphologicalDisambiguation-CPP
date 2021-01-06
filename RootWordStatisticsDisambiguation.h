//
// Created by Olcay Taner YILDIZ on 19.11.2020.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_ROOTWORDSTATISTICSDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_ROOTWORDSTATISTICSDISAMBIGUATION_H


#include <FsmParse.h>
#include <FsmParseList.h>
#include "DisambiguationCorpus.h"
#include "MorphologicalDisambiguator.h"
#include "RootWordStatistics.h"

class RootWordStatisticsDisambiguation : public MorphologicalDisambiguator{
private:
    RootWordStatistics rootWordStatistics;
public:
    void train(DisambiguationCorpus& corpus);
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_ROOTWORDSTATISTICSDISAMBIGUATION_H
