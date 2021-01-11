//
// Created by Olcay Taner Yıldız on 7.10.2018.
//

#ifndef PROPBANK_FRAMESET_H
#define PROPBANK_FRAMESET_H
#include <string>
#include <vector>
#include "FramesetArgument.h"
#include "ArgumentType.h"
#include "XmlDocument.h"

using namespace std;

class Frameset {
private:
    string id;
    vector<FramesetArgument> framesetArguments;
public:
    explicit Frameset(string id);
    explicit Frameset(XmlDocument xmlDocument);
    bool containsArgument(ArgumentType argumentType);
    void addArgument(string type, string definition, string function);
    void deleteArgument(string type, string definition);
    vector<FramesetArgument> getFramesetArguments();
    string getId();
    void setId(string id);
};


#endif //PROPBANK_FRAMESET_H
