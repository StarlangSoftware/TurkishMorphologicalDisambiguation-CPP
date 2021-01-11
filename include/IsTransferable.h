//
// Created by olcay on 2019-05-16.
//

#ifndef ANNOTATEDTREE_ISTRANSFERABLE_H
#define ANNOTATEDTREE_ISTRANSFERABLE_H
#include "IsLeafNode.h"

class IsTransferable : public IsLeafNode{
private:
    ViewLayerType secondLanguage;
public:
    explicit IsTransferable(ViewLayerType secondLanguage);
    bool satisfies(ParseNodeDrawable* parseNode);
};


#endif //ANNOTATEDTREE_ISTRANSFERABLE_H
