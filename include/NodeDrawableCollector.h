//
// Created by olcay on 2019-05-16.
//

#ifndef ANNOTATEDTREE_NODEDRAWABLECOLLECTOR_H
#define ANNOTATEDTREE_NODEDRAWABLECOLLECTOR_H


#include "NodeDrawableCondition.h"

class NodeDrawableCollector {
private:
    NodeDrawableCondition* condition = nullptr;
    ParseNodeDrawable* rootNode = nullptr;
    void collectNodes(ParseNodeDrawable* parseNode, vector<ParseNodeDrawable*>& collected);
public:
    NodeDrawableCollector(ParseNodeDrawable* rootNode, NodeDrawableCondition* condition);
    vector<ParseNodeDrawable*> collect();
};


#endif //ANNOTATEDTREE_NODEDRAWABLECOLLECTOR_H
