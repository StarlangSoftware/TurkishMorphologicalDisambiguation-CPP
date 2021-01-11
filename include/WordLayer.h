//
// Created by olcay on 2019-05-08.
//

#ifndef ANNOTATEDTREE_WORDLAYER_H
#define ANNOTATEDTREE_WORDLAYER_H

#include <string>
using namespace std;

class WordLayer {
protected:
    string layerValue;
    string layerName;
public:
    string getLayerValue();
    string getLayerName();
    string getLayerDescription();
};


#endif //ANNOTATEDTREE_WORDLAYER_H
