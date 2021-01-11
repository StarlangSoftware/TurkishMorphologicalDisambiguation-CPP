//
// Created by Olcay Taner YILDIZ on 22.10.2020.
//

#ifndef FRAMENET_FRAMENET_H
#define FRAMENET_FRAMENET_H

#include "Frame.h"

class FrameNet {
private:
    vector<Frame> frames;
public:
    FrameNet();
    bool lexicalUnitExists(string synSetId);
    vector<Frame> getFrames(string synSetId);
    int size();
    Frame getFrame(int index);
};


#endif //FRAMENET_FRAMENET_H
