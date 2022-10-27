//
// Created by olcay on 2.04.2019.
//

#include "DummyDisambiguation.h"

/**
 * Overridden disambiguate method takes an array of {@link FsmParseList} and loops through its items, if the current FsmParseList's
 * size is greater than 0, it adds a random parse of this list to the correctFsmParses {@link vector}.
 *
 * @param fsmParses {@link FsmParseList} to disambiguate.
 * @return correctFsmParses {@link vector}.
 */
vector<FsmParse> DummyDisambiguation::disambiguate(FsmParseList *fsmParses, int size) const{
    vector<FsmParse> correctFsmParses;
    for (int i = 0; i < size; i++) {
        FsmParseList fsmParseList = fsmParses[i];
        if (fsmParseList.size() > 0) {
            correctFsmParses.emplace_back(fsmParseList.getFsmParse(random() % fsmParseList.size()));
        }
    }
    return correctFsmParses;
}
