//
// Created by Olcay Taner YILDIZ on 2019-05-17.
//

#ifndef ANNOTATEDTREE_TURKISHPARTOFSPEECHDISAMBIGUATOR_H
#define ANNOTATEDTREE_TURKISHPARTOFSPEECHDISAMBIGUATOR_H


#include "PartOfSpeechDisambiguator.h"

class TurkishPartOfSpeechDisambiguator : public PartOfSpeechDisambiguator {
public:
    static bool isLastNode(int i, vector<ParseNodeDrawable*> leafList);
    static string defaultCaseForParseString(string rootForm, string parseString, string partOfSpeech);
    vector<FsmParse> simpleSingleWordDisambiguate(FsmParseList* fsmParses, int size, string partOfSpeech);
protected:
    FsmParse* parseShortEnough(vector<FsmParse> fsmParses);
    FsmParse* caseDisambiguator(vector<FsmParse> fsmParses);
private:
    vector<FsmParse> singleWordInitialPosDisambiguate(FsmParseList* fsmParses, string partOfSpeech);
    vector<FsmParse> singleWordWithParseLengthDisambiguate(FsmParseList* fsmParses, string partOfSpeech, bool initialPos);
};


#endif //ANNOTATEDTREE_TURKISHPARTOFSPEECHDISAMBIGUATOR_H
