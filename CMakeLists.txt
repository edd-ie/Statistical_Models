cmake_minimum_required(VERSION 3.30)
project(MachineLearning)

set(CMAKE_CXX_STANDARD 26)

# Find CURL package installed by vcpkg
find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# --- Find Boost components that you need (including Boost.Math) ---
# This will locate the Boost libraries installed by Vcpkg
find_package(Boost REQUIRED COMPONENTS math multi_array)



add_executable(MachineLearning main.cpp
        Dataset.cpp
        Utilities/Logger.cpp
        Utilities/FileReader.cpp
        Tables/ZScore.cpp
        Models/BlackScholes.cpp
        Options/Payoff.cpp
        Options/OptionInfo.cpp
        Models/BinomialLattice.cpp
        Models/MonteCarlo.cpp
        Models/EquityPriceGenerator.cpp
        Test/TestFile.cpp
        Utilities/Date.cpp
        Analytics/YieldCurve.cpp
        Tools/Bond.cpp
)

# If you have specific include directories needed for your own headers (e.g., "Test", "Utilities", "Tables", "Models", "Options")
target_include_directories(MachineLearning PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR} # Include current directory for things like TestFile.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
        ${CMAKE_CURRENT_SOURCE_DIR}/Tables
        ${CMAKE_CURRENT_SOURCE_DIR}/Models
        ${CMAKE_CURRENT_SOURCE_DIR}/Options
        ${CMAKE_CURRENT_SOURCE_DIR}/Tools
        ${CMAKE_CURRENT_SOURCE_DIR}/Test
        ${CMAKE_CURRENT_SOURCE_DIR}/Analytics
)

# Link against CURL
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl
        nlohmann_json::nlohmann_json
        Boost::math
        Boost::multi_array)

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)