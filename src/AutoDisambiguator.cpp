//
// Created by Olcay Taner YILDIZ on 2019-04-01.
//

#include "AutoDisambiguator.h"
#include "Dictionary/Word.h"
#include "CounterHashMap.h"

bool AutoDisambiguator::isAnyWordSecondPerson(int index, vector<FsmParse> correctParses) {
    int count = 0;
    for (int i = index - 1; i >= 0; i--) {
        if (correctParses[i].containsTag(MorphologicalTag::A2SG) || correctParses[i].containsTag(MorphologicalTag::P2SG)) {
            count++;
        }
    }
    return count >= 1;
}

bool AutoDisambiguator::isPossessivePlural(int index, vector<FsmParse> correctParses) {
    for (int i = index - 1; i >= 0; i--) {
        if (correctParses[i].isNoun()) {
            return correctParses[i].isPlural();
        }
    }
    return false;
}

string AutoDisambiguator::nextWordPos(FsmParseList nextParseList) {
    CounterHashMap<string> map = CounterHashMap<string>();
    for (int i = 0; i < nextParseList.size(); i++) {
        map.put(nextParseList.getFsmParse(i).getPos());
    }
    return map.max();
}

bool AutoDisambiguator::isBeforeLastWord(int index, int length) {
    return index + 2 == length;
}

bool AutoDisambiguator::nextWordExists(int index, int length) {
    return index + 1 < length;
}

bool AutoDisambiguator::isNextWordNoun(int index, FsmParseList *fsmParses, int length) {
    return index + 1 < length && nextWordPos(fsmParses[index + 1]) == "NOUN";
}

bool AutoDisambiguator::isNextWordNum(int index, FsmParseList *fsmParses, int length) {
    return index + 1 < length && nextWordPos(fsmParses[index + 1]) == "NUM";
}

bool AutoDisambiguator::isNextWordNounOrAdjective(int index, FsmParseList *fsmParses, int length) {
    return index + 1 < length && (nextWordPos(fsmParses[index + 1]) == "NOUN" || nextWordPos(fsmParses[index + 1]) == "ADJ" || nextWordPos(fsmParses[index + 1]) == "DET");
}

bool AutoDisambiguator::isFirstWord(int index) {
    return index == 0;
}

bool AutoDisambiguator::containsTwoNeOrYa(FsmParseList *fsmParses, string word, int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        string surfaceForm = fsmParses[i].getFsmParse(0).getSurfaceForm();
        if (surfaceForm == word) {
            count++;
        }
    }
    return count == 2;
}

bool AutoDisambiguator::hasPreviousWordTag(int index, vector<FsmParse> correctParses, MorphologicalTag tag) {
    return index > 0 && correctParses.at(index - 1).containsTag(tag);
}

