//
// Created by olcay on 2019-05-13.
//

#ifndef ANNOTATEDTREE_PARSETREEDRAWABLE_H
#define ANNOTATEDTREE_PARSETREEDRAWABLE_H

#include <ViewLayerType.h>
#include <AnnotatedSentence.h>
#include "ParseTree.h"
#include "FileDescription.h"
#include "ParseTreeSearchable.h"
#include "ParseNodeDrawable.h"

class ParseTreeDrawable : public ParseTree{
private:
    FileDescription fileDescription = FileDescription("mypath", "0000.train");
    string name;
    void readFromFile(const string& currentPath);
public:
    ParseTreeDrawable(const string& path, const string& rawFileName);
    ParseTreeDrawable(const string& path, const string& extension, int index);
    ParseTreeDrawable(const string& path, FileDescription fileDescription);
    explicit ParseTreeDrawable(istream& inputFile);
    explicit ParseTreeDrawable(string line);
    explicit ParseTreeDrawable(FileDescription fileDescription);
    void setFileDescription(const FileDescription& fileDescription);
    FileDescription getFileDescription();
    void copyInfo(const ParseTreeDrawable& parseTree);
    void reload();
    void setName(string name);
    string getName();
    void nextTree(int count);
    void previousTree(int count);
    void save();
    void saveWithPath(const string& newPath);
    int glossAgreementCount(ParseTree parseTree, ViewLayerType viewLayerType);
    int structureAgreementCount(ParseTree parseTree);
    int maxDepth();
    bool layerExists(ViewLayerType viewLayerType);
    bool layerAll(ViewLayerType viewLayerType);
    void clearLayer(ViewLayerType layerType);
    vector<ParseNodeDrawable*> satisfy(ParseTreeSearchable tree);
    AnnotatedSentence* generateAnnotatedSentence();
};


#endif //ANNOTATEDTREE_PARSETREEDRAWABLE_H
