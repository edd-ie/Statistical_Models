//
// Created by _edd.ie_ on 06/06/2025.
//

#ifndef TESTFILE_H
#define TESTFILE_H

#include <iostream>
#include <memory>
#include <cmath>
#include <vector>
#include <format>
#include <iostream>

#include "../Dataset.h"
#include "../Options/Payoff.h"
#include "../Tables/ZScore.h"
#include "../Models/BinomialLattice.h"
#include "../Models/BlackScholes.h"
#include "../Options/OptionInfo.h"
#include "../Models/MonteCarlo.h"
#include "../Utilities/Date.h"
#include "../tools/Bond.h"

void testZmodel();
void testBlackScholes();
void testMonteCarlo();
void testDate();
void testBond();


#endif //TESTFILE_H
