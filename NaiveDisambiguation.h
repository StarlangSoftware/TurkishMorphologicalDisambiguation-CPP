//
// Created by olcay on 2.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_NAIVEDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_NAIVEDISAMBIGUATION_H


#include <NGram.h>
#include "MorphologicalDisambiguator.h"

class NaiveDisambiguation : public MorphologicalDisambiguator {
protected:
    NGram<string> wordUniGramModel;
    NGram<string> igUniGramModel;
};


#endif //MORPHOLOGICALDISAMBIGUATION_NAIVEDISAMBIGUATION_H
