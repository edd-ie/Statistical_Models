//
// Created by _edd.ie_ on 13/04/2025.
//

#ifndef DATASET_H
#define DATASET_H
#include <string>
#include <vector>

using namespace std;
class Dataset {
    public:
        vector<string> fields;
        vector<double> data;
        Dataset();
        ~Dataset();
        bool importData(const string& fileName);
        void printData() const;
};

#endif //DATASET_H
