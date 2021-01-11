//
// Created by olcay on 14.03.2019.
//

#ifndef DEPENDENCYPARSER_DEPENDENCYRELATION_H
#define DEPENDENCYPARSER_DEPENDENCYRELATION_H


class DependencyRelation {
protected:
    int toWord;
    explicit DependencyRelation(int toWord);
public:
    DependencyRelation() = default;
    int to();
};


#endif //DEPENDENCYPARSER_DEPENDENCYRELATION_H
