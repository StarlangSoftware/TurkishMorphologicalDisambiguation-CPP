//
// Created by olcay on 2.04.2019.
//

#include "NaiveDisambiguation.h"

void NaiveDisambiguation::saveModel() {
    ofstream outputFile;
    outputFile.open("words.1gram", ostream::out);
    wordUniGramModel.serialize(outputFile);
    outputFile.close();
    outputFile.open("igs.1gram", ostream::out);
    igUniGramModel.serialize(outputFile);
    outputFile.close();
}

void NaiveDisambiguation::loadModel() {
    ifstream inputFile;
    inputFile.open("words.1gram", istream::in);
    wordUniGramModel = NGram<string>(inputFile);
    inputFile.close();
    inputFile.open("igs.1gram", istream::in);
    igUniGramModel = NGram<string>(inputFile);
    inputFile.close();
}
