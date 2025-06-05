//
// Created by _edd.ie_ on 05/06/2025.
//

#ifndef EQUITYPRICEGENERATOR_H
#define EQUITYPRICEGENERATOR_H

#include <vector>
#include <random>

/**
 * S_t = S_(t-1) * e^[(r-q-σ^2/2)Δt + σε_t*sqrt(Δt)] *
 * assumption: all the constructor arguments are greater than zero
 */


class EquityPriceGenerator {
    double spot, volatility,expiry, rfRate, divRate;
    unsigned timeSteps;
    double deltaT{};
public:
    EquityPriceGenerator(double spot, double vol, double expiry, double rate, double divdend, unsigned timeSteps):
        spot(spot), volatility(vol), expiry(expiry), rfRate(rate), divRate(divdend), timeSteps(timeSteps){}

    // Returns the simulated random path of equity share prices
    std::vector<double> operator()(unsigned seed) const;
};

#endif //EQUITYPRICEGENERATOR_H
