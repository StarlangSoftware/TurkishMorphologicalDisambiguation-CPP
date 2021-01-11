//
// Created by LMF319018 on 16/01/2019.
//

#ifndef NAMEDENTITYRECOGNITION_AUTONER_H
#define NAMEDENTITYRECOGNITION_AUTONER_H


#include "Gazetteer.h"

/**
 * Constructor for creating Person, Organization, and Location gazetteers in automatic Named Entity Recognition.
 */
class AutoNER {
protected:
    Gazetteer personGazetteer = Gazetteer("PERSON", "gazetteer-person.txt");;
    Gazetteer organizationGazetteer = Gazetteer("ORGANIZATION", "gazetteer-organization.txt");
    Gazetteer locationGazetteer = Gazetteer("LOCATION", "gazetteer-location.txt");
};


#endif //NAMEDENTITYRECOGNITION_AUTONER_H
