//
// Created by olcay on 4.04.2019.
//

#ifndef MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTCLASSIFIERDISAMBIGUATION_H
#define MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTCLASSIFIERDISAMBIGUATION_H


#include <Model.h>
#include "Classifier.h"
#include "RootFirstDisambiguation.h"

class RootFirstClassifierDisambiguation : public RootFirstDisambiguation {
private:
    map<string, Model*> models;
    Classifier* classifier;
    Parameter* parameters;
    DataDefinition createDataDefinition();
    void addAttributes(InflectionalGroup ig, vector<Attribute*> attributes);
    string classificationProblem(string disambiguationProblem, MorphologicalParse morphologicalParse);
public:
    RootFirstClassifierDisambiguation(Classifier* classifier, Parameter* parameters);
    void train(DisambiguationCorpus corpus) override;
    vector<FsmParse> disambiguate(FsmParseList* fsmParses, int size) override;
};


#endif //MORPHOLOGICALDISAMBIGUATION_ROOTFIRSTCLASSIFIERDISAMBIGUATION_H
