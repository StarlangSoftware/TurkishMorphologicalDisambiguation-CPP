//
// Created by Olcay Taner YILDIZ on 2019-05-10.
//

#ifndef ANNOTATEDTREE_LAYERINFO_H
#define ANNOTATEDTREE_LAYERINFO_H

#include <map>
#include <AnnotatedWord.h>
#include <ViewLayerType.h>
#include <MorphologicalParse.h>
#include <MetamorphicParse.h>
#include <Argument.h>
#include "WordLayer.h"

using namespace std;
class LayerInfo {
private:
    map<ViewLayerType, WordLayer*> layers;
    string getMultiWordAt(ViewLayerType viewLayerType, int index, string layerName);
    void updateMetaMorphemesMoved();
public:
    explicit LayerInfo(string info);
    LayerInfo() = default;
    ~LayerInfo();
    void setLayerData(ViewLayerType viewLayer, string layerValue);
    void setMorphologicalAnalysis(MorphologicalParse parse);
    void setMetaMorphemes(MetamorphicParse parse);
    bool layerExists(ViewLayerType viewLayerType);
    ViewLayerType checkLayer(ViewLayerType viewLayer);
    int getNumberOfWords();
    string getTurkishWordAt(int index);
    int getNumberOfMeanings();
    string getSemanticAt(int index);
    string getShallowParseAt(int index);
    Argument getArgument();
    Argument getArgumentAt(int index);
    MorphologicalParse getMorphologicalParseAt(int index);
    MetamorphicParse getMetamorphicParseAt(int index);
    string getMetaMorphemeAtIndex(int index);
    string getMetaMorphemeFromIndex(int index);
    int getLayerSize(ViewLayerType viewLayer);
    string getLayerInfoAt(ViewLayerType viewLayer, int index);
    string getLayerDescription();
    string getLayerData(ViewLayerType viewLayer);
    string getRobustLayerData(ViewLayerType viewLayer);
    void removeLayer(ViewLayerType layerType);
    void metaMorphemeClear();
    void englishClear();
    void dependencyClear();
    void metaMorphemesMovedClear();
    void semanticClear();
    void englishSemanticClear();
    void morphologicalAnalysisClear();
    MetamorphicParse metaMorphemeRemove(int index);
    bool isVerbal();
    bool isNominal();
    AnnotatedWord* toAnnotatedWord(int wordIndex);
};


#endif //ANNOTATEDTREE_LAYERINFO_H
