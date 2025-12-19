//
// Created by Olcay Taner YILDIZ on 24.02.2020.
//

#include "LongestRootFirstDisambiguation.h"
#include "AutoDisambiguator.h"
#include "StringUtils.h"

/**
 * The disambiguate method gets an array of fsmParses. Then loops through that parses and finds the longest root
 * word. At the end, gets the parse with longest word among the fsmParses and adds it to the correctFsmParses
 * ArrayList.
 *
 * @param fsmParses FsmParseList to disambiguate.
 * @param size Size of the fsmParses
 * @return correctFsmParses ArrayList which holds the parses with longest root words.
 */
vector<FsmParse> LongestRootFirstDisambiguation::disambiguate(FsmParseList *fsmParses, int size) const{
    vector<FsmParse> correctFsmParses;
    FsmParse bestParse;
    for (int i = 0; i < size; i++) {
        FsmParseList fsmParseList = fsmParses[i];
        string bestRoot;
        string surfaceForm = fsmParseList.getFsmParse(0).getSurfaceForm();
        if (rootList.contains(surfaceForm)){
            bestRoot = rootList.at(surfaceForm);
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

/**
 * Constructor for the longest root first disambiguation algorithm. The method reads a list of (surface form, most
 * frequent root word for that surface form) pairs from 'rootlist.txt' file.
 */
LongestRootFirstDisambiguation::LongestRootFirstDisambiguation() {
    readFromFile("rootList.txt");
}

/**
 * Constructor for the longest root first disambiguation algorithm. The method reads a list of (surface form, most
 * frequent root word for that surface form) pairs from a given file.
 * @param fileName File that contains list of (surface form, most frequent root word for that surface form) pairs.
 */
LongestRootFirstDisambiguation::LongestRootFirstDisambiguation(const string& fileName) {
    readFromFile(fileName);
}

/**
 * Reads the list of (surface form, most frequent root word for that surface form) pairs from a given file.
 * @param fileName Input file name.
 */
void LongestRootFirstDisambiguation::readFromFile(const string& fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName, ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        vector<string> tokens = StringUtils::split(line);
        if (tokens.size() == 2) {
            rootList.insert_or_assign(tokens[0], tokens[1]);
        }
    }
    inputFile.close();
}
