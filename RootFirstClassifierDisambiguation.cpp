//
// Created by olcay on 4.04.2019.
//

#include <FsmMorphologicalAnalyzer.h>
#include "Model.h"
#include "RootFirstClassifierDisambiguation.h"
#include "BinaryAttribute.h"
#include "DataSet.h"
#include "DisambiguatedWord.h"
#include "Dummy.h"
#include "DiscreteFeaturesNotAllowed.h"

/**
 * Constructor for setting the {@link Classifier} and {@link Parameter}.
 *
 * @param classifier Type of the {@link Classifier}.
 * @param parameters {@link Parameter}s of the classifier.
 */
RootFirstClassifierDisambiguation::RootFirstClassifierDisambiguation(Classifier *classifier, Parameter* parameters) {
    this->classifier = classifier;
    this->parameters = parameters;
}

/**
 * The createDataDefinition method creates an {@link ArrayList} of {@link AttributeType}s and adds 2 times BINARY AttributeType
 * for each element of the {@link InflectionalGroup}.
 *
 * @return A new data definition with the attributeTypes.
 */
DataDefinition RootFirstClassifierDisambiguation::createDataDefinition() {
    vector<AttributeType> attributeTypes;
    attributeTypes.reserve(2 * InflectionalGroup::TAG_SIZE);
    for (int i = 0; i < 2 * InflectionalGroup::TAG_SIZE; i++) {
        attributeTypes.emplace_back(AttributeType::BINARY);
    }
    return DataDefinition(attributeTypes);
}

/**
 * The addAttributes method takes an {@link InflectionalGroup} ig and an {@link ArrayList} of attributes. If the given
 * ig contains any of the morphological tags of InflectionalGroup, it adds a new {@link BinaryAttribute} with the value of
 * true to the attributes ArrayList, if not it adds a new {@link BinaryAttribute} with the value of false.
 *
 * @param ig         InflectionalGroup to check the morphological tags.
 * @param attributes ArrayList of attributes.
 */
void RootFirstClassifierDisambiguation::addAttributes(InflectionalGroup ig, vector<Attribute*> attributes) {
    for (int k = 0; k < InflectionalGroup::TAG_SIZE; k++) {
        if (ig.containsTag(InflectionalGroup::morphoTags[k])) {
            attributes.emplace_back(new BinaryAttribute(true));
        } else {
            attributes.emplace_back(new BinaryAttribute(false));
        }
    }
}

/**
 * The classificationProblem method takes a {@link String} input and parses it. If the given {@link MorphologicalParse}
 * contains the parsed String, it directly returns that String, if not it returns null.
 *
 * @param disambiguationProblem String input to be parsed.
 * @param morphologicalParse    MorphologicalParse input.
 * @return If the given {MorphologicalParse contains the String, it directly returns that String, if not it returns null.
 */
string RootFirstClassifierDisambiguation::classificationProblem(string disambiguationProblem,
                                                                MorphologicalParse morphologicalParse) {
    vector<string> parses = Word::split(move(disambiguationProblem), "$");
    for (const auto &parse : parses) {
        if (morphologicalParse.to_String().find(parse) != string::npos) {
            return parse;
        }
    }
    return "";
}

/**
 * The train method gets sentences from given {@link DisambiguationCorpus}, then perform morphological analyses for each
 * word of a sentence and gets a {@link FsmParseList} at each time and removes the other words which are identical to the current word
 * and part of speech tags.
 * <p>
 * If the size of the {@link FsmParseList} greater than 1,  it removes the prefixes and suffixes from the {@link FsmParseList} and
 * evaluates it as disambiguationProblem  String. If this String is already placed in Dataset, it gets its value, else
 * put it to the Dataset as a new key.
 * <p>
 * Apart from that, it also gets two previous InflectionalGroups and finds out their class labels, and adds them to the Dataset
 * as a new {@link Instance}.
 *
 * @param corpus {@link DisambiguationCorpus} to train.
 */
