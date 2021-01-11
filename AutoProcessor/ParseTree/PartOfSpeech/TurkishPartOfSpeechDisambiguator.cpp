//
// Created by Olcay Taner YILDIZ on 2019-05-17.
//

#include "TurkishPartOfSpeechDisambiguator.h"

bool TurkishPartOfSpeechDisambiguator::isLastNode(int i, vector<ParseNodeDrawable *> leafList) {
    int j = i + 1;
    while (j < leafList.size()){
        if (leafList.at(j)->getLayerData(ViewLayerType::INFLECTIONAL_GROUP).empty() || leafList.at(j)->getLayerData(ViewLayerType::INFLECTIONAL_GROUP).find("PUNC") == string::npos){
            return false;
        }
        j++;
    }
    return true;
}

string TurkishPartOfSpeechDisambiguator::defaultCaseForParseString(string rootForm, string parseString, string partOfSpeech) {
    string defaultCase;
    if (parseString == "A3PL+P3PL+NOM$A3PL+P3SG+NOM$A3PL+PNON+ACC$A3SG+P3PL+NOM"){
        defaultCase = "A3PL+P3SG+NOM";
    } else {
        if (parseString == "P3SG+NOM$PNON+ACC"){
            if (partOfSpeech == "PROP"){
                defaultCase = "PNON+ACC";
            } else {
                defaultCase = "P3SG+NOM";
            }
        } else {
            if (parseString == "A3SG+P2SG$A3SG+PNON"){
                defaultCase = "A3SG+PNON";
            } else {
                if (parseString == "A2SG+P2SG$A3SG+P3SG"){
                    defaultCase = "A3SG+P3SG";
                } else {
                    if (parseString == "P2SG$P3SG"){
                        defaultCase = "P3SG";
                    } else {
                        if (parseString == "A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL$A3PL+PNON+NOM"){
                            defaultCase = "A3PL+PNON+NOM";
                        } else {
                            if (parseString == "P2SG+NOM$PNON+GEN"){
                                defaultCase = "PNON+GEN";
                            } else {
                                if (parseString == "AOR^DB+ADJ+ZERO$AOR+A3SG"){
                                    defaultCase = "AOR+A3SG";
                                } else {
                                    if (parseString == "P2SG$PNON"){
                                        defaultCase = "PNON";
                                    } else {
                                        if (parseString == "ADV+SINCE$VERB+ZERO+PRES+COP+A3SG"){
                                            if (rootForm == "yıl" || rootForm == "süre" || rootForm == "zaman" || rootForm == "ay"){
                                                defaultCase = "ADV+SINCE";
                                            } else {
                                                defaultCase = "VERB+ZERO+PRES+COP+A3SG";
                                            }
                                        } else {
                                            if (parseString == "CONJ$VERB+POS+IMP+A2SG"){
                                                defaultCase = "CONJ";
                                            } else {
                                                if (parseString == "NEG+IMP+A2SG$POS^DB+NOUN+INF2+A3SG+PNON+NOM"){
                                                    defaultCase = "POS^DB+NOUN+INF2+A3SG+PNON+NOM";
                                                } else {
                                                    if (parseString == "NEG+OPT+A3SG$POS^DB+NOUN+INF2+A3SG+PNON+DAT"){
                                                        defaultCase = "POS^DB+NOUN+INF2+A3SG+PNON+DAT";
                                                    } else {
                                                        if (parseString == "NOUN+A3SG+P3SG+NOM$NOUN^DB+ADJ+ALMOST"){
                                                            defaultCase = "NOUN+A3SG+P3SG+NOM";
                                                        } else {
                                                            if (parseString == "ADJ$VERB+POS+IMP+A2SG"){
                                                                defaultCase = "ADJ";
                                                            } else {
                                                                if (parseString == "NOUN+A3SG+PNON+NOM$VERB+POS+IMP+A2SG"){
                                                                    defaultCase = "NOUN+A3SG+PNON+NOM";
                                                                } else {
                                                                    if (parseString == "INF2+A3SG+P3SG+NOM$INF2^DB+ADJ+ALMOST$"){
                                                                        defaultCase = "INF2+A3SG+P3SG+NOM";
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return defaultCase;
}

FsmParse* TurkishPartOfSpeechDisambiguator::parseShortEnough(vector<FsmParse> fsmParses) {
    int minLength = INT_MAX, min2Length = INT_MAX;
    FsmParse* bestFsmParse = nullptr;
    for (FsmParse fsmParse : fsmParses){
        int length = fsmParse.getTransitionList().size();
        if (length < minLength){
            min2Length = minLength;
            minLength = length;
            bestFsmParse = &fsmParse;
        } else {
            if (length < min2Length){
                min2Length = length;
            }
        }
    }
    if (min2Length - minLength > 10 && !Word::endsWith(bestFsmParse->getTransitionList(), "ADV+SINCE") && !Word::endsWith(bestFsmParse->getTransitionList(), "NOUN+A3SG+P1SG+DAT") && !Word::endsWith(bestFsmParse->getTransitionList(), "NOUN+A3SG+PNON+DAT")){
        return bestFsmParse;
    }
    return nullptr;
}

FsmParse *TurkishPartOfSpeechDisambiguator::caseDisambiguator(vector<FsmParse> fsmParses) {
    string defaultCase;
    if (fsmParses.size() == 1){
        return &fsmParses.at(0);
    }
    if (fsmParses.empty()){
        return nullptr;
    }
    string parseString = FsmParseList(fsmParses).parsesWithoutPrefixAndSuffix();
    defaultCase = defaultCaseForParseString(fsmParses.at(0).getWord()->getName(), parseString, fsmParses.at(0).getFinalPos());
    if (!defaultCase.empty()){
        for (int i = 0; i < fsmParses.size(); i++){
            if (fsmParses.at(i).getTransitionList().find(defaultCase) != string::npos){
                return &fsmParses.at(i);
            }
        }
    }
    return nullptr;
}

vector<FsmParse> TurkishPartOfSpeechDisambiguator::singleWordInitialPosDisambiguate(FsmParseList *fsmParses, string partOfSpeech) {
    vector<FsmParse> result;
    for (int i = 0; i < fsmParses[0].size(); i++){
        if (!fsmParses[0].getFsmParse(i).getInitialPos().empty() && fsmParses[0].getFsmParse(i).getInitialPos() == partOfSpeech){
            if (result.empty()){
                result.emplace_back(fsmParses[0].getFsmParse(i));
            } else {
                return result;
            }
        }
    }
    return result;
}

vector<FsmParse> TurkishPartOfSpeechDisambiguator::simpleSingleWordDisambiguate(FsmParseList *fsmParses, int size, string partOfSpeech) {
    if (size > 1){
        return vector<FsmParse>();
    }
    return singleWordInitialPosDisambiguate(fsmParses, move(partOfSpeech));
}

vector<FsmParse>
TurkishPartOfSpeechDisambiguator::singleWordWithParseLengthDisambiguate(FsmParseList *fsmParses, string partOfSpeech,
                                                                        bool initialPos) {
    vector<FsmParse> result;
    FsmParse* bestFsmParse;
    vector<FsmParse> bestList;
    for (int i = 0; i < fsmParses[0].size(); i++){
        if (initialPos){
            if (!fsmParses[0].getFsmParse(i).getInitialPos().empty() && fsmParses[0].getFsmParse(i).getInitialPos() == partOfSpeech){
                bestList.emplace_back(fsmParses[0].getFsmParse(i));
            }
        } else {
            if (!fsmParses[0].getFsmParse(i).getFinalPos().empty() && fsmParses[0].getFsmParse(i).getFinalPos() == partOfSpeech){
                bestList.emplace_back(fsmParses[0].getFsmParse(i));
            }
        }
    }
    bestFsmParse = caseDisambiguator(bestList);
    if (bestFsmParse != nullptr) {
        result.emplace_back(*bestFsmParse);
    } else {
        bestFsmParse = parseShortEnough(bestList);
        if (bestFsmParse != nullptr){
            result.emplace_back(*bestFsmParse);
        }
    }
    return result;
}
