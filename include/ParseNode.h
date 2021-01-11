//
// Created by Olcay Taner YILDIZ on 2019-04-21.
//

#ifndef PARSETREE_PARSENODE_H
#define PARSETREE_PARSENODE_H

#include <vector>
#include "Symbol.h"
#include "SearchDirectionType.h"
#include "ConstituentSpan.h"

using namespace std;

class ParseNode {
private:
    static const vector<string> ADJP;
    static const vector<string> ADVP;
    static const vector<string> CONJP;
    static const vector<string> FRAG;
    static const vector<string> INTJ;
    static const vector<string> LST;
    static const vector<string> NAC;
    static const vector<string> PP;
    static const vector<string> PRN;
    static const vector<string> PRT;
    static const vector<string> QP;
    static const vector<string> RRC;
    static const vector<string> S;
    static const vector<string> SBAR;
    static const vector<string> SBARQ;
    static const vector<string> SINV;
    static const vector<string> SQ;
    static const vector<string> UCP;
    static const vector<string> VP;
    static const vector<string> WHADJP;
    static const vector<string> WHADVP;
    static const vector<string> WHNP;
    static const vector<string> WHPP;
    static const vector<string> NP1;
    static const vector<string> NP2;
    static const vector<string> NP3;
    static const vector<string> NP4;
    static const vector<string> NP5;
    ParseNode* searchHeadChild(vector<string> priorityList, SearchDirectionType direction, bool defaultCase);
protected:
    vector<ParseNode*> children;
    ParseNode* parent = nullptr;
    Symbol data = Symbol("");
public:
    ParseNode() = default;
    ~ParseNode();
    ParseNode(ParseNode* parent, string line, bool isLeaf);
    ParseNode(ParseNode* left, ParseNode* right, Symbol data);
    ParseNode(ParseNode* left, Symbol data);
    explicit ParseNode(Symbol data);
    ParseNode* headLeaf();
    ParseNode* headChild();
    vector<ParseNode*>::iterator getChildIterator();
    void addChild(ParseNode* child);
    void correctParents();
    void removeXNodes();
    void addChild(int index, ParseNode* child);
    void setChild(int index, ParseNode* child);
    void removeChild(ParseNode* child);
    int leafCount();
    int nodeCount();
    int nodeCountWithMultipleChildren();
    void stripPunctuation();
    int numberOfChildren();
    ParseNode* getChild(int i);
    ParseNode* firstChild();
    ParseNode* lastChild();
    bool isLastChild(ParseNode* child);
    int getChildIndex(ParseNode* child);
    bool isDescendant(ParseNode* node);
    ParseNode* previousSibling();
    ParseNode* nextSibling();
    ParseNode* getParent();
    Symbol getData();
    void setData(Symbol data);
    int wordCount(bool excludeStopWords);
    void constituentSpanList(int startIndex, vector<ConstituentSpan>& list);
    bool isLeaf();
    bool isDummyNode();
    string toSentence();
    string to_string();
    void moveLeft(ParseNode* node);
    string ancestorString();
    void moveRight(ParseNode* node);
};


#endif //PARSETREE_PARSENODE_H
