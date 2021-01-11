//
// Created by olcay on 2019-05-13.
//

#ifndef ANNOTATEDTREE_PARSETREESEARCHABLE_H
#define ANNOTATEDTREE_PARSETREESEARCHABLE_H

#include <ParseTree.h>
#include <XmlElement.h>

class ParseTreeSearchable : public ParseTree{
public:
    explicit ParseTreeSearchable(XmlElement* rootNode);
};


#endif //ANNOTATEDTREE_PARSETREESEARCHABLE_H
