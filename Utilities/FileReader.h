//
// Created by _edd.ie_ on 13/04/2025.
//

#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Logger.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::string;

// reads the file contents and store them in a std::string object
inline string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        Logger::log(1, "%s: Could not open the file - %s\n", __FUNCTION__, path);
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream ss;
    ss << input_file.rdbuf();
    return ss.str();
}

// Convert string to a double
inline double stringToDouble(const std::string& str) {
    std::stringstream ss(str);
    double result;

    if (ss >> result) {
        // Check if there are any remaining characters after the double was read.
        char remaining;
        if (ss >> remaining) {
            // There were extra characters, so the conversion failed.
            Logger::log(1, "%s: Error with string to double conversion.\n", __FUNCTION__);
            throw std::invalid_argument("String contains non-numeric characters after the double.");
        }
        return result;
    }
    // The string could not be converted to a double.
    Logger::log(1, "%s: Error with string to double conversion.\n", __FUNCTION__);
    throw std::invalid_argument("String is not a valid double.");
}



#endif //FILEREADER_H
