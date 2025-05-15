//
// Created by _edd.ie_ on 15/05/2025.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <utility>
#include "../Options/OptionInfo.h"


/**\
 * Longstaff and Schwartz least squares Monte Carlo model,
 * Resource - https://www.jstor.org/stable/2696758?seq=7
 */

class MonteCarlo {
    OptionInfo option;

public:
    MonteCarlo(OptionInfo&& opt, double vol, double intRate, int timeStep, double divRate);
    double calculatePrice(double spot, int unifStartSeed, int numScenarios);
};



#endif //MONTECARLO_H
