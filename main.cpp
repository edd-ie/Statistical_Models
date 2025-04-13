#include <iostream>
#include <memory>

#include "Dataset.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::unique_ptr<Dataset> test = make_unique<Dataset>();

    test->importData("../resources/dummy2.csv");
    test->printData();

    //TODO: implement Z-model and T-model

    return 0;
}