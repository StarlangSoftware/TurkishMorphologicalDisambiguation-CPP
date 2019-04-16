//
// Created by olcay on 2.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_NAIVEDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_NAIVEDISAMBIGUATION_H


#include <NGram.h>
#include "MorphologicalDisambiguator.h"

class NaiveDisambiguation : public MorphologicalDisambiguator {
protected:
    NGram<string> wordUniGramModel = NGram<string>(0);
    NGram<string> igUniGramModel = NGram<string>(0);
public:
    void saveModel() override;
    void loadModel() override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_NAIVEDISAMBIGUATION_H
