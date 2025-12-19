//
// Created by olcay on 4.04.2019.
//

#include "RootFirstDisambiguation.h"
#include "DisambiguatedWord.h"
#include "LaplaceSmoothing.h"

/**
 * The train method initially creates new NGrams; wordUniGramModel, wordBiGramModel, igUniGramModel, and igBiGramModel. It gets the
 * sentences from given corpus and gets each word as a DisambiguatedWord. Then, adds the word together with its part of speech
 * tags to the wordUniGramModel. It also gets the transition list of that word and adds it to the igUniGramModel.
 * <p>
 * If there exists a next word in the sentence, it adds the current and next DisambiguatedWord to the wordBiGramModel with
 * their part of speech tags. It also adds them to the igBiGramModel with their transition lists.
 * <p>
 * At the end, it calculates the NGram probabilities of both word and ig unigram models by using LaplaceSmoothing, and
 * both word and ig bigram models by using InterpolatedSmoothing.
 *
 * @param corpus DisambiguationCorpus to train.
 */
void RootFirstDisambiguation::train(const DisambiguationCorpus& corpus) {
    int i, j;
    Sentence* sentence;
    DisambiguatedWord* word;
    LaplaceSmoothing <string> laplaceSmoothing;
    vector<string> words = {"", ""};
    vector<string> igs = {"", ""};
    wordUniGramModel = new NGram<string>(1);
    wordBiGramModel = new NGram<string>(2);
    igUniGramModel = new NGram<string>(1);
    igBiGramModel = new NGram<string>(2);
    for (i = 0; i < corpus.sentenceCount(); i++) {
        sentence = corpus.getSentence(i);
        for (j = 0; j < sentence->wordCount(); j++) {
            word = (DisambiguatedWord*) sentence->getWord(j);
            words[0] = word->getParse().getWordWithPos()->getName();
            wordUniGramModel->addNGram(words, 1);
            igs[0] = word->getParse().getTransitionList();
            igUniGramModel->addNGram(igs, 1);
            if (j + 1 < sentence->wordCount()) {
                words[1] = ((DisambiguatedWord*) sentence->getWord(j + 1))->getParse().getWordWithPos()->getName();
                wordBiGramModel->addNGram(words, 2);
                igs[1] = ((DisambiguatedWord*) sentence->getWord(j + 1))->getParse().getTransitionList();
                igBiGramModel->addNGram(igs, 2);
            }
        }
    }
    laplaceSmoothing.setProbabilities(*wordUniGramModel);
    laplaceSmoothing.setProbabilities(*igUniGramModel);
    laplaceSmoothing.setProbabilities(*wordBiGramModel);
    laplaceSmoothing.setProbabilities(*igBiGramModel);
}

/**
 * The disambiguate method gets an array of fsmParses. Then loops through that parses and finds the most probable root
 * word and removes the other words which are identical to the most probable root word. At the end, gets the most probable parse
 * among the fsmParses and adds it to the correctFsmParses.
 *
 * @param fsmParses {@link FsmParseList} to disambiguate.
 * @param size Length of the sentence
 * @return correctFsmParses which holds the most probable parses.
 */
vector<FsmParse> RootFirstDisambiguation::disambiguate(FsmParseList *fsmParses, int size) const{
    int i;
    Word* bestWord;
    FsmParse bestParse;
    vector<FsmParse> correctFsmParses;
    for (i = 0; i < size; i++) {
        bestWord = getBestRootWord(fsmParses[i]);
        fsmParses[i].reduceToParsesWithSameRootAndPos(bestWord);
        bestParse = getParseWithBestIgProbability(fsmParses[i], correctFsmParses, i);
        if (bestParse.size() > 0) {
            correctFsmParses.emplace_back(bestParse);
        }
    }
    return correctFsmParses;
}

/**
 * The getWordProbability method returns the probability of a word by using word bigram or unigram model.
 *
 * @param word             Word to find the probability.
 * @param correctFsmParses FsmParse of given word which will be used for getting part of speech tags.
 * @param index            Index of FsmParse of which part of speech tag will be used to get the probability.
 * @return The probability of the given word.
 */
