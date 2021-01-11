//
// Created by Olcay Taner YILDIZ on 2019-04-22.
//

#ifndef PARSETREE_PARSETREE_H
#define PARSETREE_PARSETREE_H

#include <vector>
#include <string>
#include "ParseNode.h"
#include "ConstituentSpan.h"

using namespace std;

class ParseTree {
private:
    static const vector<string> sentenceLabels;
protected:
    ParseNode* root = nullptr;
public:
    ParseTree() = default;
    ~ParseTree();
    explicit ParseTree(ParseNode* root);
    explicit ParseTree(istream& inputFile);
    explicit ParseTree(string fileName);
    ParseNode* nextLeafNode(ParseNode* parseNode);
    ParseNode* previousLeafNode(ParseNode* parseNode);
    int nodeCountWithMultipleChildren();
    int nodeCount();
    int leafCount();
    bool isFullSentence();
    vector<ConstituentSpan> constituentSpanList();
    void save(const string& fileName);
    void correctParents();
    void removeXNodes();
    void stripPunctuation();
    ParseNode* getRoot();
    string toSentence();
    string to_string();
    int wordCount(bool excludeStopWords);
};


#endif //PARSETREE_PARSETREE_H
