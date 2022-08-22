//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#include <FsmParse.h>
#include <FsmParseList.h>
#include "DisambiguationCorpus.h"

#ifndef MORPHOLOGICALDISAMBIGUATION_MORPHOLOGICALDISAMBIGUATOR_H
#define MORPHOLOGICALDISAMBIGUATION_MORPHOLOGICALDISAMBIGUATOR_H

class MorphologicalDisambiguator{
public:
    /**
     * Method to train the given {@link DisambiguationCorpus}.
     *
     * @param corpus {@link DisambiguationCorpus} to train.
     */
    void train(DisambiguationCorpus& corpus);

    /**
     * Method to disambiguate the given {@link FsmParseList}.
     *
     * @param fsmParses {@link FsmParseList} to disambiguate.
     * @return ArrayList of {@link FsmParse}.
     */
    virtual vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) = 0;

    /**
     * Method to save a model.
     */
    void saveModel();

    /**
     * Method to load a model.
     */
    void loadModel();
};

#endif //MORPHOLOGICALDISAMBIGUATION_MORPHOLOGICALDISAMBIGUATOR_H
