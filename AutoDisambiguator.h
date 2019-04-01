//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H
#define MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H

#include "FsmMorphologicalAnalyzer.h"
#include "RootWordStatistics.h"

class AutoDisambiguator {
protected:
    FsmMorphologicalAnalyzer morphologicalAnalyzer;
    RootWordStatistics rootWordStatistics;
};


#endif //MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H
