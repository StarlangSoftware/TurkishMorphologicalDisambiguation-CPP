//
// Created by Olcay Taner YILDIZ on 2019-05-12.
//

#ifndef ANNOTATEDTREE_PARSENODEDRAWABLE_H
#define ANNOTATEDTREE_PARSENODEDRAWABLE_H


#include <ParseNode.h>
#include "LayerInfo.h"
#include "ParseTreeSearchable.h"
#include "ParseNodeSearchable.h"

class ParseNodeDrawable : public ParseNode{
protected:
    LayerInfo* layers = nullptr;
    int depth;
public:
    ParseNodeDrawable(ParseNodeDrawable* parent, string line, bool isLeaf, int depth);
    ~ParseNodeDrawable();
    explicit ParseNodeDrawable(Symbol data);
    ParseNodeDrawable(ParseNodeDrawable* parent, ParseNodeDrawable* child, string symbol);
    LayerInfo* getLayerInfo();
    Symbol getData();
    void clearLayers();
    void clearLayer(ViewLayerType layerType);
    void clearData();
    void setDataAndClearLayers(Symbol data);
    void setData(Symbol data);
    string headWord(ViewLayerType viewLayerType);
    string getLayerData();
    string getLayerData(ViewLayerType viewLayer);
    int getDepth();
    int structureAgreementCount(ParseNodeDrawable* parseNode);
    int glossAgreementCount(ParseNodeDrawable* parseNode, ViewLayerType viewLayerType);
    void updateDepths(int depth);
    int maxDepth();
    string ancestorString();
    bool layerExists(ViewLayerType viewLayerType);
    bool isDummyNode();
    bool layerAll(ViewLayerType viewLayerType);
    string toTurkishSentence();
    void checkGazetteer(Gazetteer gazetteer, string word);
    string to_string();
    bool satisfy(ParseNodeSearchable* node);
    vector<ParseNodeDrawable*> satisfy(ParseTreeSearchable tree);
};


#endif //ANNOTATEDTREE_PARSENODEDRAWABLE_H
