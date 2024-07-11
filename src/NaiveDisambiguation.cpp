//
// Created by olcay on 2.04.2019.
//

#include "NaiveDisambiguation.h"

/**
 * The saveModel method writes the specified objects i.e wordUniGramModel and igUniGramModel to the
 * words1.txt and igs1.txt.
 */
void NaiveDisambiguation::saveModel() {
    ofstream outputFile;
    outputFile.open("words.1gram", ostream::out);
    wordUniGramModel->serialize(outputFile);
    outputFile.close();
    outputFile.open("igs.1gram", ostream::out);
    igUniGramModel->serialize(outputFile);
    outputFile.close();
}

/**
 * The loadModel method reads objects at the words1.txt and igs1.txt to the wordUniGramModel and igUniGramModel.
 */
void NaiveDisambiguation::loadModel() {
    ifstream inputFile;
    inputFile.open("words.1gram", istream::in);
    wordUniGramModel = new NGram<string>(inputFile);
    inputFile.close();
    inputFile.open("igs.1gram", istream::in);
    igUniGramModel = new NGram<string>(inputFile);
    inputFile.close();
}

NaiveDisambiguation::~NaiveDisambiguation() {
    delete wordUniGramModel;
    delete igUniGramModel;
}
