//
// Created by Olcay Taner YILDIZ on 28.08.2020.
//

#ifndef PARSETREE_CONSTITUENTSPAN_H
#define PARSETREE_CONSTITUENTSPAN_H


#include "Symbol.h"

class ConstituentSpan {
private:
    Symbol constituent = Symbol("");
    int start;
    int end;
public:
    ConstituentSpan(Symbol constituent, int start, int end);
    Symbol getConstituent();
    int getStart();
    int getEnd();
};


#endif //PARSETREE_CONSTITUENTSPAN_H