string AutoDisambiguator::selectCaseForParseString(string parseString, int index, FsmParseList *fsmParses,
                                                   vector<FsmParse> correctParses, int length) {
    string surfaceForm = fsmParses[index].getFsmParse(0).getSurfaceForm();
    string root = fsmParses[index].getFsmParse(0).getWord()->getName();
    string lastWord = fsmParses[length - 1].getFsmParse(0).getSurfaceForm();
    if (parseString == "P2SG$P3SG") {
        /* kısmını, duracağını, grubunun */
        if (isAnyWordSecondPerson(index, correctParses)) {
            return "P2SG";
        }
        return "P3SG";
    } else {
        if (parseString == "A2SG+P2SG$A3SG+P3SG") {
            if (isAnyWordSecondPerson(index, correctParses)) {
                return "A2SG+P2SG";
            }
            return "A3SG+P3SG";
        } else {
            /* BİR */
            if (parseString == "ADJ$ADV$DET$NUM+CARD") {
                return "DET";
            } else {
                /* tahminleri, işleri, hisseleri */
                if (parseString == "A3PL+P3PL+NOM$A3PL+P3SG+NOM$A3PL+PNON+ACC$A3SG+P3PL+NOM") {
                    if (isPossessivePlural(index, correctParses)) {
                        return "A3SG+P3PL+NOM";
                    }
                    return "A3PL+P3SG+NOM";
                } else {
                    /* Ocak, Cuma, ABD */
                    if (parseString == "A3SG$PROP+A3SG") {
                        if (index > 0) {
                            return "PROP+A3SG";
                        }
                    } else {
                        /* şirketin, seçimlerin, borsacıların, kitapların */
                        if (parseString == "P2SG+NOM$PNON+GEN") {
                            if (isAnyWordSecondPerson(index, correctParses)) {
                                return "P2SG+NOM";
                            }
                            return "PNON+GEN";
                        } else {
                            /* ÇOK */ /* FAZLA */
                            if (parseString == "ADJ$ADV$DET$POSTP+PCABL" || parseString == "ADJ$ADV$POSTP+PCABL") {
                                if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
                                    return "POSTP+PCABL";
                                }
                                if (index + 1 < length) {
                                    if (nextWordPos(fsmParses[index + 1]) == "NOUN"){
                                        return "ADJ";
                                    } else {
                                        if (nextWordPos(fsmParses[index + 1]) == "ADJ" || nextWordPos(fsmParses[index + 1]) == "ADV" || nextWordPos(fsmParses[index + 1]) == "VERB"){
                                            return "ADV";
                                        }
                                    }
                                }
                            } else {
                                if (parseString == "ADJ$NOUN+A3SG+PNON+NOM") {
                                    if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                        return "ADJ";
                                    }
                                    return "NOUN+A3SG+PNON+NOM";
                                } else {
                                    /* fanatiklerini, senetlerini, olduklarını */
                                    if (parseString == "A3PL+P2SG$A3PL+P3PL$A3PL+P3SG$A3SG+P3PL") {
                                        if (isAnyWordSecondPerson(index, correctParses)) {
                                            return "A3PL+P2SG";
                                        }
                                        if (isPossessivePlural(index, correctParses)) {
                                            return "A3SG+P3PL";
                                        } else {
                                            return "A3PL+P3SG";
                                        }
                                    } else {
                                        if (parseString == "ADJ$NOUN+PROP+A3SG+PNON+NOM") {
                                            if (index > 0) {
                                                return "NOUN+PROP+A3SG+PNON+NOM";
                                            }
                                        } else {
                                            /* BU, ŞU */
                                            if (parseString == "DET$PRON+DEMONSP+A3SG+PNON+NOM") {
                                                if (isNextWordNoun(index, fsmParses, length)) {
                                                    return "DET";
                                                }
                                                return "PRON+DEMONSP+A3SG+PNON+NOM";
                                            } else {
                                                /* gelebilir */
                                                if (parseString == "AOR+A3SG$AOR^DB+ADJ+ZERO") {
                                                    if (isBeforeLastWord(index, length)) {
                                                        return "AOR+A3SG";
                                                    } else if (isFirstWord(index)) {
                                                        return "AOR^DB+ADJ+ZERO";
                                                    } else {
                                                        if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                            return "AOR^DB+ADJ+ZERO";
                                                        } else {
                                                            return "AOR+A3SG";
                                                        }
                                                    }
                                                } else {
                                                    if (parseString == "ADV$NOUN+A3SG+PNON+NOM") {
                                                        return "ADV";
                                                    } else {
                                                        if (parseString == "ADJ$ADV") {
                                                            if (isNextWordNoun(index, fsmParses, length)) {
                                                                return "ADJ";
                                                            }
                                                            return "ADV";
                                                        } else {
                                                            if (parseString == "P2SG$PNON") {
                                                                if (isAnyWordSecondPerson(index, correctParses)) {
                                                                    return "P2SG";
                                                                }
                                                                return "PNON";
                                                            } else {
                                                                /* etti, kırdı */
                                                                if (parseString == "NOUN+A3SG+PNON+NOM^DB+VERB+ZERO$VERB+POS") {
                                                                    if (isBeforeLastWord(index, length)) {
                                                                        return "VERB+POS";
                                                                    }
                                                                } else {
                                                                    /* İLE */
                                                                    if (parseString == "CONJ$POSTP+PCNOM") {
                                                                        return "POSTP+PCNOM";
                                                                    } else {
                                                                        /* gelecek */
                                                                        if (parseString == "POS+FUT+A3SG$POS^DB+ADJ+FUTPART+PNON") {
                                                                            if (isBeforeLastWord(index, length)) {
                                                                                return "POS+FUT+A3SG";
                                                                            }
                                                                            return "POS^DB+ADJ+FUTPART+PNON";
                                                                        } else {
                                                                            if (parseString == "ADJ^DB$NOUN+A3SG+PNON+NOM^DB") {
                                                                                if (root == "yok" || root == "düşük" || root == "eksik" || root == "rahat" || root == "orta" || root == "vasat") {
                                                                                    return "ADJ^DB";
                                                                                }
                                                                                return "NOUN+A3SG+PNON+NOM^DB";
                                                                            } else {
                                                                                /* yaptık, şüphelendik */
                                                                                if (parseString == "POS+PAST+A1PL$POS^DB+ADJ+PASTPART+PNON$POS^DB+NOUN+PASTPART+A3SG+PNON+NOM") {
                                                                                    return "POS+PAST+A1PL";
                                                                                } else {
                                                                                    /* ederim, yaparım */
                                                                                    if (parseString == "AOR+A1SG$AOR^DB+ADJ+ZERO^DB+NOUN+ZERO+A3SG+P1SG+NOM") {
                                                                                        return "AOR+A1SG";
                                                                                    } else {
                                                                                        /* geçti, vardı, aldı */
                                                                                        if (parseString == "ADJ^DB+VERB+ZERO$VERB+POS") {
                                                                                            if (root == "var" && !isPossessivePlural(index, correctParses)) {
                                                                                                return "ADJ^DB+VERB+ZERO";
                                                                                            }
                                                                                            return "VERB+POS";
                                                                                        } else {
                                                                                            /* ancak */
                                                                                            if (parseString == "ADV$CONJ") {
                                                                                                return "CONJ";
                                                                                            } else {
                                                                                                /* yaptığı, ettiği */
                                                                                                if (parseString == "ADJ+PASTPART+P3SG$NOUN+PASTPART+A3SG+P3SG+NOM") {
                                                                                                    if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                                                                        return "ADJ+PASTPART+P3SG";
                                                                                                    }
                                                                                                    return "NOUN+PASTPART+A3SG+P3SG+NOM";
                                                                                                } else {
                                                                                                    /* ÖNCE, SONRA */
                                                                                                    if (parseString == "ADV$NOUN+A3SG+PNON+NOM$POSTP+PCABL") {
                                                                                                        if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
                                                                                                            return "POSTP+PCABL";
                                                                                                        }
                                                                                                        return "ADV";
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
                        }
                    }
                }
            }
        }
    }
    if (parseString == "NARR+A3SG$NARR^DB+ADJ+ZERO") {
        if (isBeforeLastWord(index, length)) {
            return "NARR+A3SG";
        }
        return "NARR^DB+ADJ+ZERO";
    } else {
        if (parseString == "ADJ$NOUN+A3SG+PNON+NOM$NOUN+PROP+A3SG+PNON+NOM") {
            if (index > 0) {
                return "NOUN+PROP+A3SG+PNON+NOM";
            } else {
                if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                    return "ADJ";
                }
                return "NOUN+A3SG+PNON+NOM";
            }
        } else {
            /* ödediğim */
            if (parseString == "ADJ+PASTPART+P1SG$NOUN+PASTPART+A3SG+P1SG+NOM") {
                if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                    return "ADJ+PASTPART+P1SG";
                }
                return "NOUN+PASTPART+A3SG+P1SG+NOM";
            } else {
                /* O */
                if (parseString == "DET$PRON+DEMONSP+A3SG+PNON+NOM$PRON+PERS+A3SG+PNON+NOM") {
                    if (isNextWordNoun(index, fsmParses, length)) {
                        return "DET";
                    }
                    return "PRON+PERS+A3SG+PNON+NOM";
                } else {
                    /* BAZI */
                    if (parseString == "ADJ$DET$PRON+QUANTP+A3SG+P3SG+NOM") {
                        return "DET";
                    } else {
                        /* ONUN, ONA, ONDAN, ONUNLA, OYDU, ONUNKİ */
                        if (parseString == "DEMONSP$PERS") {
                            return "PERS";
                        } else {
                            if (parseString == "ADJ$NOUN+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                    return "ADJ";
                                }
                                return "NOUN+A3SG+PNON+NOM";
                            } else {
                                /* hazineler, kıymetler */
                                if (parseString == "A3PL+PNON+NOM$A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL$PROP+A3PL+PNON+NOM") {
                                    if (index > 0) {
                                        if (fsmParses[index].getFsmParse(0).isCapitalWord()) {
                                            return "PROP+A3PL+PNON+NOM";
                                        }
                                        return "A3PL+PNON+NOM";
                                    }
                                } else {
                                    /* ARTIK, GERİ */
                                    if (parseString == "ADJ$ADV$NOUN+A3SG+PNON+NOM") {
                                        if (root == "artık") {
                                            return "ADV";
                                        } else if (isNextWordNoun(index, fsmParses, length)) {
                                            return "ADJ";
                                        }
                                        return "ADV";
                                    } else {
                                        if (parseString == "P1SG+NOM$PNON+NOM^DB+VERB+ZERO+PRES+A1SG") {
                                            if (isBeforeLastWord(index, length) || root == "değil") {
                                                return "PNON+NOM^DB+VERB+ZERO+PRES+A1SG";
                                            }
                                            return "P1SG+NOM";
                                        } else {
                                            /* görülmektedir */
                                            if (parseString == "POS+PROG2$POS^DB+NOUN+INF+A3SG+PNON+LOC^DB+VERB+ZERO+PRES") {
                                                return "POS+PROG2";
                                            } else {
                                                /* NE */
                                                if (parseString == "ADJ$ADV$CONJ$PRON+QUESP+A3SG+PNON+NOM") {
                                                    if (lastWord == "?") {
                                                        return "PRON+QUESP+A3SG+PNON+NOM";
                                                    }
                                                    if (containsTwoNeOrYa(fsmParses, "ne", length)) {
                                                        return "CONJ";
                                                    }
                                                    if (isNextWordNoun(index, fsmParses, length)) {
                                                        return "ADJ";
                                                    }
                                                    return "ADV";
                                                } else {
                                                    /* TÜM */
                                                    if (parseString == "DET$NOUN+A3SG+PNON+NOM") {
                                                        return "DET";
                                                    } else {
                                                        /* AZ */
                                                        if (parseString == "ADJ$ADV$POSTP+PCABL$VERB+POS+IMP+A2SG") {
                                                            if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
                                                                return "POSTP+PCABL";
                                                            }
                                                            if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                                return "ADJ";
                                                            }
                                                            return "ADV";
                                                        } else {
                                                            /* görülmedik */
                                                            if (parseString == "NEG+PAST+A1PL$NEG^DB+ADJ+PASTPART+PNON$NEG^DB+NOUN+PASTPART+A3SG+PNON+NOM") {
                                                                if (surfaceForm == "alışılmadık") {
                                                                    return "NEG^DB+ADJ+PASTPART+PNON";
                                                                }
                                                                return "NEG+PAST+A1PL";
                                                            } else {
                                                                if (parseString == "DATE$NUM+FRACTION") {
                                                                    return "NUM+FRACTION";
                                                                } else {
                                                                    /* giriş, satış, öpüş, vuruş */
                                                                    if (parseString == "POS^DB+NOUN+INF3+A3SG+PNON+NOM$RECIP+POS+IMP+A2SG") {
                                                                        return "POS^DB+NOUN+INF3+A3SG+PNON+NOM";
                                                                    } else {
                                                                        /* başka, yukarı */
                                                                        if (parseString == "ADJ$POSTP+PCABL") {
                                                                            if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
                                                                                return "POSTP+PCABL";
                                                                            }
                                                                            return "ADJ";
                                                                        } else {
                                                                            /* KARŞI */
                                                                            if (parseString == "ADJ$ADV$NOUN+A3SG+PNON+NOM$POSTP+PCDAT") {
                                                                                if (hasPreviousWordTag(index, correctParses, MorphologicalTag::DATIVE)) {
                                                                                    return "POSTP+PCDAT";
                                                                                }
                                                                                if (isNextWordNoun(index, fsmParses, length)) {
                                                                                    return "ADJ";
                                                                                }
                                                                                return "ADV";
                                                                            } else {
                                                                                /* BEN */
                                                                                if (parseString == "NOUN+A3SG$NOUN+PROP+A3SG$PRON+PERS+A1SG") {
                                                                                    return "PRON+PERS+A1SG";
                                                                                } else {
                                                                                    /* yapıcı, verici */
                                                                                    if (parseString == "ADJ+AGT$NOUN+AGT+A3SG+PNON+NOM") {
                                                                                        if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                                                            return "ADJ+AGT";
                                                                                        }
                                                                                        return "NOUN+AGT+A3SG+PNON+NOM";
                                                                                    } else {
                                                                                        /* BİLE */
                                                                                        if (parseString == "ADV$VERB+POS+IMP+A2SG") {
                                                                                            return "ADV";
                                                                                        } else {
                                                                                            /* ortalamalar, uzaylılar, demokratlar */
                                                                                            if (parseString == "NOUN+ZERO+A3PL+PNON+NOM$VERB+ZERO+PRES+A3PL") {
                                                                                                return "NOUN+ZERO+A3PL+PNON+NOM";
                                                                                            } else {
                                                                                                /* yasa, diye, yıla */
                                                                                                if (parseString == "NOUN+A3SG+PNON+DAT$VERB+POS+OPT+A3SG") {
                                                                                                    return "NOUN+A3SG+PNON+DAT";
                                                                                                } else {
                                                                                                    /* BİZ, BİZE */
                                                                                                    if (parseString == "NOUN+A3SG$PRON+PERS+A1PL") {
                                                                                                        return "PRON+PERS+A1PL";
                                                                                                    } else {
                                                                                                        /* AZDI */
                                                                                                        if (parseString == "ADJ^DB+VERB+ZERO$POSTP+PCABL^DB+VERB+ZERO$VERB+POS") {
                                                                                                            return "ADJ^DB+VERB+ZERO";
                                                                                                        } else {
                                                                                                            /* BİRİNCİ, İKİNCİ, ÜÇÜNCÜ, DÖRDÜNCÜ, BEŞİNCİ */
                                                                                                            if (parseString == "ADJ$NUM+ORD") {
                                                                                                                return "ADJ";
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* AY */
    if (parseString == "INTERJ$NOUN+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
        return "NOUN+A3SG+PNON+NOM";
    } else {
        /* konuşmam, savunmam, etmem */
        if (parseString == "NEG+AOR+A1SG$POS^DB+NOUN+INF2+A3SG+P1SG+NOM") {
            return "NEG+AOR+A1SG";
        } else {
            /* YA */
            if (parseString == "CONJ$INTERJ") {
                if (containsTwoNeOrYa(fsmParses, "ya", length)) {
                    return "CONJ";
                }
                if (nextWordExists(index, length) && fsmParses[index + 1].getFsmParse(0).getSurfaceForm() == "da") {
                    return "CONJ";
                }
                return "INTERJ";
            } else {
                if (parseString == "A3PL+P3PL$A3PL+P3SG$A3SG+P3PL") {
                    if (isPossessivePlural(index, correctParses)) {
                        return "A3SG+P3PL";
                    }
                    return "A3PL+P3SG";
                } else {
                    /* YÜZDE, YÜZLÜ */
                    if (parseString == "NOUN$NUM+CARD^DB+NOUN+ZERO") {
                        return "NOUN";
                    } else {
                        /* almanlar, uzmanlar, elmaslar, katiller */
                        if (parseString == "ADJ^DB+VERB+ZERO+PRES+A3PL$NOUN+A3PL+PNON+NOM$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL") {
                            return "NOUN+A3PL+PNON+NOM";
                        } else {
                            /* fazlası, yetkilisi */
                            if (parseString == "ADJ+JUSTLIKE$NOUN+ZERO+A3SG+P3SG+NOM") {
                                return "NOUN+ZERO+A3SG+P3SG+NOM";
                            } else {
                                /* HERKES, HERKESTEN, HERKESLE, HERKES */
                                if (parseString == "NOUN+A3SG+PNON$PRON+QUANTP+A3PL+P3PL") {
                                    return "PRON+QUANTP+A3PL+P3PL";
                                } else {
                                    /* BEN, BENDEN, BENCE, BANA, BENDE */
                                    if (parseString == "NOUN+A3SG$PRON+PERS+A1SG") {
                                        return "PRON+PERS+A1SG";
                                    } else {
                                        /* karşısından, geriye, geride */
                                        if (parseString == "ADJ^DB+NOUN+ZERO$NOUN") {
                                            return "ADJ^DB+NOUN+ZERO";
                                        } else {
                                            /* gideceği, kalacağı */
                                            if (parseString == "ADJ+FUTPART+P3SG$NOUN+FUTPART+A3SG+P3SG+NOM") {
                                                if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                    return "ADJ+FUTPART+P3SG";
                                                }
                                                return "NOUN+FUTPART+A3SG+P3SG+NOM";
                                            } else {
                                                /* bildiğimiz, geçtiğimiz, yaşadığımız */
                                                if (parseString == "ADJ+PASTPART+P1PL$NOUN+PASTPART+A3SG+P1PL+NOM") {
                                                    return "ADJ+PASTPART+P1PL";
                                                } else {
                                                    /* eminim, memnunum, açım */
                                                    if (parseString == "NOUN+ZERO+A3SG+P1SG+NOM$VERB+ZERO+PRES+A1SG") {
                                                        return "VERB+ZERO+PRES+A1SG";
                                                    } else {
                                                        /* yaparlar, olabilirler, değiştirirler */
                                                        if (parseString == "AOR+A3PL$AOR^DB+ADJ+ZERO^DB+NOUN+ZERO+A3PL+PNON+NOM") {
                                                            return "AOR+A3PL";
                                                        } else {
                                                            /* san, yasa */
                                                            if (parseString == "NOUN+A3SG+PNON+NOM$NOUN+PROP+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                                                if (index > 0) {
                                                                    return "NOUN+PROP+A3SG+PNON+NOM";
                                                                }
                                                            } else {
                                                                /* etmeyecek, yapmayacak, koşmayacak */
                                                                if (parseString == "NEG+FUT+A3SG$NEG^DB+ADJ+FUTPART+PNON") {
                                                                    return "NEG+FUT+A3SG";
                                                                } else {
                                                                    /* etmeli, olmalı */
                                                                    if (parseString == "POS+NECES+A3SG$POS^DB+NOUN+INF2+A3SG+PNON+NOM^DB+ADJ+WITH") {
                                                                        if (isBeforeLastWord(index, length)) {
                                                                            return "POS+NECES+A3SG";
                                                                        }
                                                                        if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                                            return "POS^DB+NOUN+INF2+A3SG+PNON+NOM^DB+ADJ+WITH";
                                                                        }
                                                                        return "POS+NECES+A3SG";
                                                                    } else {
                                                                        /* DE */
                                                                        if (parseString == "CONJ$NOUN+PROP+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                                                            if (index > 0) {
                                                                                return "NOUN+PROP+A3SG+PNON+NOM";
                                                                            }
                                                                        } else {
                                                                            /* GEÇ, SIK */
                                                                            if (parseString == "ADJ$ADV$VERB+POS+IMP+A2SG") {
                                                                                if (surfaceForm == "sık") {
                                                                                    string previousWord, nextWord;
                                                                                    if (index - 1 > -1) {
                                                                                        previousWord = fsmParses[index - 1].getFsmParse(0).getSurfaceForm();
                                                                                    }
                                                                                    if (index + 1 < length) {
                                                                                        nextWord = fsmParses[index + 1].getFsmParse(0).getSurfaceForm();
                                                                                    }
                                                                                    if (previousWord == "sık" || nextWord == "sık") {
                                                                                        return "ADV";
                                                                                    }
                                                                                }
                                                                                if (isNextWordNoun(index, fsmParses, length)) {
                                                                                    return "ADJ";
                                                                                }
                                                                                return "ADV";
                                                                            } else {
                                                                                /* BİRLİKTE */
                                                                                if (parseString == "ADV$POSTP+PCINS") {
                                                                                    if (hasPreviousWordTag(index, correctParses, MorphologicalTag::INSTRUMENTAL)) {
                                                                                        return "POSTP+PCINS";
                                                                                    }
                                                                                    return "ADV";
                                                                                } else {
                                                                                    /* yavaşça, dürüstçe, fazlaca */
                                                                                    if (parseString == "ADJ+ASIF$ADV+LY$NOUN+ZERO+A3SG+PNON+EQU") {
                                                                                        return "ADV+LY";
                                                                                    } else {
                                                                                        /* FAZLADIR, FAZLAYDI, ÇOKTU, ÇOKTUR */
                                                                                        if (parseString == "ADJ^DB$POSTP+PCABL^DB") {
                                                                                            if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
                                                                                                return "POSTP+PCABL^DB";
                                                                                            }
                                                                                            return "ADJ^DB";
                                                                                        } else {
                                                                                            /* kaybettikleri, umdukları, gösterdikleri */
                                                                                            if (parseString == "ADJ+PASTPART+P3PL$NOUN+PASTPART+A3PL+P3PL+NOM$NOUN+PASTPART+A3PL+P3SG+NOM$NOUN+PASTPART+A3SG+P3PL+NOM") {
                                                                                                if (isNextWordNounOrAdjective(index, fsmParses, length)) {
                                                                                                    return "ADJ+PASTPART+P3PL";
                                                                                                }
                                                                                                if (isPossessivePlural(index, correctParses)) {
                                                                                                    return "NOUN+PASTPART+A3SG+P3PL+NOM";
                                                                                                }
                                                                                                return "NOUN+PASTPART+A3PL+P3SG+NOM";
                                                                                            } else {
                                                                                                /* yılın, yolun */
                                                                                                if (parseString == "NOUN+A3SG+P2SG+NOM$NOUN+A3SG+PNON+GEN$VERB+POS+IMP+A2PL$VERB^DB+VERB+PASS+POS+IMP+A2SG") {
                                                                                                    if (isAnyWordSecondPerson(index, correctParses)) {
                                                                                                        return "NOUN+A3SG+P2SG+NOM";
                                                                                                    }
                                                                                                    return "NOUN+A3SG+PNON+GEN";
                                                                                                } else {
                                                                                                    /* sürmekte, beklenmekte, değişmekte */
                                                                                                    if (parseString == "POS+PROG2+A3SG$POS^DB+NOUN+INF+A3SG+PNON+LOC") {
                                                                                                        return "POS+PROG2+A3SG";
                                                                                                    } else {
                                                                                                        /* KİMSE, KİMSEDE, KİMSEYE */
                                                                                                        if (parseString == "NOUN+A3SG+PNON$PRON+QUANTP+A3SG+P3SG") {
                                                                                                            return "PRON+QUANTP+A3SG+P3SG";
                                                                                                        } else {
                                                                                                            /* DOĞRU */
                                                                                                            if (parseString == "ADJ$NOUN+A3SG+PNON+NOM$POSTP+PCDAT") {
                                                                                                                if (hasPreviousWordTag(index, correctParses, MorphologicalTag::DATIVE)) {
                                                                                                                    return "POSTP+PCDAT";
                                                                                                                }
                                                                                                                return "ADJ";
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* ikisini, ikisine, fazlasına */
    if (parseString == "ADJ+JUSTLIKE^DB+NOUN+ZERO+A3SG+P2SG$NOUN+ZERO+A3SG+P3SG") {
        return "NOUN+ZERO+A3SG+P3SG";
    } else {
        /* kişilerdir, aylardır, yıllardır */
        if (parseString == "A3PL+PNON+NOM^DB+ADV+SINCE$A3PL+PNON+NOM^DB+VERB+ZERO+PRES+COP+A3SG$A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL+COP") {
            if (root == "yıl" || root == "süre" || root == "zaman" || root == "ay") {
                return "A3PL+PNON+NOM^DB+ADV+SINCE";
            } else {
                return "A3PL+PNON+NOM^DB+VERB+ZERO+PRES+COP+A3SG";
            }
        } else {
            /* HEP */
            if (parseString == "ADV$PRON+QUANTP+A3SG+P3SG+NOM") {
                return "ADV";
            } else {
                /* O */
                if (parseString == "DET$NOUN+PROP+A3SG+PNON+NOM$PRON+DEMONSP+A3SG+PNON+NOM$PRON+PERS+A3SG+PNON+NOM") {
                    if (isNextWordNoun(index, fsmParses, length)){
                        return "DET";
                    } else {
                        return "PRON+PERS+A3SG+PNON+NOM";
                    }
                } else {
                    /* yapmalıyız, etmeliyiz, alınmalıdır */
                    if (parseString == "POS+NECES$POS^DB+NOUN+INF2+A3SG+PNON+NOM^DB+ADJ+WITH^DB+VERB+ZERO+PRES") {
                        return "POS+NECES";
                    } else {
                        /* kızdı, çekti, bozdu */
                        if (parseString == "ADJ^DB+VERB+ZERO$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO$VERB+POS") {
                            return "VERB+POS";
                        } else {
                            /* BİZİMLE */
                            if (parseString == "NOUN+A3SG+P1SG$PRON+PERS+A1PL+PNON") {
                                return "PRON+PERS+A1PL+PNON";
                            } else {
                                /* VARDIR */
                                if (parseString == "ADJ^DB+VERB+ZERO+PRES+COP+A3SG$VERB^DB+VERB+CAUS+POS+IMP+A2SG") {
                                    return "ADJ^DB+VERB+ZERO+PRES+COP+A3SG";
                                } else {
                                    /* Mİ */
                                    if (parseString == "NOUN+A3SG+PNON+NOM$QUES+PRES+A3SG") {
                                        return "QUES+PRES+A3SG";
                                    } else {
                                        /* BENİM */
                                        if (parseString == "NOUN+A3SG+P1SG+NOM$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A1SG$PRON+PERS+A1SG+PNON+GEN$PRON+PERS+A1SG+PNON+NOM^DB+VERB+ZERO+PRES+A1SG") {
                                            return "PRON+PERS+A1SG+PNON+GEN";
                                        } else {
                                            /* SUN */
                                            if (parseString == "NOUN+PROP+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                                return "NOUN+PROP+A3SG+PNON+NOM";
                                            } else {
                                                if (parseString == "ADJ+JUSTLIKE$NOUN+ZERO+A3SG+P3SG+NOM$NOUN+ZERO^DB+ADJ+ALMOST") {
                                                    return "NOUN+ZERO+A3SG+P3SG+NOM";
                                                } else {
                                                    /* düşündük, ettik, kazandık */
                                                    if (parseString == "NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PAST+A1PL$VERB+POS+PAST+A1PL$VERB+POS^DB+ADJ+PASTPART+PNON$VERB+POS^DB+NOUN+PASTPART+A3SG+PNON+NOM") {
                                                        return "VERB+POS+PAST+A1PL";
                                                    } else {
                                                        /* komiktir, eksiktir, mevcuttur, yoktur */
                                                        if (parseString == "ADJ^DB+VERB+ZERO+PRES+COP+A3SG$NOUN+A3SG+PNON+NOM^DB+ADV+SINCE$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+COP+A3SG") {
                                                            return "ADJ^DB+VERB+ZERO+PRES+COP+A3SG";
                                                        } else {
                                                            /* edeceğim, ekeceğim, koşacağım, gideceğim, savaşacağım, olacağım  */
                                                            if (parseString == "POS+FUT+A1SG$POS^DB+ADJ+FUTPART+P1SG$POS^DB+NOUN+FUTPART+A3SG+P1SG+NOM") {
                                                                return "POS+FUT+A1SG";
                                                            } else {
                                                                /* A */
                                                                if (parseString == "ADJ$INTERJ$NOUN+PROP+A3SG+PNON+NOM") {
                                                                    return "NOUN+PROP+A3SG+PNON+NOM";
                                                                } else {
                                                                    /* BİZİ */
                                                                    if (parseString == "NOUN+A3SG+P3SG+NOM$NOUN+A3SG+PNON+ACC$PRON+PERS+A1PL+PNON+ACC") {
                                                                        return "PRON+PERS+A1PL+PNON+ACC";
                                                                    } else {
                                                                        /* BİZİM */
                                                                        if (parseString == "NOUN+A3SG+P1SG+NOM$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A1SG$PRON+PERS+A1PL+PNON+GEN$PRON+PERS+A1PL+PNON+NOM^DB+VERB+ZERO+PRES+A1SG") {
                                                                            return "PRON+PERS+A1PL+PNON+GEN";
                                                                        } else {
                                                                            /* erkekler, kadınlar, madenler, uzmanlar*/
                                                                            if (parseString == "ADJ^DB+VERB+ZERO+PRES+A3PL$NOUN+A3PL+PNON+NOM$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL$NOUN+PROP+A3PL+PNON+NOM") {
                                                                                return "NOUN+A3PL+PNON+NOM";
                                                                            } else {
                                                                                /* TABİ */
                                                                                if (parseString == "ADJ$INTERJ") {
                                                                                    return "ADJ";
                                                                                } else {
                                                                                    if (parseString == "AOR+A2PL$AOR^DB+ADJ+ZERO^DB+ADJ+JUSTLIKE^DB+NOUN+ZERO+A3SG+P2PL+NOM") {
                                                                                        return "AOR+A2PL";
                                                                                    } else {
                                                                                        /* ayın, düşünün*/
                                                                                        if (parseString == "NOUN+A3SG+P2SG+NOM$NOUN+A3SG+PNON+GEN$VERB+POS+IMP+A2PL") {
                                                                                            if (isBeforeLastWord(index, length)){
                                                                                                return "VERB+POS+IMP+A2PL";
                                                                                            }
                                                                                            return "NOUN+A3SG+PNON+GEN";
                                                                                        } else {
                                                                                            /* ödeyecekler, olacaklar */
                                                                                            if (parseString == "POS+FUT+A3PL$POS^DB+NOUN+FUTPART+A3PL+PNON+NOM") {
                                                                                                return "POS+FUT+A3PL";
                                                                                            } else {
                                                                                                /* 9:30'daki */
                                                                                                if (parseString == "P3SG$PNON") {
                                                                                                    return "PNON";
                                                                                                } else {
                                                                                                    /* olabilecek, yapabilecek */
                                                                                                    if (parseString == "ABLE+FUT+A3SG$ABLE^DB+ADJ+FUTPART+PNON") {
                                                                                                        if (isNextWordNounOrAdjective(index, fsmParses, length)){
                                                                                                            return "ABLE^DB+ADJ+FUTPART+PNON";
                                                                                                        }
                                                                                                        return "ABLE+FUT+A3SG";
                                                                                                    } else {
                                                                                                        /* düşmüş duymuş artmış */
                                                                                                        if (parseString == "NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+NARR+A3SG$VERB+POS+NARR+A3SG$VERB+POS+NARR^DB+ADJ+ZERO") {
                                                                                                            if (isBeforeLastWord(index, length)){
                                                                                                                return "VERB+POS+NARR+A3SG";
                                                                                                            }
                                                                                                            return "VERB+POS+NARR^DB+ADJ+ZERO";
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
                            }
                        }
                    }
                }
            }
        }
    }
    /* BERİ, DIŞARI, AŞAĞI */
    if (parseString == "ADJ$ADV$NOUN+A3SG+PNON+NOM$POSTP+PCABL") {
        if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
            return "POSTP+PCABL";
        }
        return "ADV";
    } else {
        /* TV, CD */
        if (parseString == "A3SG+PNON+ACC$PROP+A3SG+PNON+NOM") {
            return "A3SG+PNON+ACC";
        } else {
            /* değinmeyeceğim, vermeyeceğim */
            if (parseString == "NEG+FUT+A1SG$NEG^DB+ADJ+FUTPART+P1SG$NEG^DB+NOUN+FUTPART+A3SG+P1SG+NOM") {
                return "NEG+FUT+A1SG";
            } else {
                /* görünüşe, satışa, duruşa */
                if (parseString == "POS^DB+NOUN+INF3+A3SG+PNON+DAT$RECIP+POS+OPT+A3SG") {
                    return "POS^DB+NOUN+INF3+A3SG+PNON+DAT";
                } else {
                    /* YILDIR, AYDIR, YOLDUR */
                    if (parseString == "NOUN+A3SG+PNON+NOM^DB+ADV+SINCE$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+COP+A3SG$VERB^DB+VERB+CAUS+POS+IMP+A2SG") {
                        if (root == "yıl" || root == "ay") {
                            return "NOUN+A3SG+PNON+NOM^DB+ADV+SINCE";
                        } else {
                            return "NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+COP+A3SG";
                        }
                    } else {
                        /* BENİ */
                        if (parseString == "NOUN+A3SG+P3SG+NOM$NOUN+A3SG+PNON+ACC$PRON+PERS+A1SG+PNON+ACC") {
                            return "PRON+PERS+A1SG+PNON+ACC";
                        } else {
                            /* edemezsin, kanıtlarsın, yapamazsın */
                            if (parseString == "AOR+A2SG$AOR^DB+ADJ+ZERO^DB+ADJ+JUSTLIKE^DB+NOUN+ZERO+A3SG+P2SG+NOM") {
                                return "AOR+A2SG";
                            } else {
                                /* BÜYÜME, ATAMA, KARIMA, KORUMA, TANIMA, ÜREME */
                                if (parseString == "NOUN+A3SG+P1SG+DAT$VERB+NEG+IMP+A2SG$VERB+POS^DB+NOUN+INF2+A3SG+PNON+NOM") {
                                    if (root == "karı"){
                                        return "NOUN+A3SG+P1SG+DAT";
                                    }
                                    return "VERB+POS^DB+NOUN+INF2+A3SG+PNON+NOM";
                                } else {
                                    /* HANGİ */
                                    if (parseString == "ADJ$PRON+QUESP+A3SG+PNON+NOM") {
                                        if (lastWord == "?") {
                                            return "PRON+QUESP+A3SG+PNON+NOM";
                                        }
                                        return "ADJ";
                                    } else {
                                        /* GÜCÜNÜ, GÜCÜNÜN, ESASINDA */
                                        if (parseString == "ADJ^DB+NOUN+ZERO+A3SG+P2SG$ADJ^DB+NOUN+ZERO+A3SG+P3SG$NOUN+A3SG+P2SG$NOUN+A3SG+P3SG") {
                                            return "NOUN+A3SG+P3SG";
                                        } else {
                                            /* YILININ, YOLUNUN, DİLİNİN */
                                            if (parseString == "NOUN+A3SG+P2SG+GEN$NOUN+A3SG+P3SG+GEN$VERB^DB+VERB+PASS+POS+IMP+A2PL") {
                                                return "NOUN+A3SG+P3SG+GEN";
                                            } else {
                                                /* ÇIKARDI */
                                                if (parseString == "VERB+POS+AOR$VERB^DB+VERB+CAUS+POS") {
                                                    return "VERB+POS+AOR";
                                                } else {
                                                    /* sunucularımız, rakiplerimiz, yayınlarımız */
                                                    if (parseString == "P1PL+NOM$P1SG+NOM^DB+VERB+ZERO+PRES+A1PL") {
                                                        return "P1PL+NOM";
                                                    } else {
                                                        /* etmiştir, artmıştır, düşünmüştür, alınmıştır */
                                                        if (parseString == "NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+NARR+A3SG+COP$VERB+POS+NARR+COP+A3SG") {
                                                            return "VERB+POS+NARR+COP+A3SG";
                                                        } else {
                                                            /* hazırlandı, yuvarlandı, temizlendi */
                                                            if (parseString == "VERB+REFLEX$VERB^DB+VERB+PASS") {
                                                                return "VERB^DB+VERB+PASS";
                                                            } else {
                                                                /* KARA, ÇEK, SOL, KOCA */
                                                                if (parseString == "ADJ$NOUN+A3SG+PNON+NOM$NOUN+PROP+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                                                    if (index > 0) {
                                                                        if (fsmParses[index].getFsmParse(0).isCapitalWord()) {
                                                                            return "NOUN+PROP+A3SG+PNON+NOM";
                                                                        }
                                                                        return "ADJ";
                                                                    }
                                                                } else {
                                                                    /* YÜZ */
                                                                    if (parseString == "NOUN+A3SG+PNON+NOM$NUM+CARD$VERB+POS+IMP+A2SG") {
                                                                        if (isNextWordNum(index, fsmParses, length)){
                                                                            return "NUM+CARD";
                                                                        }
                                                                        return "NOUN+A3SG+PNON+NOM";
                                                                    } else {
                                                                        if (parseString == "ADJ+AGT^DB+ADJ+JUSTLIKE$NOUN+AGT+A3SG+P3SG+NOM$NOUN+AGT^DB+ADJ+ALMOST") {
                                                                            return "NOUN+AGT+A3SG+P3SG+NOM";
                                                                        } else {
                                                                            /* artışın, düşüşün, yükselişin*/
                                                                            if (parseString == "POS^DB+NOUN+INF3+A3SG+P2SG+NOM$POS^DB+NOUN+INF3+A3SG+PNON+GEN$RECIP+POS+IMP+A2PL") {
                                                                                if (isAnyWordSecondPerson(index, correctParses)){
                                                                                    return "POS^DB+NOUN+INF3+A3SG+P2SG+NOM";
                                                                                }
                                                                                return "POS^DB+NOUN+INF3+A3SG+PNON+GEN";
                                                                            } else {
                                                                                /* VARSA */
                                                                                if (parseString == "ADJ^DB+VERB+ZERO+COND$VERB+POS+DESR") {
                                                                                    return "ADJ^DB+VERB+ZERO+COND";
                                                                                } else {
                                                                                    /* DEK */
                                                                                    if (parseString == "NOUN+A3SG+PNON+NOM$POSTP+PCDAT") {
                                                                                        return "POSTP+PCDAT";
                                                                                    } else {
                                                                                        /* ALDIK */
                                                                                        if (parseString == "ADJ^DB+VERB+ZERO+PAST+A1PL$VERB+POS+PAST+A1PL$VERB+POS^DB+ADJ+PASTPART+PNON$VERB+POS^DB+NOUN+PASTPART+A3SG+PNON+NOM") {
                                                                                            return "VERB+POS+PAST+A1PL";
                                                                                        } else {
                                                                                            /* BİRİNİN, BİRİNE, BİRİNİ, BİRİNDEN */
                                                                                            if (parseString == "ADJ^DB+NOUN+ZERO+A3SG+P2SG$ADJ^DB+NOUN+ZERO+A3SG+P3SG$NUM+CARD^DB+NOUN+ZERO+A3SG+P2SG$NUM+CARD^DB+NOUN+ZERO+A3SG+P3SG") {
                                                                                                return "NUM+CARD^DB+NOUN+ZERO+A3SG+P3SG";
                                                                                            } else {
                                                                                                /* ARTIK */
                                                                                                if (parseString == "ADJ$ADV$NOUN+A3SG+PNON+NOM$NOUN+PROP+A3SG+PNON+NOM") {
                                                                                                    return "ADV";
                                                                                                } else {
                                                                                                    /* BİRİ */
                                                                                                    if (parseString == "ADJ^DB+NOUN+ZERO+A3SG+P3SG+NOM$ADJ^DB+NOUN+ZERO+A3SG+PNON+ACC$NUM+CARD^DB+NOUN+ZERO+A3SG+P3SG+NOM$NUM+CARD^DB+NOUN+ZERO+A3SG+PNON+ACC") {
                                                                                                        return "NUM+CARD^DB+NOUN+ZERO+A3SG+P3SG+NOM";
                                                                                                    } else {
                                                                                                        /* DOĞRU */
                                                                                                        if (parseString == "ADJ$NOUN+A3SG+PNON+NOM$NOUN+PROP+A3SG+PNON+NOM$POSTP+PCDAT") {
                                                                                                            if (hasPreviousWordTag(index, correctParses, MorphologicalTag::DATIVE)) {
                                                                                                                return "POSTP+PCDAT";
                                                                                                            }
                                                                                                            return "ADJ";
                                                                                                        } else {
                                                                                                            /* demiryolları, havayolları, milletvekilleri */
                                                                                                            if (parseString == "P3PL+NOM$P3SG+NOM$PNON+ACC") {
                                                                                                                if (isPossessivePlural(index, correctParses)){
                                                                                                                    return "P3PL+NOM";
                                                                                                                }
                                                                                                                return "P3SG+NOM";
                                                                                                            } else {
                                                                                                                /* GEREK */
                                                                                                                if (parseString == "CONJ$NOUN+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                                                                                                    if (containsTwoNeOrYa(fsmParses, "gerek", length)){
                                                                                                                        return "CONJ";
                                                                                                                    }
                                                                                                                    return "NOUN+A3SG+PNON+NOM";
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
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* bilmediğiniz, sevdiğiniz, kazandığınız */
    if (parseString == "ADJ+PASTPART+P2PL$NOUN+PASTPART+A3SG+P2PL+NOM$NOUN+PASTPART+A3SG+PNON+GEN^DB+VERB+ZERO+PRES+A1PL") {
        if (isNextWordNounOrAdjective(index, fsmParses, length)){
            return "ADJ+PASTPART+P2PL";
        }
        return "NOUN+PASTPART+A3SG+P2PL+NOM";
    } else {
        /* yapabilecekleri, edebilecekleri, sunabilecekleri */
        if (parseString == "ADJ+FUTPART+P3PL$NOUN+FUTPART+A3PL+P3PL+NOM$NOUN+FUTPART+A3PL+P3SG+NOM$NOUN+FUTPART+A3PL+PNON+ACC$NOUN+FUTPART+A3SG+P3PL+NOM") {
            if (isNextWordNounOrAdjective(index, fsmParses, length)){
                return "ADJ+FUTPART+P3PL";
            }
            if (isPossessivePlural(index, correctParses)){
                return "NOUN+FUTPART+A3SG+P3PL+NOM";
            }
            return "NOUN+FUTPART+A3PL+P3SG+NOM";
        } else {
            /* KİM */
            if (parseString == "NOUN+PROP$PRON+QUESP") {
                if (lastWord == "?") {
                    return "PRON+QUESP";
                }
                return "NOUN+PROP";
            } else {
                /* ALINDI */
                if (parseString == "ADJ^DB+NOUN+ZERO+A3SG+P2SG+NOM^DB+VERB+ZERO$ADJ^DB+NOUN+ZERO+A3SG+PNON+GEN^DB+VERB+ZERO$VERB^DB+VERB+PASS+POS") {
                    return "VERB^DB+VERB+PASS+POS";
                } else {
                    /* KIZIM */
                    if (parseString == "ADJ^DB+VERB+ZERO+PRES+A1SG$NOUN+A3SG+P1SG+NOM$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A1SG") {
                        return "NOUN+A3SG+P1SG+NOM";
                    } else {
                        /* etmeliydi, yaratmalıydı */
                        if (parseString == "POS+NECES$POS^DB+NOUN+INF2+A3SG+PNON+NOM^DB+ADJ+WITH^DB+VERB+ZERO") {
                            return "POS+NECES";
                        } else {
                            /* HERKESİN */
                            if (parseString == "NOUN+A3SG+P2SG+NOM$NOUN+A3SG+PNON+GEN$PRON+QUANTP+A3PL+P3PL+GEN") {
                                return "PRON+QUANTP+A3PL+P3PL+GEN";
                            } else {
                                if (parseString == "ADJ+JUSTLIKE^DB+NOUN+ZERO+A3SG+P2SG$ADJ+JUSTLIKE^DB+NOUN+ZERO+A3SG+PNON$NOUN+ZERO+A3SG+P3SG") {
                                    return "NOUN+ZERO+A3SG+P3SG";
                                } else {
                                    /* milyarlık, milyonluk, beşlik, ikilik */
                                    if (parseString == "NESS+A3SG+PNON+NOM$ZERO+A3SG+PNON+NOM^DB+ADJ+FITFOR") {
                                        return "ZERO+A3SG+PNON+NOM^DB+ADJ+FITFOR";
                                    } else {
                                        /* alınmamaktadır, koymamaktadır */
                                        if (parseString == "NEG+PROG2$NEG^DB+NOUN+INF+A3SG+PNON+LOC^DB+VERB+ZERO+PRES") {
                                            return "NEG+PROG2";
                                        } else {
                                            /* HEPİMİZ */
                                            if (parseString == "A1PL+P1PL+NOM$A3SG+P3SG+GEN^DB+VERB+ZERO+PRES+A1PL") {
                                                return "A1PL+P1PL+NOM";
                                            } else {
                                                /* KİMSENİN */
                                                if (parseString == "NOUN+A3SG+P2SG$NOUN+A3SG+PNON$PRON+QUANTP+A3SG+P3SG") {
                                                    return "PRON+QUANTP+A3SG+P3SG";
                                                } else {
                                                    /* GEÇMİŞ, ALMIŞ, VARMIŞ */
                                                    if (parseString == "ADJ^DB+VERB+ZERO+NARR+A3SG$VERB+POS+NARR+A3SG$VERB+POS+NARR^DB+ADJ+ZERO") {
                                                        if (isNextWordNounOrAdjective(index, fsmParses, length)){
                                                            return "VERB+POS+NARR^DB+ADJ+ZERO";
                                                        }
                                                        return "VERB+POS+NARR+A3SG";
                                                    } else {
                                                        /* yapacağınız, konuşabileceğiniz, olacağınız */
                                                        if (parseString == "ADJ+FUTPART+P2PL$NOUN+FUTPART+A3SG+P2PL+NOM$NOUN+FUTPART+A3SG+PNON+GEN^DB+VERB+ZERO+PRES+A1PL") {
                                                            if (isNextWordNounOrAdjective(index, fsmParses, length)){
                                                                return "ADJ+FUTPART+P2PL";
                                                            }
                                                            return "NOUN+FUTPART+A3SG+P2PL+NOM";
                                                        } else {
                                                            /* YILINA, DİLİNE, YOLUNA */
                                                            if (parseString == "NOUN+A3SG+P2SG+DAT$NOUN+A3SG+P3SG+DAT$VERB^DB+VERB+PASS+POS+OPT+A3SG") {
                                                                if (isAnyWordSecondPerson(index, correctParses)){
                                                                    return "NOUN+A3SG+P2SG+DAT";
                                                                }
                                                                return "NOUN+A3SG+P3SG+DAT";
                                                            } else {
                                                                /* MİSİN, MİYDİ, MİSİNİZ */
                                                                if (parseString == "NOUN+A3SG+PNON+NOM^DB+VERB+ZERO$QUES") {
                                                                    return "QUES";
                                                                } else {
                                                                    /* ATAKLAR, GÜÇLER, ESASLAR */
                                                                    if (parseString == "ADJ^DB+NOUN+ZERO+A3PL+PNON+NOM$ADJ^DB+VERB+ZERO+PRES+A3PL$NOUN+A3PL+PNON+NOM$NOUN+A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL") {
                                                                        return "NOUN+A3PL+PNON+NOM";
                                                                    } else {
                                                                        if (parseString == "A3PL+P3SG$A3SG+P3PL$PROP+A3PL+P3PL") {
                                                                            return "PROP+A3PL+P3PL";
                                                                        } else {
                                                                            /* pilotunuz, suçunuz, haberiniz */
                                                                            if (parseString == "P2PL+NOM$PNON+GEN^DB+VERB+ZERO+PRES+A1PL") {
                                                                                return "P2PL+NOM";
                                                                            } else {
                                                                                /* yıllarca, aylarca, düşmanca */
                                                                                if (parseString == "ADJ+ASIF$ADV+LY") {
                                                                                    if (isNextWordNounOrAdjective(index, fsmParses, length)){
                                                                                        return "ADJ+ASIF";
                                                                                    }
                                                                                    return "ADV+LY";
                                                                                } else {
                                                                                    /* gerçekçi, alıcı */
                                                                                    if (parseString == "ADJ^DB+NOUN+AGT+A3SG+PNON+NOM$NOUN+A3SG+PNON+NOM^DB+ADJ+AGT") {
                                                                                        if (isNextWordNounOrAdjective(index, fsmParses, length)){
                                                                                            return "NOUN+A3SG+PNON+NOM^DB+ADJ+AGT";
                                                                                        }
                                                                                        return "ADJ^DB+NOUN+AGT+A3SG+PNON+NOM";
                                                                                    } else {
                                                                                        /* havayollarına, gözyaşlarına */
                                                                                        if (parseString == "P2SG$P3PL$P3SG") {
                                                                                            if (isAnyWordSecondPerson(index, correctParses)){
                                                                                                return "P2SG";
                                                                                            }
                                                                                            if (isPossessivePlural(index, correctParses)){
                                                                                                return "P3PL";
                                                                                            }
                                                                                            return "P3SG";
                                                                                        } else {
                                                                                            /* olun, kurtulun, gelin */
                                                                                            if (parseString == "VERB+POS+IMP+A2PL$VERB^DB+VERB+PASS+POS+IMP+A2SG") {
                                                                                                return "VERB+POS+IMP+A2PL";
                                                                                            } else {
                                                                                                if (parseString == "ADJ+JUSTLIKE^DB$NOUN+ZERO+A3SG+P3SG+NOM^DB") {
                                                                                                    return "NOUN+ZERO+A3SG+P3SG+NOM^DB";
                                                                                                } else {
                                                                                                    /* oluşmaktaydı, gerekemekteydi */
                                                                                                    if (parseString == "POS+PROG2$POS^DB+NOUN+INF+A3SG+PNON+LOC^DB+VERB+ZERO") {
                                                                                                        return "POS+PROG2";
                                                                                                    } else {
                                                                                                        /* BERABER */
                                                                                                        if (parseString == "ADJ$ADV$POSTP+PCINS") {
                                                                                                            if (hasPreviousWordTag(index, correctParses, MorphologicalTag::INSTRUMENTAL)) {
                                                                                                                return "POSTP+PCINS";
                                                                                                            }
                                                                                                            if (isNextWordNounOrAdjective(index, fsmParses, length)){
                                                                                                                return "ADJ";
                                                                                                            }
                                                                                                            return "ADV";
                                                                                                        } else {
                                                                                                            /* BİN, KIRK */
                                                                                                            if (parseString == "NUM+CARD$VERB+POS+IMP+A2SG") {
                                                                                                                return "NUM+CARD";
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
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* ÖTE */
    if (parseString == "NOUN+A3SG+PNON+NOM$POSTP+PCABL") {
        if (hasPreviousWordTag(index, correctParses, MorphologicalTag::ABLATIVE)) {
            return "POSTP+PCABL";
        }
        return "NOUN+A3SG+PNON+NOM";
    } else {
        /* BENİMLE */
        if (parseString == "NOUN+A3SG+P1SG$PRON+PERS+A1SG+PNON") {
            return "PRON+PERS+A1SG+PNON";
        } else {
            /* Accusative and Ablative Cases*/
            if (parseString == "ADV+WITHOUTHAVINGDONESO$NOUN+INF2+A3SG+PNON+ABL") {
                return "ADV+WITHOUTHAVINGDONESO";
            } else {
                 if (parseString == "ADJ^DB+NOUN+ZERO+A3SG+P3SG+NOM$ADJ^DB+NOUN+ZERO+A3SG+PNON+ACC$NOUN+A3SG+P3SG+NOM$NOUN+A3SG+PNON+ACC") {
                     return "ADJ^DB+NOUN+ZERO+A3SG+P3SG+NOM";
                 } else {
                     if (parseString == "P3SG+NOM$PNON+ACC") {
                         if (fsmParses[index].getFsmParse(0).getFinalPos() == "PROP") {
                             return "PNON+ACC";
                         } else {
                             return "P3SG+NOM";
                         }
                     } else {
                         if (parseString == "A3PL+PNON+NOM$A3SG+PNON+NOM^DB+VERB+ZERO+PRES+A3PL") {
                             return "A3PL+PNON+NOM";
                         } else {
                             if (parseString == "ADV+SINCE$VERB+ZERO+PRES+COP+A3SG") {
                                 if (root == "yıl" || root == "süre" || root == "zaman" || root == "ay") {
                                     return "ADV+SINCE";
                                 } else {
                                     return "VERB+ZERO+PRES+COP+A3SG";
                                 }
                             } else {
                                 if (parseString == "CONJ$VERB+POS+IMP+A2SG") {
                                     return "CONJ";
                                 } else {
                                     if (parseString == "NEG+IMP+A2SG$POS^DB+NOUN+INF2+A3SG+PNON+NOM") {
                                         return "POS^DB+NOUN+INF2+A3SG+PNON+NOM";
                                     } else {
                                         if (parseString == "NEG+OPT+A3SG$POS^DB+NOUN+INF2+A3SG+PNON+DAT") {
                                             return "POS^DB+NOUN+INF2+A3SG+PNON+DAT";
                                         } else {
                                             if (parseString == "NOUN+A3SG+P3SG+NOM$NOUN^DB+ADJ+ALMOST") {
                                                 return "NOUN+A3SG+P3SG+NOM";
                                             } else {
                                                 if (parseString == "ADJ$VERB+POS+IMP+A2SG") {
                                                     return "ADJ";
                                                 } else {
                                                     if (parseString == "NOUN+A3SG+PNON+NOM$VERB+POS+IMP+A2SG") {
                                                         return "NOUN+A3SG+PNON+NOM";
                                                     } else {
                                                         if (parseString == "INF2+A3SG+P3SG+NOM$INF2^DB+ADJ+ALMOST$") {
                                                             return "INF2+A3SG+P3SG+NOM";
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
    return "";
}

FsmParse
AutoDisambiguator::caseDisambiguator(int index, FsmParseList *fsmParses, vector<FsmParse> correctParses, int length) {
    FsmParseList fsmParseList = fsmParses[index];
    string caseString = fsmParses[index].parsesWithoutPrefixAndSuffix();
    string defaultCase = selectCaseForParseString(caseString, index, fsmParses, correctParses, length);
    if (!defaultCase.empty()) {
        for (int i = 0; i < fsmParseList.size(); i++) {
            FsmParse fsmParse = fsmParseList.getFsmParse(i);
            if (fsmParse.getTransitionList().find(defaultCase) != std::string::npos) {
                return fsmParse;
            }
        }
    }
    return fsmParseList.getFsmParse(0);
}
