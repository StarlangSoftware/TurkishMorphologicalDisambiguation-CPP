//
// Created by Olcay Taner Yıldız on 2019-04-24.
//

#ifndef ANNOTATEDSENTENCE_ANNOTATEDWORD_H
#define ANNOTATEDSENTENCE_ANNOTATEDWORD_H

#include <Word.h>
#include <MorphologicalParse.h>
#include <MetamorphicParse.h>
#include <NamedEntityType.h>
#include <Argument.h>
#include <FsmParse.h>
#include <UniversalDependencyRelation.h>
#include <FrameElement.h>
#include "ViewLayerType.h"
#include "Gazetteer.h"

class AnnotatedWord : public Word{
private:
    MorphologicalParse* parse = nullptr;
    MetamorphicParse* metamorphicParse = nullptr;
    string semantic;
    NamedEntityType* namedEntityType = nullptr;
    Argument* argument = nullptr;
    FrameElement* frameElement = nullptr;
    string shallowParse;
    UniversalDependencyRelation* universalDependency = nullptr;
public:
    ~AnnotatedWord() override;
    explicit AnnotatedWord(string word);
    AnnotatedWord(string name, NamedEntityType* namedEntityType);
    string to_string();
    AnnotatedWord(string name, MorphologicalParse* parse);
    AnnotatedWord(string name, FsmParse* parse);
    string getLayerInfo(ViewLayerType viewLayerType);
    MorphologicalParse* getParse();
    void setParse(string parseString);
    MetamorphicParse* getMetamorphicParse();
    void setMetamorphicParse(string parseString);
    string getSemantic();
    void setSemantic(string semantic);
    NamedEntityType* getNamedEntity();
    void setNamedEntityType(string namedEntity);
    Argument* getArgument();
    void setArgument(string argument);
    FrameElement* getFrameElement();
    void setFrameElement(string frameElement);
    string getShallowParse();
    void setShallowParse(string parse);
    UniversalDependencyRelation* getUniversalDependency();
    void setUniversalDependency(int to, string dependencyType);
    void checkGazetteer(Gazetteer gazetteer);
};


#endif //ANNOTATEDSENTENCE_ANNOTATEDWORD_H
