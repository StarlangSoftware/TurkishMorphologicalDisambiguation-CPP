//
// Created by LMF319018 on 16/01/2019.
//

#ifndef NAMEDENTITYRECOGNITION_NAMEDENTITYTYPE_H
#define NAMEDENTITYRECOGNITION_NAMEDENTITYTYPE_H
#include <string>

using namespace std;

/**
 * Enumerator class for Named Entity types.
 */
enum class NamedEntityType{
    NONE, PERSON, ORGANIZATION, LOCATION, TIME, MONEY
};

/**
 * Static function to convert a string entity type to {@link NamedEntityType} type.
 * @param entityType Entity type in string form
 * @return Entity type in {@link NamedEntityType} form
 */
static NamedEntityType getNamedEntityType(const string entityType){
    if (entityType.empty()){
        return NamedEntityType::NONE;
    } else {
        if (entityType == "PERSON"){
            return NamedEntityType::PERSON;
        } else {
            if (entityType == "LOCATION"){
                return NamedEntityType::LOCATION;
            } else {
                if (entityType == "ORGANIZATION"){
                    return NamedEntityType::ORGANIZATION;
                } else {
                    if (entityType == "TIME"){
                        return NamedEntityType::TIME;
                    } else {
                        if (entityType == "MONEY"){
                            return NamedEntityType::MONEY;
                        } else {
                            return NamedEntityType::NONE;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Static function to convert a {@link NamedEntityType} to string form.
 * @param entityType Entity type in {@link NamedEntityType} form
 * @return Entity type in string form
 */
static string getNamedEntityType(NamedEntityType entityType){
    switch (entityType){
        case NamedEntityType::PERSON:
            return "PERSON";
        case NamedEntityType::LOCATION:
            return "LOCATION";
        case NamedEntityType::ORGANIZATION:
            return "ORGANIZATION";
        case NamedEntityType::TIME:
            return "TIME";
        case NamedEntityType::MONEY:
            return "MONEY";
        default:
            return "NONE";
    }
}

#endif //NAMEDENTITYRECOGNITION_NAMEDENTITYTYPE_H