double RootFirstDisambiguation::getWordProbability(const string& word, const vector<FsmParse>& correctFsmParses, int index) const{
    if (index != 0 && correctFsmParses.size() == index) {
        return wordBiGramModel->getProbability({correctFsmParses.at(index - 1).getWordWithPos()->getName(), word});
    } else {
        return wordUniGramModel->getProbability({word});
    }
}

/**
 * The getIgProbability method returns the probability of a word by using ig bigram or unigram model.
 *
 * @param word             Word to find the probability.
 * @param correctFsmParses FsmParse of given word which will be used for getting transition list.
 * @param index            Index of FsmParse of which transition list will be used to get the probability.
 * @return The probability of the given word.
 */
double RootFirstDisambiguation::getIgProbability(const string& word, const vector<FsmParse>& correctFsmParses, int index) const{
    if (index != 0 && correctFsmParses.size() == index) {
        return igBiGramModel->getProbability({correctFsmParses.at(index - 1).getTransitionList(), word});
    } else {
        return igUniGramModel->getProbability({word});
    }
}

/**
 * The getBestRootWord method takes a FsmParseList as an input and loops through the list. It gets each word with its
 * part of speech tags as a new Word word and its transition list as a Word ig. Then, finds their corresponding
 * probabilities. At the end returns the word with the highest probability.
 *
 * @param fsmParseList FsmParseList is used to get the part of speech tags and transition lists of words.
 * @return The word with the highest probability.
 */
Word* RootFirstDisambiguation::getBestRootWord(const FsmParseList& fsmParseList) const{
    double bestProbability, probability;
    Word* bestWord = nullptr;
    bestProbability = -INT_MAX;
    for (int j = 0; j < fsmParseList.size(); j++) {
        string word = fsmParseList.getFsmParse(j).getWordWithPos()->getName();
        string ig = fsmParseList.getFsmParse(j).getTransitionList();
        double wordProbability = wordUniGramModel->getProbability({word});
        double igProbability = igUniGramModel->getProbability({ig});
        probability = wordProbability * igProbability;
        if (probability > bestProbability) {
            bestWord = fsmParseList.getFsmParse(j).getWordWithPos();
            bestProbability = probability;
        }
    }
    return bestWord;
}

/**
 * The getParseWithBestIgProbability gets each FsmParse's transition list as a Word ig. Then, finds the corresponding
 * probabilitt. At the end returns the parse with the highest ig probability.
 *
 * @param parseList        FsmParseList is used to get the FsmParse.
 * @param correctFsmParses FsmParse is used to get the transition lists.
 * @param index            Index of FsmParse of which transition list will be used to get the probability.
 * @return The parse with the highest probability.
 */
FsmParse
RootFirstDisambiguation::getParseWithBestIgProbability(const FsmParseList& parseList, const vector<FsmParse>& correctFsmParses,
                                                       int index) const{
    double bestProbability, probability;
    FsmParse bestParse;
    bestProbability = -INT_MAX;
    for (int j = 0; j < parseList.size(); j++) {
        string ig = parseList.getFsmParse(j).getTransitionList();
        probability = getIgProbability(ig, correctFsmParses, index);
        if (probability > bestProbability) {
            bestParse = parseList.getFsmParse(j);
            bestProbability = probability;
        }
    }
    return bestParse;
}

/**
 * Method to load unigrams and bigrams.
 */
void RootFirstDisambiguation::loadModel() {
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
void RootFirstDisambiguation::saveModel() const {
    NaiveDisambiguation::saveModel();
    ofstream outputFile;
    outputFile.open("words.2gram", ostream::out);
    wordBiGramModel->serialize(outputFile);
    outputFile.close();
    outputFile.open("igs.2gram", ostream::out);
    igBiGramModel->serialize(outputFile);
    outputFile.close();
}

RootFirstDisambiguation::~RootFirstDisambiguation() {
    delete wordBiGramModel;
    delete igBiGramModel;
}
