//
// Created by olcay on 2019-05-16.
//

#ifndef ANNOTATEDTREE_ISLEAFNODE_H
#define ANNOTATEDTREE_ISLEAFNODE_H


#include "NodeDrawableCondition.h"

class IsLeafNode : public NodeDrawableCondition{
public:
    bool satisfies(ParseNodeDrawable* parseNode) override;
};


#endif //ANNOTATEDTREE_ISLEAFNODE_H
