//
// Created by Olcay Taner YILDIZ on 6.01.2021.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <FsmMorphologicalAnalyzer.h>
#include "catch.hpp"
#include "../RootWordStatistics.h"

TEST_CASE("RootWordStatisticsTest-testRootWordStatistics") {
    FsmMorphologicalAnalyzer fsm = FsmMorphologicalAnalyzer();
    RootWordStatistics rootWordStatistics = RootWordStatistics("penntreebank_statistics.txt");
    REQUIRE_FALSE(!rootWordStatistics.containsKey("yasasını"));
    REQUIRE_FALSE(!rootWordStatistics.containsKey("yapılandırıyorlar"));
    REQUIRE_FALSE(!rootWordStatistics.containsKey("çöküşten"));
    REQUIRE("yasa" == rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("yasasını"), 0.0));
    REQUIRE("karşılaş" == rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("karşılaşabilir"), 0.0));
    REQUIRE(rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("karşılaşabilir"), 0.7).empty());
    REQUIRE("anlat" == rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("anlattı"), 0.0));
    REQUIRE(rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("anlattı"), 0.9).empty());
    REQUIRE("ver" == rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("vermesini"), 0.0));
    REQUIRE(rootWordStatistics.bestRootWord(fsm.morphologicalAnalysis("vermesini"), 0.9).empty());
}