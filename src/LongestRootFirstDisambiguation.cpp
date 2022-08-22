//
// Created by Olcay Taner YILDIZ on 24.02.2020.
//

#include "LongestRootFirstDisambiguation.h"
#include "AutoDisambiguator.h"

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
        string bestRoot;
        string surfaceForm = fsmParseList.getFsmParse(0).getSurfaceForm();
        if (rootList.contains(surfaceForm)){
            bestRoot = rootList[surfaceForm];
        }
        bool rootFound = false;
        for (int j = 0; j < fsmParseList.size(); j++) {
            if (fsmParseList.getFsmParse(j).getWord()->getName() == bestRoot) {
                rootFound = true;
                break;
            }
        }
        if (!rootFound){
            bestParse = fsmParseList.getParseWithLongestRootWord();
            fsmParses[i].reduceToParsesWithSameRoot(bestParse.getWord()->getName());
        } else {
            fsmParses[i].reduceToParsesWithSameRoot(bestRoot);
        }
        FsmParse newBestParse = AutoDisambiguator::caseDisambiguator(i, fsmParses, correctFsmParses, size);
        correctFsmParses.emplace_back(newBestParse);
    }
    return correctFsmParses;
}

LongestRootFirstDisambiguation::LongestRootFirstDisambiguation() {
    readFromFile("rootList.txt");
}

LongestRootFirstDisambiguation::LongestRootFirstDisambiguation(string fileName) {
    readFromFile(fileName);
}

void LongestRootFirstDisambiguation::readFromFile(string fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName, ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        vector<string> tokens = Word::split(line);
        if (tokens.size() == 2) {
            rootList.insert_or_assign(tokens[0], tokens[1]);
        }
    }
    inputFile.close();
}