void RootFirstClassifierDisambiguation::train(DisambiguationCorpus corpus) {
    RootFirstDisambiguation::train(corpus);
    int i, j;
    Sentence* sentence;
    Classifier* currentClassifier;
    DataDefinition dataDefinition;
    DataSet dataSet;
    vector<Attribute*> attributes;
    map<string, DataSet> dataSets;
    dataDefinition = createDataDefinition();
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    for (i = 0; i < corpus.sentenceCount(); i++) {
        sentence = corpus.getSentence(i);
        for (j = 2; j < sentence->wordCount(); j++) {
            FsmParseList parseList = fsm.morphologicalAnalysis(sentence->getWord(j)->getName());
            parseList.reduceToParsesWithSameRootAndPos(((DisambiguatedWord*) sentence->getWord(j))->getParse().getWordWithPos());
            if (parseList.size() > 1) {
                string disambiguationProblem = parseList.parsesWithoutPrefixAndSuffix();
                if (dataSets.find(disambiguationProblem) != dataSets.end()) {
                    dataSet = dataSets.find(disambiguationProblem)->second;
                } else {
                    dataSet = DataSet(dataDefinition);
                    dataSets.emplace(disambiguationProblem, dataSet);
                }
                InflectionalGroup previousIg = ((DisambiguatedWord*) sentence->getWord(j - 2))->getParse().lastInflectionalGroup();
                attributes.clear();
                addAttributes(previousIg, attributes);
                InflectionalGroup ig = ((DisambiguatedWord*) sentence->getWord(j - 1))->getParse().lastInflectionalGroup();
                addAttributes(ig, attributes);
                string classLabel = classificationProblem(disambiguationProblem, ((DisambiguatedWord*) sentence->getWord(j))->getParse());
                if (!classLabel.empty()) {
                    dataSet.addInstance(new Instance(classLabel, attributes));
                }
            }
        }
    }
    i = 0;
    for (auto& iterator : dataSets) {
        if (dataSets.find(iterator.first)->second.sampleSize() >= 10) {
            currentClassifier = classifier;
        } else {
            currentClassifier = new Dummy();
        }
        try {
            InstanceList instanceList = dataSets.find(iterator.first)->second.getInstanceList();
            currentClassifier->train(instanceList, parameters);
            models.emplace(iterator.first, currentClassifier->getModel());
        } catch (DiscreteFeaturesNotAllowed& discreteFeaturesNotAllowed) {
        }
    }
}

/**
 * The disambiguate method gets an array of fsmParses. Then loops through these parses and finds the most probable root
 * word and removes the other words which are identical to the most probable root word. For the first two items and
 * the last item, it gets the most probable ig parse among the fsmParses and adds it to the correctFsmParses {@link ArrayList} and returns it.
 * For the other cases, it gets the classification model,  considering the previous two ig it performs a prediction
 * and at the end returns the correctFsmParses that holds the best parses.
 *
 * @param fsmParses {@link FsmParseList} to disambiguate.
 * @return The correctFsmParses that holds the best parses.
 */
vector<FsmParse> RootFirstClassifierDisambiguation::disambiguate(FsmParseList *fsmParses, int size) {
    int i;
    Word* bestWord;
    FsmParse bestParse;
    vector<Attribute*> attributes;
    vector<FsmParse> correctFsmParses;
    for (i = 0; i < size; i++) {
        bestWord = getBestRootWord(fsmParses[i]);
        fsmParses[i].reduceToParsesWithSameRootAndPos(bestWord);
        if (i < 2 || i != correctFsmParses.size()) {
            bestParse = getParseWithBestIgProbability(fsmParses[i], correctFsmParses, i);
        } else {
            if (fsmParses[i].size() == 0) {
                bestParse = FsmParse();
            } else {
                if (fsmParses[i].size() == 1) {
                    bestParse = fsmParses[i].getFsmParse(0);
                } else {
                    string disambiguationProblem = fsmParses[i].parsesWithoutPrefixAndSuffix();
                    if (models.find(disambiguationProblem) != models.end()) {
                        Model* model = models.find(disambiguationProblem)->second;
                        attributes.clear();
                        InflectionalGroup previousIg = correctFsmParses.at(i - 2).lastInflectionalGroup();
                        addAttributes(previousIg, attributes);
                        InflectionalGroup ig = correctFsmParses.at(i - 1).lastInflectionalGroup();
                        addAttributes(ig, attributes);
                        string predictedParse = model->predict(new Instance("", attributes));
                        for (int j = 0; j < fsmParses[i].size(); j++) {
                            if (fsmParses[i].getFsmParse(j).getTransitionList().find(predictedParse) != string::npos) {
                                bestParse = fsmParses[i].getFsmParse(j);
                                break;
                            }
                        }
                        if (bestParse.size() == 0) {
                            bestParse = getParseWithBestIgProbability(fsmParses[i], correctFsmParses, i);
                        }
                    } else {
                        bestParse = getParseWithBestIgProbability(fsmParses[i], correctFsmParses, i);
                    }
                }
            }
        }
        if (bestParse.size() > 0) {
            correctFsmParses.emplace_back(bestParse);
        }
    }
    return correctFsmParses;
}

void RootFirstClassifierDisambiguation::saveModel() {
    RootFirstDisambiguation::saveModel();
    ofstream outputFile;
    outputFile.open("classifiers.bin", ostream::out);
    outputFile << models.size();
    for (auto& model : models){
        outputFile << model.first << "\n";
        model.second->serialize(outputFile);
    }
    outputFile.close();
}

void RootFirstClassifierDisambiguation::loadModel() {
    RootFirstDisambiguation::loadModel();
    ifstream inputFile;
    inputFile.open("classifiers.bin", istream::in);
    int size;
    inputFile >> size;
    for (int i = 0; i < size; i++){
        string modelName;
        inputFile >> modelName;
        Model* model = new Model(inputFile);
        models.emplace(modelName, model);
    }
    inputFile.close();
}
