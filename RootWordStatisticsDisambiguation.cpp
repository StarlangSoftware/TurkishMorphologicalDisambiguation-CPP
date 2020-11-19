//
// Created by Olcay Taner YILDIZ on 19.11.2020.
//

#include "RootWordStatisticsDisambiguation.h"

void RootWordStatisticsDisambiguation::train(DisambiguationCorpus corpus) {
    ifstream inputFile;
    inputFile.open("penntrebank_statistics.txt", ifstream :: in);
    rootWordStatistics = RootWordStatistics(inputFile);
    inputFile.close();
}

vector<FsmParse> RootWordStatisticsDisambiguation::disambiguate(FsmParseList *fsmParses, int size) {
    vector<FsmParse> correctFsmParses;
    FsmParse bestParse;
    for (int i = 0; i < size; i++) {
        FsmParseList fsmParseList = fsmParses[i];
        string bestRoot = rootWordStatistics.bestRootWord(fsmParseList, 0.0);
        if (!bestRoot.empty()){
            fsmParseList.reduceToParsesWithSameRoot(bestRoot);
            FsmParse* newBestParse = fsmParseList.caseDisambiguator();
            if (newBestParse != nullptr){
                bestParse = *newBestParse;
            } else {
                bestParse = fsmParseList.getFsmParse(0);
            }
        } else {
            bestParse = fsmParseList.getFsmParse(0);
        }
        correctFsmParses.emplace_back(bestParse);
    }
    return correctFsmParses;
}
