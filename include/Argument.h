//
// Created by Olcay Taner Yıldız on 4.10.2018.
//

#ifndef PROPBANK_ARGUMENT_H
#define PROPBANK_ARGUMENT_H
#include <string>

using namespace std;

class Argument {
private:
    string argumentType;
    string id;
public:
    Argument(string argument);
    Argument(string argumentType, string id);
    string getArgumentType();
    string getId();
    string to_string();
};


#endif //PROPBANK_ARGUMENT_H
