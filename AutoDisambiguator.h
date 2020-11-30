//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H
#define MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H

#include "FsmMorphologicalAnalyzer.h"
#include "RootWordStatistics.h"

class AutoDisambiguator {
protected:
    FsmMorphologicalAnalyzer morphologicalAnalyzer;
    RootWordStatistics rootWordStatistics;
private:
    static bool isAnyWordSecondPerson(int index, vector<FsmParse> correctParses);
    static bool isPossessivePlural(int index, vector<FsmParse> correctParses);
    static string nextWordPos(FsmParseList nextParseList);
    static bool isBeforeLastWord(int index, int length);
    static bool nextWordExists(int index, int length);
    static bool isNextWordNoun(int index, FsmParseList* fsmParses, int length);
    static bool isNextWordNum(int index, FsmParseList* fsmParses, int length);
    static bool isNextWordNounOrAdjective(int index, FsmParseList* fsmParses, int length);
    static bool isFirstWord(int index);
    static bool containsTwoNeOrYa(FsmParseList* fsmParses, string word, int length);
    static bool hasPreviousWordTag(int index, vector<FsmParse> correctParses, MorphologicalTag tag);
    static string selectCaseForParseString(string parseString, int index, FsmParseList* fsmParses, vector<FsmParse> correctParses, int length);
public:
    static FsmParse caseDisambiguator(int index, FsmParseList* fsmParses, vector<FsmParse> correctParses, int length);
};


#endif //MORPHOLOGICALDISAMBIGUATION_AUTODISAMBIGUATOR_H
