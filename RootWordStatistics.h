//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_ROOTWORDSTATISTICS_H
#define MORPHOLOGICALDISAMBIGUATION_ROOTWORDSTATISTICS_H

#include "CounterHashMap.h"
#include <map>
#include <FsmParseList.h>

using  namespace std;

class RootWordStatistics {
private:
    map<string, CounterHashMap<string>> statistics;
public:
    explicit RootWordStatistics(ifstream& inputFile);
    RootWordStatistics();
    bool containsKey(string key);
    CounterHashMap<string> get(string key);
    void put(string key, CounterHashMap<string> wordStatistics);
    string bestRootWord(FsmParseList parseList, double threshold);
    void serialize(ostream& outputFile);
};


#endif //MORPHOLOGICALDISAMBIGUATION_ROOTWORDSTATISTICS_H
