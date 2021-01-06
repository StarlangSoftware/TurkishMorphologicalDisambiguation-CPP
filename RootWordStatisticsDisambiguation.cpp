//
// Created by Olcay Taner YILDIZ on 19.11.2020.
//

#include "RootWordStatisticsDisambiguation.h"
#include "AutoDisambiguator.h"

void RootWordStatisticsDisambiguation::train(DisambiguationCorpus& corpus) {
    ifstream inputFile;
    inputFile.open("penntrebank_statistics.txt", ifstream :: in);
    rootWordStatistics = RootWordStatistics(inputFile);
    inputFile.close();
}

vector<FsmParse> RootWordStatisticsDisambiguation::disambiguate(FsmParseList *fsmParses, int size) {
    vector<FsmParse> correctFsmParses;
    FsmParse bestParse;
    string bestRoot;
    for (int i = 0; i < size; i++) {
        FsmParseList fsmParseList = fsmParses[i];
        string rootWords = fsmParseList.rootWords();
        if (rootWords.find('$') != std::string::npos){
            bestRoot = rootWordStatistics.bestRootWord(fsmParseList, 0.0);
            if (bestRoot.empty()){
                bestRoot = fsmParseList.getParseWithLongestRootWord().getWord()->getName();
            }
        } else {
            bestRoot = rootWords;
        }
        if (!bestRoot.empty()){
            fsmParses[i].reduceToParsesWithSameRoot(bestRoot);
            bestParse = AutoDisambiguator::caseDisambiguator(i, fsmParses, correctFsmParses, size);
        } else {
            bestParse = fsmParseList.getFsmParse(0);
        }
        correctFsmParses.emplace_back(bestParse);
    }
    return correctFsmParses;
}
