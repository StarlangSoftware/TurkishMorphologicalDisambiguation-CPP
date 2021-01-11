//
// Created by Olcay Taner YILDIZ on 2019-05-17.
//

#ifndef ANNOTATEDTREE_PARTOFSPEECHDISAMBIGUATOR_H
#define ANNOTATEDTREE_PARTOFSPEECHDISAMBIGUATOR_H


#include <FsmParse.h>
#include <FsmParseList.h>
#include "ParseTreeDrawable.h"

class PartOfSpeechDisambiguator {
    virtual FsmParse* disambiguate(FsmParseList* fsmParses, ParseNodeDrawable* node, ParseTreeDrawable* parseTree);
};


#endif //ANNOTATEDTREE_PARTOFSPEECHDISAMBIGUATOR_H
