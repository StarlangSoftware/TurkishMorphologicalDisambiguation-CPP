//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H
#define MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H

#include "FsmMorphologicalAnalyzer.h"

class AutoDisambiguator {
protected:
    FsmMorphologicalAnalyzer morphologicalAnalyzer;
private:
    static bool isAnyWordSecondPerson(int index, const vector<FsmParse>& correctParses);
    static bool isPossessivePlural(int index, const vector<FsmParse>& correctParses);
    static string nextWordPos(const FsmParseList& nextParseList);
    static bool isBeforeLastWord(int index, int length);
    static bool nextWordExists(int index, int length);
    static bool isNextWordNoun(int index, const FsmParseList* fsmParses, int length);
    static bool isNextWordNum(int index, const FsmParseList* fsmParses, int length);
    static bool isNextWordNounOrAdjective(int index, const FsmParseList* fsmParses, int length);
    static bool isFirstWord(int index);
    static bool containsTwoNeOrYa(const FsmParseList* fsmParses, const string& word, int length);
    static bool hasPreviousWordTag(int index, const vector<FsmParse>& correctParses, MorphologicalTag tag);
    static string selectCaseForParseString(const string& parseString, int index, FsmParseList* fsmParses, const vector<FsmParse>& correctParses, int length);
public:
    static FsmParse caseDisambiguator(int index, FsmParseList* fsmParses, const vector<FsmParse>& correctParses, int length);
};


#endif //MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H
