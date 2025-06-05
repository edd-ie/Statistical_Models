//
// Created by _edd.ie_ on 15/05/2025.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H


#include "../Options/OptionInfo.h"
#include "EquityPriceGenerator.h"

#include <utility>            // std::move
#include <cmath>
#include <vector>
#include <numeric>            // std::accumulate


/**\
 * Longstaff and Schwartz least squares Monte-Carlo model,
 * Resource - https://www.jstor.org/stable/2696758?seq=7
 *
 * Generating a single random price scenario.
 * To calculate the option price, iterate through a set of seeds to:
 * - Generate thousands of distinct and varying scenarios
 * - Calculate the terminal payoffs,
 * - Take the mean of the discounted payoffs.
 */

class MonteCarlo {
    OptionInfo option;
    unsigned timeSteps;
    double vol, intRate, divRate;

public:
    MonteCarlo(OptionInfo&& opt, unsigned timeStep, double vol, double intRate, double divRate):
        option(opt), timeSteps(timeStep), vol(vol), intRate(intRate), divRate(divRate){}

    double calculatePrice(double spot, unsigned unifStartSeed, int numScenarios);
};



#endif //MONTECARLO_H
