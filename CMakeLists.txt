cmake_minimum_required(VERSION 3.0)

project(partsEngine VERSION 0.0.1 DESCRIPTION "Library for creating and manipulating system of magnetic particles")

find_package(Qt5 COMPONENTS Core REQUIRED)


set(SOURCES 
    Vect.cpp 
    config.cpp 
    Part.cpp 
    distributionLaw.cpp 
    statemachinebase.cpp 
    PartArray.cpp 
    StateMachine.cpp 
    StateMachineFree.cpp 
    squarespinicearray.cpp 
    squarespinicecell.cpp 
    wanglandau.cpp 
    honeycombspinicearray.cpp 
    honeycombspinicecell.cpp 
    squarelattice.cpp 
    amorphousarray.cpp 
    loadhelper.cpp 
    savehelper.cpp 
    squareisinglattice.cpp 
    clustermachine.cpp 
    dos2.cpp 
    dos3.cpp 
    random.cpp)

set(HEADERS 
    Vect.h 
    random.h 
    Part.h 
    statemachinebase.h 
    StateMachine.h 
    StateMachineFree.h 
    PartArray.h 
    config.h 
    squarespinicecell.h 
    squarespinicearray.h 
    honeycombspinicecell.h 
    honeycombspinicearray.h 
    squarelattice.h 
    distributionLaw.h 
    wanglandau.h 
    amorphousarray.h 
    loadhelper.h 
    savehelper.h 
    squareisinglattice.h 
    clustermachine.h 
    dos2.h 
    dos3.h 
    utils.h 
    brickworka.h 
    brickworkc.h)

set(OTHER_FILES 
    README.md 
    .gitignore)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x")

option(BUILD_STATIC_LIB "Build static library" ON)

if(BUILD_STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core)