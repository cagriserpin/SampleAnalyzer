cmake_minimum_required(VERSION 3.13)
project(EdgeAnalyzer)

add_definitions(-DLOGIC2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(ExternalAnalyzerSDK)

set(SOURCES
    src/EdgeAnalyzer.cpp
    src/EdgeAnalyzer.h
    src/EdgeAnalyzerResults.cpp
    src/EdgeAnalyzerResults.h
    src/EdgeAnalyzerSettings.cpp
    src/EdgeAnalyzerSettings.h
    src/EdgeSimulationDataGenerator.cpp
    src/EdgeSimulationDataGenerator.h
)

add_analyzer_plugin(${PROJECT_NAME} SOURCES ${SOURCES})
