//
// Created by Olcay Taner Yıldız on 4.10.2018.
//

#ifndef PROPBANK_ARGUMENTTYPE_H
#define PROPBANK_ARGUMENTTYPE_H
#include <string>

using namespace std;

/**
 * Enumerated class for argument type.
 */
enum class ArgumentType {
    NONE, PREDICATE, ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARGMNONE, ARGMEXT, ARGMLOC, ARGMDIS, ARGMADV, ARGMCAU, ARGMTMP, ARGMPNC, ARGMMNR, ARGMDIR
};

/**
 * The getArguments method takes an argumentsType string and returns the {@link ArgumentType} form of it.
 *
 * @param argumentsType  Type of the argument in string form
 * @return Type of the argument in {@link ArgumentType} form
 */
static ArgumentType getArguments(string argumentsType){
    std::transform(argumentsType.begin(), argumentsType.end(), argumentsType.begin(), [](unsigned char c){ return std::toupper(c); });
    if (argumentsType.empty()){
        return ArgumentType::NONE;
    } else {
        if (argumentsType == "ARG0"){
            return ArgumentType::ARG0;
        } else {
            if (argumentsType == "ARG1"){
                return ArgumentType::ARG1;
            } else {
                if (argumentsType == "ARG2"){
                    return ArgumentType::ARG2;
                } else {
                    if (argumentsType == "ARG3"){
                        return ArgumentType::ARG3;
                    } else {
                        if (argumentsType == "ARG4"){
                            return ArgumentType::ARG4;
                        } else{
                            if(argumentsType == "ARG5"){
                                return ArgumentType::ARG5;
                            }else {
                                if (argumentsType == "ARGMADV"){
                                    return ArgumentType::ARGMADV;
                                } else{
                                    if (argumentsType == "ARGMCAU"){
                                        return ArgumentType::ARGMCAU;
                                    } else{
                                        if (argumentsType == "ARGMDIR"){
                                            return ArgumentType::ARGMDIR;
                                        } else{
                                            if (argumentsType == "ARGMDIS"){
                                                return ArgumentType::ARGMDIS;
                                            } else{
                                                if (argumentsType == "ARGMEXT"){
                                                    return ArgumentType::ARGMEXT;
                                                } else{
                                                    if (argumentsType == "ARGMLOC"){
                                                        return ArgumentType::ARGMLOC;
                                                    } else{
                                                        if (argumentsType == "ARGMMNR"){
                                                            return ArgumentType::ARGMMNR;
                                                        } else{
                                                            if (argumentsType == "ARGMTMP"){
                                                                return ArgumentType::ARGMTMP;
                                                            } else{
                                                                if (argumentsType == "ARGMNONE"){
                                                                    return ArgumentType::ARGMNONE;
                                                                } else{
                                                                    if (argumentsType == "ARGMPNC"){
                                                                        return ArgumentType::ARGMPNC;
                                                                    } else{
                                                                        if (argumentsType =="PREDICATE"){
                                                                            return ArgumentType::PREDICATE;
                                                                        } else {
                                                                            return ArgumentType::NONE;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * The getPropbankType method takes an argumentType in {@link ArgumentType} form and returns the string form of it.
 *
 * @param argumentType  Type of the argument in {@link ArgumentType} form
 * @return Type of the argument in string form
 */
static string getPropbankType(ArgumentType argumentType){
    switch (argumentType){
        case ArgumentType::ARG0:
            return "ARG0";
        case ArgumentType::ARG1:
            return "ARG1";
        case ArgumentType::ARG2:
            return "ARG2";
        case ArgumentType::ARG3:
            return "ARG3";
        case ArgumentType::ARG4:
            return "ARG4";
        case ArgumentType::ARG5:
            return "ARG5";
        case ArgumentType::ARGMADV:
            return "ARGMADV";
        case ArgumentType::ARGMCAU:
            return "ARGMCAU";
        case ArgumentType::ARGMDIR:
            return "ARGMDIR";
        case ArgumentType::ARGMDIS:
            return "ARGMDIS";
        case ArgumentType::ARGMEXT:
            return "ARGMEXT";
        case ArgumentType::ARGMLOC:
            return "ARGMLOC";
        case ArgumentType::ARGMMNR:
            return "ARGMMNR";
        case ArgumentType::ARGMTMP:
            return "ARGMTMP";
        case ArgumentType::ARGMNONE:
            return "ARGMNONE";
        case ArgumentType::ARGMPNC:
            return "ARGMPNC";
        case ArgumentType::PREDICATE:
            return "PREDICATE";
        default:
            return "NONE";
    }
}

#endif //PROPBANK_ARGUMENTTYPE_H
