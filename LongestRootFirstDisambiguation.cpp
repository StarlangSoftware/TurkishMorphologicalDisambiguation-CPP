//
// Created by Olcay Taner YILDIZ on 24.02.2020.
//

#include "LongestRootFirstDisambiguation.h"

/**
 * The disambiguate method gets an array of fsmParses. Then loops through that parses and finds the longest root
 * word. At the end, gets the parse with longest word among the fsmParses and adds it to the correctFsmParses
 * {@link ArrayList}.
 *
 * @param fsmParses {@link FsmParseList} to disambiguate.
 * @return correctFsmParses {@link ArrayList} which holds the parses with longest root words.
 */
vector<FsmParse> LongestRootFirstDisambiguation::disambiguate(FsmParseList *fsmParses, int size) {
    vector<FsmParse> correctFsmParses;
    FsmParse bestParse;
    for (int i = 0; i < size; i++) {
        FsmParseList fsmParseList = fsmParses[i];
        if (fsmParseList.size() > 0) {
            bestParse = fsmParseList.getParseWithLongestRootWord();
            correctFsmParses.emplace_back(bestParse);
        }
    }
    return correctFsmParses;
}
