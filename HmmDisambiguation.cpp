//
// Created by olcay on 2.04.2019.
//

#include "HmmDisambiguation.h"
#include "DisambiguatedWord.h"

/**
 * The train method gets sentences from given {@link DisambiguationCorpus} and both word and the next word of that sentence at each iteration.
 * Then, adds these words together with their part of speech tags to word unigram and bigram models. It also adds the last inflectional group of
 * word to the ig unigram and bigram models.
 * <p>
 * At the end, it calculates the NGram probabilities of both word and ig unigram models by using LaplaceSmoothing, and
 * both word and ig bigram models by using InterpolatedSmoothing.
 *
 * @param corpus {@link DisambiguationCorpus} to train.
 */
void HmmDisambiguation::train(DisambiguationCorpus corpus) {
    int i, j, k;
    Sentence* sentence;
    DisambiguatedWord* word, *nextWord;
    auto* words = new string[2];
    auto* igs = new string[2];
    wordUniGramModel = NGram<string>(1);
    igUniGramModel = NGram<string>(1);
    wordBiGramModel = NGram<string>(2);
    igBiGramModel = NGram<string>(2);
    for (i = 0; i < corpus.sentenceCount(); i++) {
        sentence = corpus.getSentence(i);
        for (j = 0; j < sentence->wordCount() - 1; j++) {
            word = (DisambiguatedWord*) sentence->getWord(j);
            nextWord = (DisambiguatedWord*) sentence->getWord(j + 1);
            words[0] = word->getParse().getWordWithPos()->getName();
            words[1] = nextWord->getParse().getWordWithPos()->getName();
            wordUniGramModel.addNGram(words, 2);
            wordBiGramModel.addNGram(words, 2);
            for (k = 0; k < nextWord->getParse().size(); k++) {
                igs[0] = word->getParse().getLastInflectionalGroup().to_String();
                igs[1] = nextWord->getParse().getInflectionalGroup(k).to_String();
                igBiGramModel.addNGram(igs, 2);
                igs[0] = igs[1];
                igUniGramModel.addNGram(igs,2 );
            }
        }
        }
    }
}
