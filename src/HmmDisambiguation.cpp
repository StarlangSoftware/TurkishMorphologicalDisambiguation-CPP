//
// Created by olcay on 2.04.2019.
//

#include "HmmDisambiguation.h"
#include "DisambiguatedWord.h"
#include "LaplaceSmoothing.h"

/**
 * The train method gets sentences from given DisambiguationCorpus and both word and the next word of that sentence at each iteration.
 * Then, adds these words together with their part of speech tags to word unigram and bigram models. It also adds the last inflectional group of
 * word to the ig unigram and bigram models.
 * <p>
 * At the end, it calculates the NGram probabilities of both word and ig unigram models by using LaplaceSmoothing, and
 * both word and ig bigram models by using InterpolatedSmoothing.
 *
 * @param corpus DisambiguationCorpus to train.
 */
void HmmDisambiguation::train(DisambiguationCorpus& corpus) {
    int i, j, k;
    Sentence* sentence;
    DisambiguatedWord* word, *nextWord;
    LaplaceSmoothing <string> laplaceSmoothing;
    vector<string> words = {"", ""};
    vector<string> igs = {"", ""};
    wordUniGramModel = new NGram<string>(1);
    igUniGramModel = new NGram<string>(1);
    wordBiGramModel = new NGram<string>(2);
    igBiGramModel = new NGram<string>(2);
    for (i = 0; i < corpus.sentenceCount(); i++) {
        sentence = corpus.getSentence(i);
        for (j = 0; j < sentence->wordCount() - 1; j++) {
            word = (DisambiguatedWord*) sentence->getWord(j);
            nextWord = (DisambiguatedWord*) sentence->getWord(j + 1);
            words[0] = word->getParse().getWordWithPos()->getName();
            words[1] = nextWord->getParse().getWordWithPos()->getName();
            wordUniGramModel->addNGram(words, 1);
            wordBiGramModel->addNGram(words, 2);
            for (k = 0; k < nextWord->getParse().size(); k++) {
                igs[0] = word->getParse().getLastInflectionalGroup().to_String();
                igs[1] = nextWord->getParse().getInflectionalGroup(k).to_String();
                igBiGramModel->addNGram(igs, 2);
                igs[0] = igs[1];
                igUniGramModel->addNGram(igs, 1);
            }
        }
    }
    laplaceSmoothing.setProbabilities(*wordUniGramModel);
    laplaceSmoothing.setProbabilities(*igUniGramModel);
    laplaceSmoothing.setProbabilities(*wordBiGramModel);
    laplaceSmoothing.setProbabilities(*igBiGramModel);
}

/**
 * The disambiguate method takes FsmParseList as an input and gets one word with its part of speech tags, then gets its probability
 * from word unigram model. It also gets ig and its probability. Then, hold the logarithmic value of  the product of these probabilities in an array.
 * Also by taking into consideration the parses of these word it recalculates the probabilities and returns these parses.
 *
 * @param fsmParses FsmParseList to disambiguate.
 * @return ArrayList of FsmParses.
 */
vector<FsmParse> HmmDisambiguation::disambiguate(FsmParseList *fsmParses, int size) const{
    int i, j, k, t, bestIndex;
    double probability, bestProbability;
    string w1, w2, ig1, ig2;
    for (i = 0; i < size; i++) {
        if (fsmParses[i].size() == 0) {
            return vector<FsmParse>();
        }
    }
    vector<FsmParse> correctFsmParses;
    auto** probabilities = new double*[size];
    int** best = new int*[size];
    for (i = 0; i < size; i++) {
        probabilities[i] = new double[fsmParses[i].size()];
        best[i] = new int[fsmParses[i].size()];
    }
    for (i = 0; i < fsmParses[0].size(); i++) {
        FsmParse currentParse = fsmParses[0].getFsmParse(i);
        w1 = currentParse.getWordWithPos()->getName();
        probability = wordUniGramModel->getProbability({w1});
        for (j = 0; j < currentParse.size(); j++) {
            ig1 = currentParse.getInflectionalGroup(j).to_String();
            probability *= igUniGramModel->getProbability({ig1});
        }
        probabilities[0][i] = log(probability);
    }
    for (i = 1; i < size; i++) {
        for (j = 0; j < fsmParses[i].size(); j++) {
            bestProbability = -INT_MAX;
            bestIndex = -1;
            FsmParse currentParse = fsmParses[i].getFsmParse(j);
            for (k = 0; k < fsmParses[i - 1].size(); k++) {
                FsmParse previousParse = fsmParses[i - 1].getFsmParse(k);
                w1 = previousParse.getWordWithPos()->getName();
                w2 = currentParse.getWordWithPos()->getName();
                probability = probabilities[i - 1][k] + log(wordBiGramModel->getProbability({w1, w2}));
                for (t = 0; t < fsmParses[i].getFsmParse(j).size(); t++) {
                    ig1 = previousParse.lastInflectionalGroup().to_String();
                    ig2 = currentParse.getInflectionalGroup(t).to_String();
                    probability += log(igBiGramModel->getProbability({ig1, ig2}));
                }
                if (probability > bestProbability) {
                    bestIndex = k;
                    bestProbability = probability;
                }
            }
            probabilities[i][j] = bestProbability;
            best[i][j] = bestIndex;
        }
    }
    bestProbability = -INT_MAX;
    bestIndex = -1;
    for (i = 0; i < fsmParses[size - 1].size(); i++) {
        if (probabilities[size - 1][i] > bestProbability) {
            bestProbability = probabilities[size - 1][i];
            bestIndex = i;
        }
    }
    if (bestIndex == -1) {
        return vector<FsmParse>();
    }
    correctFsmParses.emplace_back(fsmParses[size - 1].getFsmParse(bestIndex));
    for (i = size - 2; i >= 0; i--) {
        bestIndex = best[i + 1][bestIndex];
        if (bestIndex == -1) {
            return vector<FsmParse>();
        }
        correctFsmParses.insert(correctFsmParses.begin(), fsmParses[i].getFsmParse(bestIndex));
    }
    return correctFsmParses;
}

/**
 * Method to load unigrams and bigrams.
 */
void HmmDisambiguation::loadModel() {
    NaiveDisambiguation::loadModel();
    ifstream inputFile;
    inputFile.open("words.2gram", istream::in);
    wordBiGramModel = new NGram<string>(inputFile);
    inputFile.close();
    inputFile.open("igs.2gram", istream::in);
    igBiGramModel = new NGram<string>(inputFile);
    inputFile.close();
}

/**
 * Method to save unigrams and bigrams.
 */
void HmmDisambiguation::saveModel() {
    NaiveDisambiguation::saveModel();
    ofstream outputFile;
    outputFile.open("words.2gram", ostream::out);
    wordBiGramModel->serialize(outputFile);
    outputFile.close();
    outputFile.open("igs.2gram", ostream::out);
    igBiGramModel->serialize(outputFile);
    outputFile.close();
}

HmmDisambiguation::~HmmDisambiguation() {
    delete wordBiGramModel;
    delete igBiGramModel;
}
