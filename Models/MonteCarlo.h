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
#include <algorithm>
#include <future>


/**\
 * Longstaff and Schwartz least squares Monte-Carlo model,
 * Resource - https://www.jstor.org/stable/2696758?seq=7
 *
 */


// Barrier option for valuing path-dependent options.
enum class BarrierType {
    none,
    up_and_out,
    down_and_out
};

class MonteCarlo {
    OptionInfo option;
    unsigned timeSteps;
    double vol, intRate, divRate, barrierValue;
    BarrierType barrierType;

public:
    MonteCarlo(OptionInfo&& opt, unsigned timeStep, double vol, double intRate, double divRate=0.0,
        double barrierVal=0.0, BarrierType barrierType=BarrierType::none):
         option(std::move(opt)), timeSteps(timeStep), vol(vol), intRate(intRate), divRate(divRate),
        barrierValue(barrierVal), barrierType(barrierType){}

    double calculatePrice(double spot, unsigned unifStartSeed, int numScenarios);
    double calculatePrice_par(double spot, unsigned unifStartSeed, int numScenarios); // parallel version
};



#endif //MONTECARLO_H
