//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#include "RootWordStatistics.h"

/**
 * Constructor of {@link RootWordStatistics} class that generates a new {@link HashMap} for statistics.
 */
RootWordStatistics::RootWordStatistics() = default;

/**
 * Method to check whether statistics contains the given String.
 *
 * @param key String to look for.
 * @return Returns <tt>true</tt> if this map contains a mapping for the specified key.
 */
bool RootWordStatistics::containsKey(string key) {
    return statistics.find(key) != statistics.end();
}

/**
 * Method to get the value of the given String.
 *
 * @param key String to look for.
 * @return Returns the value to which the specified key is mapped, or {@code null} if this map contains no mapping for the key.
 */
CounterHashMap<string> RootWordStatistics::get(string key) {
    return statistics.find(key)->second;
}

/**
 * Method to associates a String along with a {@link CounterHashMap} in the statistics.
 *
 * @param key            Key with which the specified value is to be associated.
 * @param wordStatistics Value to be associated with the specified key.
 */
void RootWordStatistics::put(string key, CounterHashMap<string> wordStatistics) {
    statistics.insert_or_assign(key, wordStatistics);
}

/**
 * The bestRootWord method gets the root word of given {@link FsmParseList} and if statistics has a value for that word,
 * it returns the max value associated with that word.
 *
 * @param parseList {@link FsmParseList} to check.
 * @param threshold A double value for limit.
 * @return The max value for the root word.
 */
string RootWordStatistics::bestRootWord(FsmParseList parseList, double threshold) {
    string surfaceForm = parseList.getFsmParse(0).getSurfaceForm();
    if (containsKey(surfaceForm)) {
        CounterHashMap<string> rootWordStatistics = statistics.find(surfaceForm)->second;
        string rootWord = rootWordStatistics.max(threshold);
        for (int i = 0; i < parseList.size(); i++){
            if (parseList.getFsmParse(i).getWord()->getName() == rootWord){
                return rootWord;
            }
        }
    }
    return "";
}

void RootWordStatistics::serialize(ostream& outputFile) {
    outputFile << statistics.size();
    for (auto& iterator : statistics){
        outputFile << iterator.first;
        iterator.second.serialize(outputFile);
    }
}

RootWordStatistics::RootWordStatistics(ifstream &inputFile) {
    int size;
    inputFile >> size;
    for (int i = 0; i < size; i++){
        string item;
        inputFile >> item;
        CounterHashMap<string> rootWordStatistics = CounterHashMap<string>(inputFile);
        statistics.emplace(item, rootWordStatistics);
    }
}

RootWordStatistics::RootWordStatistics(string fileName) {
    ifstream inputFile;
    inputFile.open(fileName, ifstream::in);
    int size;
    inputFile >> size;
    for (int i = 0; i < size; i++){
        string item;
        inputFile >> item;
        CounterHashMap<string> rootWordStatistics = CounterHashMap<string>(inputFile);
        statistics.emplace(item, rootWordStatistics);
    }
    inputFile.close();
}
