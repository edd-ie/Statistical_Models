//
// Created by _edd.ie_ on 13/04/2025.
//
#include "Dataset.h"
#include "Utilities/Logger.h"
#include "Utilities/FileReader.h"
#include <algorithm>
#include <cctype> // for isspace
#include <iomanip> // Required for formatting output

Dataset::Dataset() = default;
Dataset::~Dataset() = default;


bool Dataset::importData(const string& fileName) {
    try {
        string fileContents = readFileIntoString(fileName);
        if (fileContents.empty()) {
            Logger::log(1, "%s: Failed to read file: %s\n", __FUNCTION__, fileName);
            return false;
        }

        istringstream sstream(fileContents);
        string record;
        bool firstLine = true;
        char delimiter = ',';

        while (std::getline(sstream, record)) {
            istringstream line(record);
            while (std::getline(line, record, delimiter)) {
                record.erase(ranges::remove_if(record, ::isspace).begin(), record.end());

                if (firstLine) fields.push_back(record);
                else data.push_back(stringToDouble(record));
            }
            if (firstLine) firstLine = false;
        }

        return true;
    } catch (const std::exception& e) {
        Logger::log(1, "%s: Error importing data: %s\n", __FUNCTION__, e.what());
        return false;
    }
}

// Function to print the data in tabular format
void Dataset::printData() const {
    if (fields.empty()) {
        std::cout << "No fields (column headers) to display." << std::endl;
        return;
    }
    if (data.empty()) {
        std::cout << "No data to display." << std::endl;
        return;
    }

    // Calculate the number of columns and rows
    size_t numCols = fields.size();
    size_t numRows = data.size() / numCols;

    if (data.size() % numCols != 0) {
        std::cout << "Error: Inconsistent data size.  Not a proper table." << std::endl;
        return;
    }
    // Determine maximum column widths for formatting
    std::vector<size_t> columnWidths(numCols, 0);
    for (size_t i = 0; i < numCols; ++i) {
        columnWidths[i] = fields[i].length(); // Start with header width
    }
    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            size_t dataLength = std::to_string(data[i * numCols + j]).length();
            if (dataLength > columnWidths[j]) {
                columnWidths[j] = dataLength;
            }
        }
    }

    // Print the header row
    for (size_t j = 0; j < numCols; ++j) {
        std::cout << std::setw(columnWidths[j] + 2) << fields[j]; // Add 2 for padding
    }
    std::cout << std::endl;

    // Print a separator line
    for (size_t j = 0; j < numCols; ++j) {
        for(size_t k = 0; k < columnWidths[j] + 2; ++k)
            std::cout << "-";
    }
    std::cout << std::endl;
    // Print the data rows
    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            std::cout << std::setw(columnWidths[j] + 2) << data[i * numCols + j];
        }
        std::cout << std::endl;
    }
}
