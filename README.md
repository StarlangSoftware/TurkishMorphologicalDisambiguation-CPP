For Developers
============

You can also see [Java](https://github.com/starlangsoftware/TurkishMorphologicalDisambiguation), [Python](https://github.com/starlangsoftware/TurkishMorphologicalDisambiguation-Py), [Cython](https://github.com/starlangsoftware/TurkishMorphologicalDisambiguation-Cy), or [C#](https://github.com/starlangsoftware/TurkishMorphologicalDisambiguation-CS) repository.

## Requirements

* [C++ Compiler](#cpp)
* [Git](#git)


### CPP
To check if you have compatible C++ Compiler installed,
* Open CLion IDE 
* Preferences >Build,Execution,Deployment > Toolchain  

### Git

Install the [latest version of Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).

## Download Code

In order to work on code, create a fork from GitHub page. 
Use Git for cloning the code to your local or below line for Ubuntu:

	git clone <your-fork-git-link>

A directory called TurkishMorphologicalDisambiguation-CPP will be created. Or you can use below link for exploring the code:

	git clone https://github.com/starlangsoftware/TurkishMorphologicalDisambiguation-CPP.git

## Open project with CLion IDE

To import projects from Git with version control:

* Open CLion IDE , select Get From Version Control.

* In the Import window, click URL tab and paste github URL.

* Click open as Project.

Result: The imported project is listed in the Project Explorer view and files are loaded.


## Compile

**From IDE**

After being done with the downloading and opening project, select **Build Project** option from **Build** menu.

Detailed Description
============

+ [Creating MorphologicalDisambiguator](#creating-morphologicaldisambiguator)
+ [Training MorphologicalDisambiguator](#training-morphologicaldisambiguator)
+ [Sentence Disambiguation](#sentence-disambiguation)

## Creating MorphologicalDisambiguator 

MorphologicalDisambiguator provides Turkish morphological disambiguation. There are possible disambiguation techniques. Depending on the technique used, disambiguator can be instantiated as follows:

* Using `RootFirstDisambiguation`, the one that chooses only the root amongst the given analyses

        MorphologicalDisambiguator morphologicalDisambiguator = new RootFirstDisambiguation();

* Using `LongestRootFirstDisambiguation`, the one that chooses the root that is the most frequently used amongst the given analyses

        MorphologicalDisambiguator morphologicalDisambiguator = new LongestRootFirstDisambiguation();

* Using `HmmDisambiguation`, the one that chooses using an Hmm-based algorithm 
        
        MorphologicalDisambiguator morphologicalDisambiguator = new HmmDisambiguation();

* Using `DummyDisambiguation`, the one that chooses a random one amongst the given analyses
     
        MorphologicalDisambiguator morphologicalDisambiguator = new DummyDisambiguation();
    

## Training MorphologicalDisambiguator

To train the disambiguator, an instance of `DisambiguationCorpus` object is needed. This can be instantiated and the disambiguator can be trained and saved as follows:

    DisambiguationCorpus corpus = new DisambiguationCorpus("penn_treebank.txt");
    morphologicalDisambiguator.train(corpus);
    morphologicalDisambiguator.saveModel();
    
      
## Sentence Disambiguation

To disambiguate a sentence, a `FsmMorphologicalAnalyzer` instance is required. This can be created as below, further information can be found [here](https://github.com/starlangsoftware/MorphologicalAnalysis/blob/master/README.md#creating-fsmmorphologicalanalyzer).

    FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer();
    
A sentence can be disambiguated as follows: 
    
    Sentence sentence = new Sentence("Yarın doktora gidecekler");
    FsmParseList[] fsmParseList = fsm.robustMorphologicalAnalysis(sentence);
    System.out.println("All parses");
    System.out.println("--------------------------");
    for(int i = 0; i < fsmParseList.length; i++){
        System.out.println(fsmParseList[i]);
    }
    ArrayList<FsmParse> candidateParses = morphologicalDisambiguator.disambiguate(fsmParseList);
    System.out.println("Parses after disambiguation");
    System.out.println("--------------------------");
    for(int i = 0; i < candidateParses.size(); i++){
        System.out.println(candidateParses.get(i));
    }

Output

    
    All parses
    --------------------------
    yar+NOUN+A3SG+P2SG+NOM
    yar+NOUN+A3SG+PNON+GEN
    yar+VERB+POS+IMP+A2PL
    yarı+NOUN+A3SG+P2SG+NOM
    yarın+NOUN+A3SG+PNON+NOM
    
    doktor+NOUN+A3SG+PNON+DAT
    doktora+NOUN+A3SG+PNON+NOM
    
    git+VERB+POS+FUT+A3PL
    git+VERB+POS^DB+NOUN+FUTPART+A3PL+PNON+NOM
    
    Parses after disambiguation
    --------------------------
    yarın+NOUN+A3SG+PNON+NOM
    doktor+NOUN+A3SG+PNON+DAT
    git+VERB+POS+FUT+A3PL
