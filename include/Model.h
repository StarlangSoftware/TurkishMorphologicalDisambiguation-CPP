//
// Created by olcay on 6.02.2019.
//

#ifndef CLASSIFICATION_MODEL_H
#define CLASSIFICATION_MODEL_H


#include "Instance.h"

class Model {
public:
    virtual string predict(Instance* instance) = 0;
};


#endif //CLASSIFICATION_MODEL_H
