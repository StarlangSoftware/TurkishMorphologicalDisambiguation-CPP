//
// Created by Olcay Taner YILDIZ on 2019-04-20.
//

#ifndef PARSETREE_SYMBOL_H
#define PARSETREE_SYMBOL_H

#include <vector>
#include "Word.h"

class Symbol : public Word{
private:
    static const vector<string> nonTerminalList;
    static const vector<string> phraseLabels;
    static const vector<string> sentenceLabels;
    static const vector<string> verbLabels;
    string VPLabel = "VP";
public:
    explicit Symbol(string name);
    bool isVerb();
    bool isVP();
    bool isTerminal();
    bool isChunkLabel();
    Symbol trimSymbol();
    static bool contains(vector<string> list, string searchedItem);
    bool operator==(const Symbol &anotherSymbol) const{
        return (name == anotherSymbol.name);
    }
    bool operator<(const Symbol &anotherSymbol) const{
        return (name < anotherSymbol.name);
    }
};


#endif //PARSETREE_SYMBOL_H
