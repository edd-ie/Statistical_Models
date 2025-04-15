#include <iostream>
#include <memory>

#include "Dataset.h"

#include <cmath>
#include <vector>

// Function to calculate the cumulative distribution function (CDF) for the standard normal distribution
double normalCDF(double value) {
    return 0.5 * erfc(-value * M_SQRT1_2);
}

// Function to generate a Z-table
std::vector<std::vector<double>> generateZTable() {
    std::vector<std::vector<double>> zTable(10, std::vector<double>(10));
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            double zValue = i * 0.1 + j * 0.01;
            zTable[i][j] = normalCDF(zValue);
        }
    }
    return zTable;
}

// Function to print the Z-table
void printZTable(const std::vector<std::vector<double>>& zTable) {
    std::cout << "Z-Table:\n";
    for (const auto& row : zTable) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    // std::unique_ptr<Dataset> test = make_unique<Dataset>();
    //
    // test->importData("../resources/dummy2.csv");
    // test->printData();

    auto zTable = generateZTable();
    printZTable(zTable);
    return 0;

    //TODO: implement Z-model and T-model

    return 0;
}