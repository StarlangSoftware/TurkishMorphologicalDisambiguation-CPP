//
// Created by olcay on 2019-05-16.
//

#ifndef ANNOTATEDTREE_NODEDRAWABLECONDITION_H
#define ANNOTATEDTREE_NODEDRAWABLECONDITION_H


#include "ParseNodeDrawable.h"

class NodeDrawableCondition {
public:
    virtual bool satisfies(ParseNodeDrawable* parseNode) = 0;
};


#endif //ANNOTATEDTREE_NODEDRAWABLECONDITION_H
