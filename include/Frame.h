//
// Created by Olcay Taner YILDIZ on 22.10.2020.
//

#ifndef FRAMENET_FRAME_H
#define FRAMENET_FRAME_H
#include <string>
#include <vector>
#include <XmlElement.h>
#include <XmlDocument.h>

using namespace std;

class Frame {
private:
    string name;
    vector<string> lexicalUnits;
    vector<string> frameElements;
public:
    explicit Frame(string name);
    bool lexicalUnitExists(string synSetId);
    void addLexicalUnit(string lexicalUnit);
    void addFrameElement(string frameElement);
    string getLexicalUnit(int index);
    string getFrameElement(int index);
    int lexicalUnitSize();
    int frameElementSize();
    string getName();
};


#endif //FRAMENET_FRAME_H
