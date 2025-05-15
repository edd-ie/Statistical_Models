cmake_minimum_required(VERSION 3.30)
project(MachineLearning)

set(CMAKE_CXX_STANDARD 26)

add_executable(MachineLearning main.cpp
        Dataset.h
        Dataset.cpp
        Utilities/Logger.h
        Utilities/Logger.cpp
        Utilities/FileReader.cpp
        Utilities/FileReader.h
        Tables/ZScore.h
        Tables/ZScore.cpp
        Models/BlackScholes.h
        Models/BlackScholes.cpp
        Options/Payoff.cpp
        Options/Payoff.h
        Options/OptionInfo.cpp
        Options/OptionInfo.h
        Models/BinomialLattice.cpp
        Models/BinomialLattice.h
        Models/MonteCarlo.cpp
        Models/MonteCarlo.h)
