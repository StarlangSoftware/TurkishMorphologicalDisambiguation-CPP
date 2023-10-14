cmake_minimum_required(VERSION 3.12.3)
project(MorphologicalDisambiguation)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(MorphologicalDisambiguation src/AutoDisambiguator.h src/MorphologicalDisambiguator.h src/DummyDisambiguation.cpp src/DummyDisambiguation.h src/NaiveDisambiguation.cpp src/NaiveDisambiguation.h src/HmmDisambiguation.cpp src/HmmDisambiguation.h src/RootFirstDisambiguation.cpp src/RootFirstDisambiguation.h src/LongestRootFirstDisambiguation.cpp src/LongestRootFirstDisambiguation.h src/AutoDisambiguator.cpp src/AutoProcessor/Sentence/SentenceAutoDisambiguator.cpp src/AutoProcessor/Sentence/SentenceAutoDisambiguator.h src/AutoProcessor/Sentence/TurkishSentenceAutoDisambiguator.cpp src/AutoProcessor/Sentence/TurkishSentenceAutoDisambiguator.h)
add_executable(Test src/AutoDisambiguator.h src/MorphologicalDisambiguator.h src/DummyDisambiguation.cpp src/DummyDisambiguation.h src/NaiveDisambiguation.cpp src/NaiveDisambiguation.h src/HmmDisambiguation.cpp src/HmmDisambiguation.h src/RootFirstDisambiguation.cpp src/RootFirstDisambiguation.h src/LongestRootFirstDisambiguation.cpp src/LongestRootFirstDisambiguation.h src/AutoDisambiguator.cpp Test/DummyDisambiguationTest.cpp Test/HmmDisambiguationTest.cpp Test/LongestRootFirstDisambiguationTest.cpp Test/RootFirstDisambiguationTest.cpp src/AutoProcessor/Sentence/SentenceAutoDisambiguator.cpp src/AutoProcessor/Sentence/SentenceAutoDisambiguator.h src/AutoProcessor/Sentence/TurkishSentenceAutoDisambiguator.cpp src/AutoProcessor/Sentence/TurkishSentenceAutoDisambiguator.h)
target_link_libraries(MorphologicalDisambiguation ${CONAN_LIBS})
target_link_libraries(Test ${CONAN_LIBS})