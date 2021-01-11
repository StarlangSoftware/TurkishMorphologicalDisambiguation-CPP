//
// Created by Olcay Taner Yıldız on 11.01.2019.
//

#ifndef CORPUS_FILEDESCRIPTION_H
#define CORPUS_FILEDESCRIPTION_H

#include <string>
using namespace std;

class FileDescription {
private:
    string path, extension;
    int index;
public:
    FileDescription(string path, string rawFileName);
    FileDescription(string path, string extension, int index);
    string getPath();
    int getIndex();
    string getExtension();
    string getFileName();
    string getFileNameWithExtension(string extension);
    string getFileName(string thisPath);
    string getFileName(string thisPath, string extension);
    string getFileName(string thisPath, int thisIndex);
    string getFileName(string thisPath, int thisIndex, string extension);
    string getRawFileName();
    void addToIndex(int count);
};


#endif //CORPUS_FILEDESCRIPTION_H
